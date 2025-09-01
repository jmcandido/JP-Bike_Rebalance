#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(NULL));

    int n = 7; 
    int m = 4;  // número de cliente;    // número de caminhões
    int Q = 10;   // capacidade de cada caminhão

    ofstream out("instancia.txt");

    // cabeçalho
    out << n << "\n";
    out << m << "\n";
    out << Q << "\n";

    // === gerar demandas no estilo do professor ===
    // valores inteiros entre -Q e +Q, exceto 0
    for (int i = 1; i <= n; i++) {
        int d = (rand() % Q) + 1; // 1..Q
        if (rand() % 2) d = -d;   // metade negativas
        out << d;
        if (i < n) out << " ";
    }
    out << "\n";

    // === gerar matriz de custos simétrica ===
    vector<vector<int>> c(n+1, vector<int>(n+1, 0));
    for (int i = 0; i <= n; i++) {
        for (int j = i+1; j <= n; j++) {
            int v = (rand() % 50) + 1; // custo entre 1 e 50
            c[i][j] = c[j][i] = v;
        }
    }

    // imprimir matriz
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            out << c[i][j];
            if (j < n) out << " ";
        }
        out << "\n";
    }

    out.close();
    cout << "Instância (versão professor) gerada em instancia.txt\n";
    return 0;
}
