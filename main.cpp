#include "resultados.h"
#include "guloso.h"
#include "vnd.h"
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
    //imprimirResultado(solucao);    

    //vnd

    int k = 1;
    int vizinhancas = 2; 

    while (k <= vizinhancas) {
        bool melhorou = false;

        cout << "k = " << k << endl;

        switch (k) {
            case 1:
                melhorou = aplicarSwap(solucao, d, c, Q); 
                break;
            case 2:
                melhorou = aplicarSwapInterRotas(solucao, d, c, Q);
                break;
        }

        if (melhorou) {
            //cout << "Melhoria encontrada na vizinhança " << k << endl;
            k = 1; // volta para a primeira vizinhança
        } else {
            k++;   // vai para a próxima vizinhança 17440 ms   1241
        }
    }

    cout << "\nSolução após VND:" << endl;
    imprimirResultado(solucao);
    gravaResultado("resultados/vnd",instancia, solucao);

    auto fim = std::chrono::high_resolution_clock::now();

    // calcula duração em ms
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
    cout << "\nTempo total de execução: " << duracao.count() << " ms" << endl;

    return 0;
}
