#include "resultados.h"
#include "guloso.h"
#include "vnd.h"

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

    // solução inicial 
    Resultado solucao = guloso(n, m, Q, d, c);

    cout << "Solução inicial:" << endl;
    imprimirResultado(solucao);    

    //vnd

    int k = 1;
    int vizinhancas = 1; 

    while (k <= vizinhancas) {
        bool melhorou = false;

        switch (k) {
            case 1:
                //melhorou = aplicarSwap(solucao, d, c, Q); // intra-rota
                break;
        }

        if (melhorou) {
            cout << "Melhoria encontrada na vizinhança " << k << endl;
            k = 1; // volta para a primeira vizinhança
        } else {
            k++;   // vai para a próxima vizinhança
        }
    }

    cout << "\nSolução após VND:" << endl;
    imprimirResultado(solucao);

    gravarResultado("saida.txt", solucao);

    return 0;
}
