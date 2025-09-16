#include "resultados.h"
#include "vnd.h"
#include "grasp.h"

#include <chrono>
#include <random>
#include <limits>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " instancia.txt alpha iteracoes" << endl;
        return -1;
    }

    string instancia = argv[1];

    double alpha = stod(argv[2]);         
    int qtd_iteracoes = stoi(argv[3]);           

    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    if (!lerInstancia(instancia, n, m, Q, d, c)) {
        cerr << "Erro ao abrir instancia!\n";
        return -1;
    }

    auto inicio = chrono::high_resolution_clock::now();


    random_device rd;
    mt19937 rng(rd());

    Resultado res = GRASP(n, m, Q, d, c, alpha,qtd_iteracoes);

    imprimirResultado(res);
    gravaResultado("resultados/grasp", instancia, res); 

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    std::cout << "\nTempo total de execução: " << duracao.count() << " ms\n";

    return 0;
}
