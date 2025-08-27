#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

struct Rota {
    vector<int> caminho;
    int custo;
};

// === Função de debug ===
void debugStep(int veiculo, int atual, int proximo, int custoAresta,
               int custoAcumulado, int cargaAntes, int cargaDepois,
               const string &motivo) {
    cerr << "[DEBUG] Veículo " << veiculo
         << " | Atual=" << atual
         << " -> Próx=" << proximo
         << " | Custo +" << custoAresta
         << " | Total=" << custoAcumulado
         << " | Carga antes=" << cargaAntes
         << " | Carga depois=" << cargaDepois
         << " | Motivo=" << motivo
         << "\n";
}

int main() {
    ifstream in("instancia.txt");
    if (!in) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    int n, m, Q;
    in >> n >> m >> Q;

    // demandas
    vector<int> d(n + 1);
    d[0] = 0; // depósito
    for (int i = 1; i <= n; i++) in >> d[i];

    // matriz de custos (n+1)x(n+1)
    vector<vector<int>> c(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            in >> c[i][j];

    vector<bool> visitado(n + 1, false);
    vector<Rota> rotas;

    for (int k = 0; k < m; k++) {
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) { todasVisitadas = false; break; }
        }
        if (todasVisitadas) break;

        Rota rota;
        rota.custo = 0;
        rota.caminho.push_back(0);

        int faltamEntregas = 0, faltamColetas = 0;
        for (int i = 1; i <= n; i++) {
            if (visitado[i]) continue;
            if (d[i] < 0) faltamEntregas += -d[i];
            else if (d[i] > 0) faltamColetas += d[i];
        }

        int carga = 0;
        if (faltamEntregas > 0) {
            carga = min(Q, faltamEntregas); // sai carregado
        } else {
            carga = 0; // sai vazio
        }

        int atual = 0;

        while (true) {
            // candidatos viáveis
            vector<int> candidatos;
            for (int i = 1; i <= n; i++) {
                if (visitado[i]) continue;
                if (d[i] < 0 && carga >= -d[i]) candidatos.push_back(i);
                else if (d[i] > 0 && carga + d[i] <= Q) candidatos.push_back(i);
            }

            if (candidatos.empty()) {
                // volta ao depósito
                rota.caminho.push_back(0);
                rota.custo += c[atual][0];
                cerr << "[DEBUG] Veículo " << (k+1)
                     << " | Voltando ao depósito do cliente " << atual
                     << " | Custo +" << c[atual][0]
                     << " | Total=" << rota.custo
                     << " | Carga final=" << carga << "\n";
                break;
            }

            // escolhe cliente mais próximo
            int melhor = candidatos[0];
            int melhorCusto = c[atual][melhor];
            for (int j : candidatos) {
                if (c[atual][j] < melhorCusto) {
                    melhor = j;
                    melhorCusto = c[atual][j];
                }
            }

            int cargaAntes = carga;
            carga += d[melhor];

            string motivo;
            if (d[melhor] < 0)
                motivo = "Entrega viável mais próxima";
            else
                motivo = "Coleta viável mais próxima";

            debugStep(k+1, atual, melhor, melhorCusto,
                      rota.custo + melhorCusto, cargaAntes, carga, motivo);

            rota.caminho.push_back(melhor);
            rota.custo += melhorCusto;
            visitado[melhor] = true;
            atual = melhor;
        }

        rotas.push_back(rota);
    }

    // checagem final: todos clientes foram atendidos
    for (int i = 1; i <= n; i++) {
        if (!visitado[i]) {
            cerr << "Erro: cliente " << i << " não foi atendido!\n";
            return 1;
        }
    }

    int custoTotal = 0;
    for (auto &r : rotas) custoTotal += r.custo;

    // saída final
    cout << custoTotal << "\n";
    cout << rotas.size() << "\n";
    for (auto &r : rotas) {
        for (int idx = 0; idx < (int)r.caminho.size(); idx++) {
            cout << r.caminho[idx] << (idx+1 < (int)r.caminho.size() ? ' ' : '\n');
        }
    }

    return 0;
}
