#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <climits>
using namespace std;

struct Rota {
    vector<int> caminho;
    int custo;
    int cargaInicial; // bikes que o caminhão leva ao sair do depósito
};

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
    for (int i = 0; i < (int)caminho.size()-1; i++)
        custo += c[caminho[i]][caminho[i+1]];
    return custo;
}

/// --- Inserção Mais Barata adaptada ---
vector<Rota> insercaoMaisBarata(int n, int m, int Q, const vector<int>& d, const vector<vector<int>>& c) {
    vector<bool> visitado(n+1, false);
    visitado[0] = true;

    vector<Rota> rotas;

    for (int k = 0; k < m; k++) {
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) 
            if (!visitado[i]) { todasVisitadas = false; break; }

        if (todasVisitadas) break;

        Rota rota;
        rota.caminho = {0, 0};
        rota.custo = 0;
        rota.cargaInicial = 0;

        bool primeiroClienteInserido = false;

        while (true) {
            int melhorCliente = -1, melhorPos = -1, melhorDelta = INT_MAX;

            for (int u = 1; u <= n; u++) {
                if (visitado[u]) continue;

                int cargaInicialTeste = rota.cargaInicial;
                if (!primeiroClienteInserido) {
                    cargaInicialTeste = max(0, -d[u]);
                }

                for (int pos = 0; pos < (int)rota.caminho.size()-1; pos++) {
                    vector<int> teste = rota.caminho;
                    teste.insert(teste.begin() + pos + 1, u);

                    if (!rotaViavel(teste, d, Q, cargaInicialTeste)) continue;

                    int delta = c[rota.caminho[pos]][u] + c[u][rota.caminho[pos+1]] - c[rota.caminho[pos]][rota.caminho[pos+1]];
                    if (delta < melhorDelta) {
                        melhorDelta = delta;
                        melhorCliente = u;
                        melhorPos = pos+1;
                    }
                }
            }

            if (melhorCliente == -1) break;

            rota.caminho.insert(rota.caminho.begin() + melhorPos, melhorCliente);
            rota.custo = calcularCusto(rota.caminho, c);

            if (!primeiroClienteInserido) {
                rota.cargaInicial = max(0, -d[melhorCliente]);
                primeiroClienteInserido = true;
            }

            visitado[melhorCliente] = true;
        }

        if (rota.caminho.size() > 2)
            rotas.push_back(rota);
    }

    return rotas;
}

int main() {
    ifstream in("instancia.txt");
    if (!in) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    int n, m, Q;
    in >> n >> m >> Q;

    vector<int> d(n+1);
    d[0] = 0;
    for (int i = 1; i <= n; i++) in >> d[i];

    vector<vector<int>> c(n+1, vector<int>(n+1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            in >> c[i][j];

    // --- Calcula déficit global ---
    int coletas = 0, entregas = 0;
    for (int i = 1; i <= n; i++) {
        if (d[i] > 0) coletas += d[i];
        else entregas += abs(d[i]);
    }
    int deficit = max(0, entregas - coletas);

    cout << "\n=== Informações da Instância ===" << endl; ;
    cout << "Total de coletas: " << coletas << endl;
    cout << "Total de entregas: " << entregas << endl;
    cout << "Déficit global (mínimo a sair do depósito): " << deficit << endl;
    cout << "Capacidade total disponível: " << m*Q << endl;
    if (deficit > m*Q) {
        cout << "⚠️ Instância inviável: déficit maior que capacidade total dos caminhões!" << endl;
    }

    // --- Executa Inserção Mais Barata VRP ---
    vector<Rota> rotas = insercaoMaisBarata(n, m, Q, d, c);

    int custoTotal = 0;
    for (auto &r : rotas) custoTotal += r.custo;

    cout << endl << "Inserção Mais Barata" << endl;
    cout << "Custo total: " << custoTotal << endl;
    cout << "Veículos usados: " << rotas.size() << endl;

    for (int k = 0; k < (int)rotas.size(); k++) {
        cout << "Rota " << k+1 << " (custo " << rotas[k].custo 
             << ", carga inicial " << rotas[k].cargaInicial << "): ";
        for (int i = 0; i < (int)rotas[k].caminho.size(); i++) {
            cout << rotas[k].caminho[i];
            if (i < (int)rotas[k].caminho.size()-1) cout << " -> ";
        }
        cout << endl;
    }

    return 0;
}
