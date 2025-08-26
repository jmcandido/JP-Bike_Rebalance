#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(NULL));

    int n = 50;      // número de clientes
    int m = 4;        // número de caminhões
    int Q = 10;       // capacidade de cada caminhão

    ofstream out("instancia1000.txt");

    out << n << "\n";
    out << m << "\n";
    out << Q << "\n";

    // demandas: mistura de coletas (+) e entregas (-)
    for (int i = 1; i <= n; i++) {
        int d = (rand() % 10) + 1; // 1..10
        if (rand() % 2) d = -d;    // metade entrega, metade coleta
        out << d;
        if (i < n) out << " ";
    }
    out << "\n";

    // matriz de custos simétrica (0..50 aleatório)
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            int c;
            if (i == j) c = 0;
            else c = (rand() % 50) + 1;
            out << c;
            if (j < n) out << " ";
        }
        out << "\n";
    }

    out.close();
    cout << "Instância gerada em instancia1000.txt\n";
    return 0;
}
