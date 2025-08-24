#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

struct Rota {
    vector<int> caminho;
    int custo;
};

int main() {
    ifstream in("instancia.txt");
    if (!in) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    int n, m, Q;
    in >> n >> m >> Q;
    vector<int> d(n + 1);
    d[0] = 0; // depósito
    for (int i = 1; i <= n; i++) {
        in >> d[i]; // lê estações 1..n
    }

    // --- matriz de custos: (n+1)x(n+1) incluindo depósito ---
    vector<vector<int>> c(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            in >> c[i][j];
        }
    }

    // --- calcula déficit (opcional: pode começar com carga 0 se preferir) ---
    int coletas = 0, entregas = 0;
    for (int i = 1; i <= n; i++) {
        if (d[i] > 0) coletas += d[i];
        else entregas += abs(d[i]);
    }
    int deficit = max(0, entregas - coletas);

    // --- visitados (1..n) ---
    vector<bool> visitado(n + 1, false);

    // --- rotas ---
    vector<Rota> rotas;

    for (int k = 0; k < m; k++) {
        // critério de parada: se já cobriu todas as estações
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) { todasVisitadas = false; break; }
        }
        if (todasVisitadas) break;

        // inicia rota do veículo k
        Rota rota;
        rota.custo = 0;
        rota.caminho.push_back(0);

        // define carga inicial
        int carga = 0;
        if (deficit > 0) {
            carga = min(Q, deficit);
            deficit -= carga;
        }

        int atual = 0;

        cout << "\n--- Veículo " << (k+1) << " ---\n";
        cout << "Carga inicial: " << carga << "\n";

        while (true) {
            // candidatos viáveis
            vector<int> candidatos;
            for (int i = 1; i <= n; i++) {
                if (visitado[i]) continue;
                if (d[i] < 0 && carga >= abs(d[i])) candidatos.push_back(i);      // entrega
                else if (d[i] > 0 && carga + d[i] <= Q) candidatos.push_back(i);  // coleta
            }

            if (candidatos.empty()) {
                // fecha rota
                rota.caminho.push_back(0);
                rota.custo += c[atual][0];
                cout << "Retornando ao depósito (custo +" << c[atual][0] 
                     << ", total=" << rota.custo << ")\n";
                break;
            }

            // escolhe mais próximo
            int melhor = candidatos[0];
            int melhorCusto = c[atual][melhor];
            for (int j : candidatos) {
                if (c[atual][j] < melhorCusto) {
                    melhor = j;
                    melhorCusto = c[atual][j];
                }
            }

            // debug passo a passo
            cout << "Atual=" << atual 
                 << " | Indo para " << melhor 
                 << " (custo +" << melhorCusto << ")"
                 << " | Carga antes=" << carga;

            // atualiza estado
            rota.caminho.push_back(melhor);
            rota.custo += c[atual][melhor];
            carga += d[melhor];      // coleta(+) aumenta, entrega(-) diminui
            visitado[melhor] = true;
            atual = melhor;

            cout << " | Carga depois=" << carga 
                 << " | Custo total=" << rota.custo << "\n";
        }

        rotas.push_back(rota);
    }

    // calcula custo total
    int custoTotal = 0;
    for (auto &r : rotas) custoTotal += r.custo;

    // --- Saída final ---
    cout << "\n=== RESULTADO FINAL ===\n";
    cout << custoTotal << "\n";       // 1) valor da solução
    cout << rotas.size() << "\n";     // 2) número de veículos usados

    // 3) rotas de cada veículo
    for (auto &r : rotas) {
        for (int idx = 0; idx < (int)r.caminho.size(); idx++) {
            cout << r.caminho[idx];
            if (idx < (int)r.caminho.size()-1) cout << " ";
        }
        cout << "\n";
    }

    return 0;
}
