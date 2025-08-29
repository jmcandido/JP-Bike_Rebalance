#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "utilitarios.h"
#include "guloso.h"

using namespace std;


int main() {
    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    if (!lerInstancia("instancia.txt", n, m, Q, d, c)) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    Resultado res = guloso(n, m, Q, d, c);
    imprimirResultado(res);

    return 0;
}
