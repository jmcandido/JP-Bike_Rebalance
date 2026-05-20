#include "grasp.h"
#include "resultados.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
using SocketHandle = SOCKET;
static const SocketHandle SOCKET_INVALIDO = INVALID_SOCKET;
#else
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using SocketHandle = int;
static const SocketHandle SOCKET_INVALIDO = -1;
#endif

using Query = map<string, string>;

static const int PORTA_PADRAO = 8080;
static const string PASTA_INSTANCIAS = "instancias";
static const string PASTA_WEB = "web";

struct Referencia {
    int valor;
    string tipo;
};

static const map<string, Referencia> REFERENCIAS = {
    {"n12_q20.txt", {15700, "OPT"}},
    {"n12_q30.txt", {14600, "OPT"}},
    {"n13_q30.txt", {16900, "OPT"}},
    {"n14_q12.txt", {13500, "OPT"}},
    {"n14_q30.txt", {12600, "OPT"}},
    {"n17_q10.txt", {31443, "OPT"}},
    {"n17_q20.txt", {29259, "OPT"}},
    {"n20_q20.txt", {91619, "OPT"}},
    {"n20_q30.txt", {76999, "OPT"}},
    {"n26_q20.txt", {31100, "OPT"}},
    {"n26_q30.txt", {30300, "OPT"}},
    {"n40_q20.txt", {59493, "OPT"}},
    {"n40_q30.txt", {57476, "OPT"}},
    {"n54_q30.txt", {120277, "LB"}},
    {"n58_q30.txt", {65669, "OPT"}},
    {"n74_q20.txt", {48829, "LB"}},
    {"n79_q30.txt", {39979, "LB"}},
    {"n81_q10.txt", {388680, "LB"}},
    {"n115_q20.txt", {157115, "LB"}}
};

static string urlDecode(const string &valor){
    string saida;

    for (size_t i = 0; i < valor.size(); i++){
        if (valor[i] == '%' && i + 2 < valor.size()){
            string hex = valor.substr(i + 1, 2);
            saida.push_back(static_cast<char>(strtol(hex.c_str(), nullptr, 16)));
            i += 2;
        }
        else if (valor[i] == '+'){
            saida.push_back(' ');
        }
        else{
            saida.push_back(valor[i]);
        }
    }

    return saida;
}

static Query parseQuery(const string &path){
    Query query;
    size_t pos = path.find('?');
    if (pos == string::npos)
        return query;

    string qs = path.substr(pos + 1);
    stringstream ss(qs);
    string item;

    while (getline(ss, item, '&')){
        size_t eq = item.find('=');
        string chave = urlDecode(item.substr(0, eq));
        string valor = eq == string::npos ? "" : urlDecode(item.substr(eq + 1));
        query[chave] = valor;
    }

    return query;
}

static string limpaPath(const string &path){
    size_t q = path.find('?');
    return q == string::npos ? path : path.substr(0, q);
}

static bool nomeInstanciaSeguro(const string &nome){
    if (nome.empty() || nome.find('/') != string::npos || nome.find('\\') != string::npos)
        return false;

    for (char ch : nome){
        if (!(isalnum(static_cast<unsigned char>(ch)) || ch == '_' || ch == '-' || ch == '.'))
            return false;
    }

    return nome.size() > 4 && nome.substr(nome.size() - 4) == ".txt";
}

static vector<string> listarInstancias(){
    vector<string> arquivos;

#ifdef _WIN32
    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA((PASTA_INSTANCIAS + "\\*.txt").c_str(), &data);

    if (hFind != INVALID_HANDLE_VALUE){
        do{
            if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                arquivos.push_back(data.cFileName);
        } while (FindNextFileA(hFind, &data));

        FindClose(hFind);
    }
#else
    DIR *dir = opendir(PASTA_INSTANCIAS.c_str());
    if (dir){
        dirent *entry;
        while ((entry = readdir(dir)) != nullptr){
            string nome = entry->d_name;
            if (nome.size() > 4 && nome.substr(nome.size() - 4) == ".txt")
                arquivos.push_back(nome);
        }
        closedir(dir);
    }
#endif

    sort(arquivos.begin(), arquivos.end());
    return arquivos;
}

static string jsonEscape(const string &s){
    string out;
    for (char ch : s){
        switch (ch){
        case '"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default: out.push_back(ch);
        }
    }
    return out;
}

static string respostaJson(int status, const string &body){
    string textoStatus = status == 200 ? "OK" : status == 400 ? "Bad Request" : status == 404 ? "Not Found" : "Internal Server Error";
    stringstream res;
    res << "HTTP/1.1 " << status << " " << textoStatus << "\r\n"
        << "Content-Type: application/json; charset=utf-8\r\n"
        << "Cache-Control: no-store\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Connection: close\r\n\r\n"
        << body;
    return res.str();
}

static string respostaArquivo(const string &body, const string &contentType){
    stringstream res;
    res << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: " << contentType << "; charset=utf-8\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Connection: close\r\n\r\n"
        << body;
    return res.str();
}

static string erroJson(int status, const string &mensagem){
    return respostaJson(status, "{\"erro\":\"" + jsonEscape(mensagem) + "\"}");
}

static string apiInstancias(){
    vector<string> arquivos = listarInstancias();
    stringstream json;
    json << "{\"instancias\":[";
    for (size_t i = 0; i < arquivos.size(); i++){
        if (i)
            json << ',';
        json << '"' << jsonEscape(arquivos[i]) << '"';
    }
    json << "]}";
    return respostaJson(200, json.str());
}

