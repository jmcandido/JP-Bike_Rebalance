#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <string.h>


using namespace std;

struct Rota {
    vector<int> caminho;
    int custo;
    int cargaInicial;
};

struct Instancia {
    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;
};

struct Diagnostico {
    int coletas = 0;
    int entregas = 0;
    int deficit = 0;      // mín. que precisa sair do depósito
    int capTotal = 0;     // m * Q
    bool inviavel = false;
};

Instancia lerInstancia(const string& nomeArquivo) {
    ifstream in(nomeArquivo);
    if (!in) {
        cerr << "Erro ao abrir o arquivo!\n";
        exit(1);
    }
    Instancia inst;
    in >> inst.n >> inst.m >> inst.Q;

    inst.d.resize(inst.n + 1);
    inst.d[0] = 0;
    for (int i = 1; i <= inst.n; i++) in >> inst.d[i];

    inst.c.assign(inst.n + 1, vector<int>(inst.n + 1));
    for (int i = 0; i <= inst.n; i++)
        for (int j = 0; j <= inst.n; j++)
            in >> inst.c[i][j];

    return inst;
}

Diagnostico diagnosticarInstancia(const Instancia& inst) {
    Diagnostico diag;
    for (int i = 1; i <= inst.n; i++) {
        if (inst.d[i] > 0) diag.coletas += inst.d[i];
        else               diag.entregas += abs(inst.d[i]);
    }
    diag.deficit = max(0, diag.entregas - diag.coletas);
    diag.capTotal = inst.m * inst.Q;
    diag.inviavel = (diag.deficit > diag.capTotal);

    cout << "\n=== Informações da Instância ===\n";
    cout << "Total de coletas: " << diag.coletas << '\n';
    cout << "Total de entregas: " << diag.entregas << '\n';
    cout << "Déficit global (mínimo a sair do depósito): " << diag.deficit << '\n';
    cout << "Capacidade total disponível: " << diag.capTotal << '\n';
    if (diag.inviavel) {
        cout << "⚠️ Instância inviável: déficit maior que capacidade total dos caminhões!\n";
    }
    return diag;
}

/// --- Função para checar se uma rota é viável ---

bool rotaViavel(const vector<int>& caminho, const vector<int>& d, int Q, int cargaInicial = 0) {
    int carga = cargaInicial;
    for (int v : caminho) {
        carga += d[v];
        if (carga < 0 || carga > Q) return false;
    }
    return true;
}


/// --- Função para calcular o custo de uma rota ---
int calcularCusto(const vector<int>& caminho, const vector<vector<int>>& c) {
    int custo = 0;
    for (int i = 0; i < (int)caminho.size() - 1; i++)
        custo += c[caminho[i]][caminho[i+1]];
    return custo;
}

// --- Helpers só para depuração ---
static string pathToStr(const vector<int>& p) {
    string s;
    for (int i = 0; i < (int)p.size(); ++i) {
        s += to_string(p[i]);
        if (i + 1 < (int)p.size()) s += " -> ";
    }
    return s;
}

static void printNaoVisitados(const vector<bool>& visitado) {
    cout << "Ainda não visitados: ";
    bool any = false;
    for (int i = 1; i < (int)visitado.size(); ++i) {
        if (!visitado[i]) {
            cout << i << " ";
            any = true;
        }
    }
    if (!any) cout << "(nenhum)";
    cout << "\n";
}

