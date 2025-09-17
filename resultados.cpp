#include "resultados.h"
#include <fstream>

// Lê instância do arquivo
bool lerInstancia(const string &nomeArquivo, int &n, int &m, int &Q, vector<int> &d, vector<vector<int>> &c){

    ifstream inst(nomeArquivo);
    if (!inst)
        return false;

    inst >> n >> m >> Q;

    d.resize(n + 1);
    d[0] = 0;

    for (int i = 1; i <= n; i++)
        inst >> d[i];

    c.resize(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            inst >> c[i][j];

    return true;
}

//grava resultado no arquivo
void gravaResultado(const string &pasta, const string &instancia, const Resultado &res){
    // extrai só o nome-base da instância
    string nomeBase = instancia.substr(instancia.find_last_of("/\\") + 1);
    string caminho = pasta + "/" + nomeBase;

    ofstream arquivo(caminho);

    if (!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }

    // grava custos gerais
    arquivo << res.custoFinal << endl;

    // conta somente rotas que tenham pelo menos 1 cliente (caminho > 2)
    int qtdRotasValidas = 0;

    for (const Rota &r : res.rotas){
        if (r.caminho.size() > 2)
        {
            qtdRotasValidas++;
        }
    }
    arquivo << qtdRotasValidas << endl;

    // grava apenas as rotas com pelo menos 1 cliente
    for (const Rota &r : res.rotas){
        if (r.caminho.size() > 2){
            for (int i = 0; i < r.caminho.size(); i++){
                arquivo << r.caminho[i]<< (i + 1 < r.caminho.size() ? ' ' : '\n');
            }
        }
    }
    arquivo.close();
}

// Recalcula o custo de uma rota
int custoRota(const Rota &r, const vector<vector<int>> &c){
    int custo = 0;

    for (int i = 0; i < r.caminho.size() - 1; i++){
        custo += c[r.caminho[i]][r.caminho[i + 1]];
    }

    return custo;
}

// verifica se uma rota é válida
bool validaRota(const Rota &r, const vector<int> &d, const int Q){
    int demandas_acumuladas = 0;
    int min_demanda = 0;
    int max_demanda = 0;

    for (int valor : r.caminho){
        demandas_acumuladas += d[valor];
        min_demanda = min(min_demanda, demandas_acumuladas);
        max_demanda = max(max_demanda, demandas_acumuladas);
    }

    int minima_viavel = -min_demanda;    // carga mínima necessária para não ficar negativa
    int maxima_viavel = Q - max_demanda; // carga máxima permitida para não estourar Q

    return (minima_viavel <= maxima_viavel);

    // existe pelo menos um valor inicial viável
}

// Imprime solução
void imprimirResultado(const Resultado &res){
    cout << res.custoFinal << endl;

    int qtdRotasValidas = 0;

    for (const Rota &r : res.rotas){
        if (r.caminho.size() > 2){
            qtdRotasValidas++;
        }
    }

    cout << qtdRotasValidas << endl;

    for (const Rota &r : res.rotas){
        for (int i = 0; i < r.caminho.size(); i++){
            if (r.caminho.size() > 2){
                cout << r.caminho[i] << (i + 1 < r.caminho.size() ? ' ' : '\n');
            }
        }
    }
}
