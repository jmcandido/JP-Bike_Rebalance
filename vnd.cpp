#include "vnd.h"
#include "utilitarios.h"

using namespace std;

bool aplicarSwap(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c,int Q) {

     bool melhorou = false;

     for(Rota &rota: solucao.rotas){

            for(int i = 1; i < rota.caminho.size() - 2; i++){
                for(int j = i+1; j < rota.caminho.size() - 1; j++){

                    Rota nova_rota = rota;
                    
                    swap(nova_rota.caminho[i],nova_rota.caminho[j]);

                    nova_rota.custo = custoRota(nova_rota,c);

                    if(!validaRota(nova_rota, d, Q)){
                        continue;
                    }

                    if (nova_rota.custo < rota.custo){
                       solucao.custoFinal -= rota.custo;
                       solucao.custoFinal += nova_rota.custo;

                       rota = nova_rota;
                       melhorou = true;
                    }
                }

            }
        }

        return melhorou;

     }


// bool aplicarSwap(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c, int Q) {
//     bool melhorou = false;

//     cout << "=== Iniciando aplicarSwap ===" << endl;
//     cout << "Custo inicial da solucao: " << solucao.custoFinal << endl;

//     int rotaIndex = 0;
//     for (Rota &rota : solucao.rotas) {
//         cout << "\n>> Analisando rota " << rotaIndex << " (custo: " << rota.custo << ")\n";
//         cout << "Caminho atual: ";
//         for (int v : rota.caminho) cout << v << " ";
//         cout << endl;

//         for (int i = 1; i < (int)rota.caminho.size() - 2; i++) {
//             for (int j = i + 1; j < (int)rota.caminho.size() - 1; j++) {
//                 cout << " - Tentando swap posicoes " << i << " e " << j << endl;

//                 Rota nova_rota = rota;
//                 swap(nova_rota.caminho[i], nova_rota.caminho[j]);

//                 nova_rota.custo = custoRota(nova_rota, c);

//                 cout << "   Caminho apos swap: ";
//                 for (int v : nova_rota.caminho) cout << v << " ";
//                 cout << " | Custo: " << nova_rota.custo << endl;

//                 if (!validaRota(nova_rota, d, Q)) {
//                     cout << "   >> Rota invalida, ignorando." << endl;
//                     continue;
//                 }

//                 if (nova_rota.custo < rota.custo) {
//                     cout << "   >> Melhoria encontrada! " 
//                          << "Custo antigo: " << rota.custo 
//                          << ", Novo custo: " << nova_rota.custo << endl;

//                     solucao.custoFinal -= rota.custo;
//                     solucao.custoFinal += nova_rota.custo;

//                     rota = nova_rota;
//                     melhorou = true;

//                     cout << "   Atualizando solucao: novo custo final = " 
//                          << solucao.custoFinal << endl;
//                 }
//             }
//         }
//         rotaIndex++;
//     }

//     cout << "\n=== aplicarSwap finalizado ===" << endl;
//     cout << "Custo final da solucao: " << solucao.custoFinal << endl;

//     return melhorou;
// }
