#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    int n = 7;     // número de clientes
    int m = 5;      // número de veículos
    int Q = 6;     // capacidade

    ofstream out("instancia_gerada.txt");
    if (!out) {
        cerr << "Erro ao criar o arquivo!\n";
        return 1;
    }

    // cabeçalho
    out << n << "\n";
    out << m << "\n";
    out << Q << "\n";

    for (int i = 1; i <= n; i++) {
    int valor = rand() % 10 + 1;   // de 1 a 10
    // sorteia sinal: 0 = negativo, 1 = positivo
    if (rand() % 2 == 0) valor = -valor;
    out << valor;
    if (i < n) out << " ";
    }
    out << "\n";

    // matriz de custos (n+1 x n+1)
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            int custo;
            if (i == j) custo = 0;
            else custo = rand() % 50 + 5; // custos entre 5 e 55
            out << custo;
            if (j < n) out << " ";
        }
        cout << "\n";
    }

    out.close();
    cout << "Arquivo 'instancia_gerada.txt' criado com sucesso!\n";
    return 0;
}
