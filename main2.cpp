#include "resultados.h"
#include "guloso.h"
#include "vnd.h"
#include "grasp.h"

#include <chrono>
#include <random>
#include <limits>
#include <iostream>

int main(int argc, char* argv[]) {
    // Uso: ./programa instancia.txt [alpha] [iteracoes]
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " instancia.txt [alpha=0.35] [iteracoes=30]\n";
        return -1;
    }

    string instancia = argv[1];

    double alpha = stod(argv[2]);         
    int qtd_iteracoes = stoi(argv[3]);           

    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    if (!lerInstancia(instancia, n, m, Q, d, c)) {
        std::cerr << "Erro ao abrir instancia!\n";
        return -1;
    }

    auto inicio = chrono::high_resolution_clock::now();

    random_device rd;
    mt19937 rng(rd());

    // Melhor solução global
    Resultado melhor;
    melhor.custoFinal = numeric_limits<int>::max();

    for (int i = 0; i < qtd_iteracoes; i++) {
        // Construção GRASP
        Resultado sol = guloso_GRASP(n, m, Q, d, c, alpha, rng);
        imprimirResultado(sol);
        
        VND(sol, d, c, Q);

        //Atualiza melhor solução
        if (!sol.rotas.empty() && sol.custoFinal < melhor.custoFinal) {
            melhor = sol;
        }
    }

    std::cout << "Solução inicial (melhor entre " << qtd_iteracoes << " iterações GRASP+VND):\n";
    imprimirResultado(melhor);
    gravaResultado("resultados/vnd", instancia, melhor);   // salva a melhor pós-VND

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    std::cout << "\nTempo total de execução: " << duracao.count() << " ms\n";

    return 0;
}
