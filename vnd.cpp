#include "vnd.h"
#include "utilitarios.h"

using namespace std;

bool aplicarSwap(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c,int Q) {

     bool melhorou = false;

     for(Rota &rota: solucao.rotas){

            for(int i = 1; i < rota.caminho.size() - 2; i++){
                for(int j = i+1; j < rota.caminho.size() - 1; j++){
                    
                    swap(rota.caminho[i],rota.caminho[j]);

                    if(!validaRota(rota, d, Q)){
                        swap(rota.caminho[i], rota.caminho[j]);
                        continue;
                    }
                      
                    int novo_custo = custoRota(rota,c);

                    if (novo_custo >= rota.custo){
                        swap(rota.caminho[i],rota.caminho[j]);
                        continue;
                    }

                       solucao.custoFinal -= rota.custo;
                       solucao.custoFinal += novo_custo;
                       rota.custo = novo_custo;
                       melhorou = true;

                    }
                }
            }
                    return melhorou;
        }
