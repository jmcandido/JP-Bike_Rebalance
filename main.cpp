#include "resultados.h"
#include "guloso.h"
#include "vnd.h"
#include "grasp.h"
#include <chrono>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        return -1;
    }

    string instancia = argv[1];

    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;


    // leitura da instância
    if (!lerInstancia(instancia, n, m, Q, d, c)) {
        cerr << "Erro ao abrir instancia!\n";
        return -1;
    }

     auto inicio = std::chrono::high_resolution_clock::now();


    // solução inicial 
    Resultado solucao = guloso(n, m, Q, d, c);

    cout << "Solução inicial:" << endl;
    gravaResultado("resultados/guloso",instancia,solucao);
    imprimirResultado(solucao);    

    VND(solucao,d,c,Q);

    cout << "\nSolução após VND:" << endl;
    imprimirResultado(solucao);
    gravaResultado("resultados/vnd",instancia, solucao);

    auto fim = std::chrono::high_resolution_clock::now();

    // calcula duração em ms
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
    cout << "\nTempo total de execução: " << duracao.count() << " ms" << endl;

    return 0;
}