static string resultadoParaJson(const string &nome, int n, int m, int Q, const vector<int> &d, const vector<vector<int>> &c, const Resultado &res, long long tempoMs){
    stringstream json;
    json << "{\"instancia\":\"" << jsonEscape(nome) << "\",";
    json << "\"n\":" << n << ",\"m\":" << m << ",\"Q\":" << Q << ',';
    json << "\"custo\":" << res.custoFinal << ",\"tempoMs\":" << tempoMs << ',';

    auto ref = REFERENCIAS.find(nome);
    if (ref != REFERENCIAS.end() && ref->second.valor > 0){
        double gap = ((static_cast<double>(res.custoFinal) - ref->second.valor) / ref->second.valor) * 100.0;
        json << "\"referencia\":{\"valor\":" << ref->second.valor
             << ",\"tipo\":\"" << ref->second.tipo
             << "\",\"gapPercent\":" << gap << "},";
    }
    else{
        json << "\"referencia\":null,";
    }

    json << "\"demandas\":[";
    for (int i = 0; i <= n; i++){
        if (i)
            json << ',';
        json << d[i];
    }
    json << "],\"custos\":[";
    for (int i = 0; i <= n; i++){
        if (i)
            json << ',';
        json << '[';
        for (int j = 0; j <= n; j++){
            if (j)
                json << ',';
            json << c[i][j];
        }
        json << ']';
    }
    json << "],\"rotas\":[";
    bool primeiraRota = true;
    for (const Rota &rota : res.rotas){
        if (rota.caminho.size() <= 2)
            continue;

        if (!primeiraRota)
            json << ',';
        primeiraRota = false;

        json << "{\"custo\":" << rota.custo << ",\"caminho\":[";
        for (size_t i = 0; i < rota.caminho.size(); i++){
            if (i)
                json << ',';
            json << rota.caminho[i];
        }
        json << "]}";
    }
    json << "]}";

    return json.str();
}

static string apiSolve(const Query &query){
    string instancia = query.count("instance") ? query.at("instance") : "";
    if (!nomeInstanciaSeguro(instancia))
        return erroJson(400, "Instancia invalida.");

    double alpha = query.count("alpha") ? atof(query.at("alpha").c_str()) : 0.35;
    int iteracoes = query.count("iterations") ? atoi(query.at("iterations").c_str()) : 100;

    if (alpha < 0.0 || alpha > 1.0)
        return erroJson(400, "Alpha deve estar entre 0 e 1.");
    if (iteracoes <= 0 || iteracoes > 10000)
        return erroJson(400, "Iteracoes deve estar entre 1 e 10000.");

    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;
    string caminho = PASTA_INSTANCIAS + "/" + instancia;

    if (!lerInstancia(caminho, n, m, Q, d, c))
        return erroJson(404, "Instancia nao encontrada.");

    auto inicio = chrono::high_resolution_clock::now();
    Resultado solucao = GRASP(n, m, Q, d, c, alpha, iteracoes);
    auto fim = chrono::high_resolution_clock::now();
    long long tempoMs = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();

    return respostaJson(200, resultadoParaJson(instancia, n, m, Q, d, c, solucao, tempoMs));
}

static string contentType(const string &path){
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".css")
        return "text/css";
    if (path.size() >= 3 && path.substr(path.size() - 3) == ".js")
        return "application/javascript";
    return "text/html";
}

static string servirArquivo(const string &path){
    string webPath = limpaPath(path);
    if (webPath == "/")
        webPath = "/index.html";
    if (webPath.find("..") != string::npos)
        return erroJson(400, "Caminho invalido.");

    string arquivo = PASTA_WEB + webPath;
    ifstream in(arquivo.c_str(), ios::binary);
    if (!in)
        return erroJson(404, "Arquivo nao encontrado.");

    stringstream buffer;
    buffer << in.rdbuf();
    return respostaArquivo(buffer.str(), contentType(arquivo));
}

static string processarRequisicao(const string &req){
    stringstream ss(req);
    string metodo, path, versao;
    ss >> metodo >> path >> versao;

    if (metodo != "GET")
        return erroJson(400, "Apenas GET e suportado.");

    string rota = limpaPath(path);
    if (rota == "/api/instances")
        return apiInstancias();
    if (rota == "/api/solve")
        return apiSolve(parseQuery(path));

    return servirArquivo(path);
}

static void fecharSocket(SocketHandle sock){
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

int main(int argc, char *argv[]){
    int porta = argc > 1 ? atoi(argv[1]) : PORTA_PADRAO;

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    SocketHandle servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == SOCKET_INVALIDO){
        cerr << "Erro ao criar socket." << endl;
        return 1;
    }

    int opt = 1;
    setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&opt), sizeof(opt));

    sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = htonl(INADDR_ANY);
    endereco.sin_port = htons(static_cast<unsigned short>(porta));

    if (bind(servidor, reinterpret_cast<sockaddr *>(&endereco), sizeof(endereco)) < 0 || listen(servidor, 16) < 0){
        cerr << "Nao foi possivel iniciar o servidor na porta " << porta << "." << endl;
        fecharSocket(servidor);
        return 1;
    }

    cout << "JP-Bike Rebalance web: http://localhost:" << porta << endl;

    while (true){
        sockaddr_in clienteAddr;
#ifdef _WIN32
        int tam = sizeof(clienteAddr);
#else
        socklen_t tam = sizeof(clienteAddr);
#endif
        SocketHandle cliente = accept(servidor, reinterpret_cast<sockaddr *>(&clienteAddr), &tam);
        if (cliente == SOCKET_INVALIDO)
            continue;

        char buffer[8192];
        int recebidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (recebidos > 0){
            buffer[recebidos] = '\0';
            string resposta = processarRequisicao(buffer);
            send(cliente, resposta.c_str(), static_cast<int>(resposta.size()), 0);
        }
        fecharSocket(cliente);
    }

    fecharSocket(servidor);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
