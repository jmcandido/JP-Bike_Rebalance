#include "utilitarios.h"
#include "guloso.h"
#include "vnd.h"
#include <iostream>
using namespace std;

int main() {
    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    if (!lerInstancia("instancia.txt", n, m, Q, d, c)) {
        cerr << "Erro ao abrir instancia!\n";
        return 1;
    }

    // Solução inicial (guloso)
    Resultado sol = guloso(n, m, Q, d, c);

    imprimirResultado(sol);

    // Loop de melhoria com Swap
    bool melhorou = true;
    while (melhorou) {
        melhorou = aplicarSwap(sol, d, c, Q);
    }

    cout << "\n=== Solução após Swap ===" << endl;
    imprimirResultado(sol);

    gravarResultado("saida.txt",sol);
    
    return 0;
}
