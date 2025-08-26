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
    ifstream in("instancia1000.txt");
    if (!in) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    int n, m, Q;
    in >> n >> m >> Q;
    vector<int> d(n + 1);
    d[0] = 0; // depósito
    for (int i = 1; i <= n; i++) in >> d[i];

    // matriz de custos (n+1)x(n+1) incluindo depósito 
    vector<vector<int>> c(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            in >> c[i][j];

    int coletas = 0, entregas = 0;
    for (int i = 1; i <= n; i++) {
        if (d[i] > 0) coletas += d[i];
        else entregas += abs(d[i]);
    }
    int deficit = max(0, entregas - coletas);

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
                if (d[i] < 0 && carga >= -d[i]) candidatos.push_back(i);
                else if (d[i] > 0 && carga + d[i] <= Q) candidatos.push_back(i);
            }

            if (candidatos.empty()) {
                // checa se ainda restam clientes
                bool aindaTem = false;
                for (int i = 1; i <= n; i++) if (!visitado[i]) { aindaTem = true; break; }

                if (!aindaTem) {
                    // fecha rota normal
                    rota.caminho.push_back(0);
                    rota.custo += c[atual][0];
                    cout << "Retornando ao depósito (custo +" << c[atual][0] 
                         << ", total=" << rota.custo << ")\n";
                    break;
                }

                // checa se algum cliente seria viável depois do pit stop
                bool alguemViavel = false;
                for (int i = 1; i <= n; i++) {
                    if (visitado[i]) continue;
                    if (d[i] < 0 && Q >= -d[i]) alguemViavel = true;
                    if (d[i] > 0 && d[i] <= Q) alguemViavel = true;
                }

                if (!alguemViavel) {
                    // nenhum cliente viável → encerra rota
                    rota.caminho.push_back(0);
                    rota.custo += c[atual][0];
                    cout << "Encerrando rota: clientes restantes inviáveis\n";
                    break;
                }

                // caso contrário, pit stop
                rota.caminho.push_back(0);
                rota.custo += c[atual][0];
                cout << "Pit stop no depósito (custo +" << c[atual][0] 
                     << ", total=" << rota.custo << ")\n";
                atual = 0;
                carga = 0; // ou Q se quiser sair cheio
                continue;
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

            cout << "Atual=" << atual 
                 << " | Indo para " << melhor 
                 << " (custo +" << melhorCusto << ")"
                 << " | Carga antes=" << carga;

            rota.caminho.push_back(melhor);
            rota.custo += c[atual][melhor];
            carga += d[melhor];
            visitado[melhor] = true;
            atual = melhor;

            cout << " | Carga depois=" << carga 
                 << " | Custo total=" << rota.custo << "\n";
        }

        rotas.push_back(rota);
    }

    int custoTotal = 0;
    for (auto &r : rotas) custoTotal += r.custo;

    cout << "\n=== RESULTADO FINAL ===\n";
    cout << custoTotal << "\n";
    cout << rotas.size() << "\n";
    for (auto &r : rotas) {
        for (int idx = 0; idx < (int)r.caminho.size(); idx++) {
            cout << r.caminho[idx] << (idx+1 < (int)r.caminho.size() ? ' ' : '\n');
        }
    }

    return 0;
}
