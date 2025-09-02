#include "vnd.h"
#include "utilitarios.h"

using namespace std;

bool aplicarSwap(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c,int Q) {
    bool melhorou = false;

    for (int i = 0 ; i < solucao.rotas.size(); i++) {

        Rota &rota = solucao.rotas[i];
        int melhorCusto = rota.custo;
        int melhor_i = -1, melhor_j = -1;

        for (int i = 1; i < rota.caminho.size() - 2; i++) {
            for (int j = i + 1; j < rota.caminho.size() - 1; j++) {
                //cout << "Testando i = " << i << " j = " << j << endl << endl;
                swap(rota.caminho[i], rota.caminho[j]);

                if (validaRota(rota, d, Q)) {
                    int novoCusto = custoRota(rota, c);
                    //cout << "custo atual: " << novoCusto << endl;
                    //cout << "melhor custo:" << melhorCusto << endl;
                    if (novoCusto < melhorCusto) {
                        melhorCusto = novoCusto;
                        melhor_i = i;
                        melhor_j= j;
                    }
                }

                // rollback sempre
                swap(rota.caminho[i], rota.caminho[j]);
            }
        }

        if (melhor_i != -1) {
           cout << "Rota melhorada! "
         << "Troquei posicoes " << melhor_i << " (" << rota.caminho[melhor_i] << ")"
         << " <-> " << melhor_j << " (" << rota.caminho[melhor_j] << ")"
         << " | custo antigo=" << rota.custo
         << " novo custo=" << melhorCusto << endl;

        swap(rota.caminho[melhor_i], rota.caminho[melhor_j]);
        solucao.custoFinal += (melhorCusto - rota.custo);
        rota.custo = melhorCusto;
        melhorou = true;
        }
    }

    return melhorou;
}
