#include <iostream>
#include <vector>
#include "utilitarios.h"
using namespace std;

int main() {
    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    // Carrega instância
    if (!lerInstancia("instancia.txt", n, m, Q, d, c)) {
        cerr << "Erro ao abrir instancia!\n";
        return 1;
    }

    cout << "Capacidade Q = " << Q << "\n";
    cout << "Digite as rotas manualmente.\n";
    cout << "Formato: comece sempre com 0 (depósito).\n";
    cout << "Quando voltar ao depósito (0), a rota é fechada.\n";
    cout << "Digite -1 quando não houver mais rotas.\n\n";

    vector<Rota> rotas;
    while (true) {
        Rota rota;
        int cliente;
        cout << "Rota " << rotas.size() << ": ";
        while (cin >> cliente) {
            if (cliente == -1) goto fim;   // termina tudo
            rota.caminho.push_back(cliente);
            if (cliente == 0 && rota.caminho.size() > 1) break; // rota fechada
        }
        if (!rota.caminho.empty()) {
            rota.custo = custoRota(rota, c);
            rotas.push_back(rota);
        }
    }

fim:
    int custoTotal = 0;
    cout << "\n=== Rotas informadas ===\n";
    for (int i = 0; i < (int)rotas.size(); i++) {
        Rota& r = rotas[i];
        bool ok = validaRota(r, d, Q);

        cout << "Rota " << i << ": ";
        for (int v : r.caminho) cout << v << " ";
        cout << "\n";
        cout << "  custo = " << r.custo;
        cout << " | " << (ok ? "✅ viável" : "❌ inviável") << "\n";

        custoTotal += r.custo;
    }

    cout << "\nCusto total da solução = " << custoTotal << "\n";

    return 0;
}
