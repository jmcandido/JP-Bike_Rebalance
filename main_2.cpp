#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <climits>
using namespace std;

struct Rota {
    vector<int> caminho;
    int custo;
};

/// Inserção Mais Barata
vector<Rota> cheapestInsertionVRP(int n, int m, int Q, const vector<int>& d, const vector<vector<int>>& c) {
    vector<bool> visitado(n+1, false);
    visitado[0] = true; // depósito

    vector<Rota> rotas;

    for (int k = 0; k < m; k++) {
        // verifica se já acabou
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) 
            if (!visitado[i]) {
                 todasVisitadas = false; break; 
                 }

        if (todasVisitadas) break;

        Rota rota;
        rota.caminho = {0, 0}; // começa e termina no depósito
        rota.custo = 0;

        int carga = 0;

        while (true) {
            int melhorCliente = -1, melhorPos = -1, melhorDelta = INT_MAX;

            for (int u = 1; u <= n; u++) {
                if (visitado[u]) continue;
                if (carga + d[u] > Q) continue; // inviável

                // tenta inserir em todas as posições da rota
                for (int pos = 0; pos < (int)rota.caminho.size()-1; pos++) {
                    int i = rota.caminho[pos];
                    int j = rota.caminho[pos+1];

                    int delta = c[i][u] + c[u][j] - c[i][j];
                    if (delta < melhorDelta) {
                        melhorDelta = delta;
                        melhorCliente = u;
                        melhorPos = pos+1;
                    }
                }
            }

            if (melhorCliente == -1) break; // não cabe mais ninguém

            // insere cliente escolhido
            rota.caminho.insert(rota.caminho.begin() + melhorPos, melhorCliente);
            rota.custo += melhorDelta;
            carga += d[melhorCliente];
            visitado[melhorCliente] = true;
        }

        rotas.push_back(rota);
    }

    return rotas;
}

int main() {
    ifstream in("instancia_gerada.txt");
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

    // --- Executa Inserção Mais Barata VRP ---
    vector<Rota> rotas = cheapestInsertionVRP(n, m, Q, d, c);

    int custoTotal = 0;
    for (auto &r : rotas) custoTotal += r.custo;

    cout << "\n=== Inserção Mais Barata (VRP) ===\n";
    cout << "Custo total: " << custoTotal << "\n";
    cout << "Veículos usados: " << rotas.size() << "\n";

    for (auto &r : rotas) {
        for (int i = 0; i < (int)r.caminho.size(); i++) {
            cout << r.caminho[i];
            if (i < (int)r.caminho.size()-1) cout << " ";
        }
        cout << "\n";
    }

    return 0;
}
