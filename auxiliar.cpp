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
    cout << "Digite uma rota (sequência de vértices terminada em -1):\n";

    Rota rota;
    int cliente;
    while (cin >> cliente && cliente != -1) {
        rota.caminho.push_back(cliente);
    }

    // calcula custo
    rota.custo = custoRota(rota, c);

    // valida factibilidade
    bool ok = validaRota(rota, d, Q);

    // saída
    cout << "\nRota testada: ";
    for (int v : rota.caminho) cout << v << " ";
    cout << "\n";

    cout << "Custo da rota = " << rota.custo << "\n";
    cout << "Rota " << (ok ? "✅ viável" : "❌ inviável") << "\n";

    return 0;
}