/// --- Inserção Mais Barata com depuração ---
vector<Rota> insercaoMaisBarata(int n, int m, int Q,
                                const vector<int>& d,
                                const vector<vector<int>>& c) {
    vector<bool> visitado(n+1, false);
    visitado[0] = true; // evita listar depósito como "não visitado"
    vector<Rota> rotas;

    cout << "\n[DEBUG] Iniciando construtivo (n=" << n
         << ", m=" << m << ", Q=" << Q << ")\n";
    printNaoVisitados(visitado);

    for (int k = 0; k < m; k++) {
        // critério de parada: se já cobriu todas as estações
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) { todasVisitadas = false; break; }
        }
        if (todasVisitadas) {
            cout << "[DEBUG] Todos os clientes já foram visitados antes de abrir o veículo "
                 << (k+1) << ". Encerrando.\n";
            break;
        }

        Rota rota;
        rota.caminho = {0, 0};
        rota.custo = 0;
        rota.cargaInicial = 0;
        bool primeiroClienteInserido = false;

        cout << "\n=== Veículo " << (k+1) << " ===\n";
        cout << "[DEBUG] Caminho inicial: " << pathToStr(rota.caminho) << "\n";

        while (true) {
            int melhorCliente = -1, melhorPos = -1, melhorDelta = INT_MAX;

            // tenta inserir cada cliente não visitado em todas as posições
            for (int u = 1; u <= n; u++) {
                if (visitado[u]) continue;

                int cargaInicialTeste = rota.cargaInicial;
                if (!primeiroClienteInserido) {
                    cargaInicialTeste = max(0, -d[u]);
                }

                for (int pos = 0; pos < (int)rota.caminho.size() - 1; pos++) {
                    int a = rota.caminho[pos];
                    int b = rota.caminho[pos+1];

                    // monta caminho de teste
                    vector<int> teste = rota.caminho;
                    teste.insert(teste.begin() + pos + 1, u);

                    bool ok = rotaViavel(teste, d, Q, cargaInicialTeste);
                    cout << "[TESTE] inserir cliente " << u
                         << " entre " << a << " e " << b
                         << " | cargaInicialTeste=" << cargaInicialTeste
                         << " | viavel=" << (ok ? "sim" : "nao");

                    if (!ok) { cout << "\n"; continue; }

                    int delta = c[a][u] + c[u][b] - c[a][b];
                    cout << " | delta=" << delta << "\n";

                    if (delta < melhorDelta) {
                        melhorDelta = delta;
                        melhorCliente = u;
                        melhorPos = pos+1;
                    }
                }
            }

            if (melhorCliente == -1) {
                cout << "[DEBUG] Nenhuma inserção viável restante para o veículo "
                     << (k+1) << ". Fechando rota.\n";
                break;
            }

            // aplica melhor inserção
            rota.caminho.insert(rota.caminho.begin() + melhorPos, melhorCliente);
            rota.custo = calcularCusto(rota.caminho, c);

            if (!primeiroClienteInserido) {
                rota.cargaInicial = max(0, -d[melhorCliente]);
                primeiroClienteInserido = true;
                cout << "[DEBUG] Primeiro cliente da rota: " << melhorCliente
                     << " | cargaInicial definida para " << rota.cargaInicial << "\n";
            }

            visitado[melhorCliente] = true;

            cout << "[ESCOLHA] Inserido cliente " << melhorCliente
                 << " na pos " << melhorPos
                 << " | melhorDelta=" << melhorDelta << "\n";
            cout << "[DEBUG] Caminho atual: " << pathToStr(rota.caminho)
                 << " | custo=" << rota.custo << "\n";

            printNaoVisitados(visitado);
        }

        // fecha rota (se necessário)
        if (rota.caminho.size() > 2) {
            cout << "[DEBUG] Rota do veículo " << (k+1) << " finalizada: "
                 << pathToStr(rota.caminho) << " | custo=" << rota.custo
                 << " | cargaInicial=" << rota.cargaInicial << "\n";
            rotas.push_back(rota);
        } else {
            cout << "[DEBUG] Veículo " << (k+1)
                 << " não atendeu ninguém (rota 0->0). Ignorando.\n";
        }
    }

    // relatório de quem ficou de fora
    cout << "\n[RELATÓRIO] Clientes não atendidos: ";
    bool algum = false;
    for (int i = 1; i <= n; i++) {
        if (!visitado[i]) { cout << i << " "; algum = true; }
    }
    if (!algum) cout << "(nenhum)";
    cout << "\n";

    return rotas;
}


/// --- Função para imprimir as rotas ---
void imprimirRotas(const vector<Rota>& rotas) {
    int custoTotal = 0;
    for (auto &r : rotas) custoTotal += r.custo;

    cout << endl << "=== Resultado Inserção Mais Barata ===\n";
    cout << "Custo total: " << custoTotal << '\n';
    cout << "Veículos usados: " << rotas.size() << '\n';

    for (int k = 0; k < (int)rotas.size(); k++) {
        cout << "Rota " << k+1 << " (custo " << rotas[k].custo 
             << ", carga inicial " << rotas[k].cargaInicial << "): ";
        for (int i = 0; i < (int)rotas[k].caminho.size(); i++) {
            cout << rotas[k].caminho[i];
            if (i < (int)rotas[k].caminho.size()-1) cout << " -> ";
        }
        cout << endl;
    }
}

int main() {
    Instancia inst = lerInstancia("instancia1000.txt");
    Diagnostico diag = diagnosticarInstancia(inst);

    vector<Rota> rotas = insercaoMaisBarata(inst.n, inst.m, inst.Q, inst.d, inst.c);

    imprimirRotas(rotas);

    return 0;
}
