#include "vnd.h"
#include "resultados.h"
#include <algorithm>

bool aplicarSwap2(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c,int Q) {
    bool melhorou = false;
    // cout << "voltei" << endl;
    for (int r = 0 ; r < solucao.rotas.size(); r++) {

        Rota &rota = solucao.rotas[r];

        // cout << "Analisando rota " << r << " (caminho: ";
        // for (int v : rota.caminho) cout << v << " ";
        // cout << ") | custo atual=" << rota.custo << endl;

        int melhorCusto = rota.custo;
        int melhor_i = -1, melhor_j = -1;

        for (int i = 1; i < rota.caminho.size() - 2; i++) {
            for (int j = i + 1; j < rota.caminho.size() - 1; j++) {
                // cout << "Testando estacao  " << rota.caminho[i]  << " e " << rota.caminho[j]  << " (" << i << "," << j << ")" << endl << endl;
                swap(rota.caminho[i], rota.caminho[j]);
                if (validaRota(rota, d, Q)) {
                    int novoCusto = custoRota(rota, c);
                    // cout << "custo atual: " << novoCusto << endl;
                    // cout << "melhor custo:" << melhorCusto << endl;
                    if (novoCusto < melhorCusto) {
                        melhorCusto = novoCusto;
                        melhor_i = i;
                        melhor_j= j;
                    }
                }else{
                    // cout << "troca invalida" << endl;
                }

                swap(rota.caminho[i], rota.caminho[j]);
            }
        }

        if (melhor_i != -1) {
        //    cout << "Rota melhorada! "
        //  << "Troquei estacao " << rota.caminho[melhor_i] <<   " com " << rota.caminho[melhor_j]
        //  << " | custo antigo=" << rota.custo
        //  << " novo custo=" << melhorCusto << endl;

        swap(rota.caminho[melhor_i], rota.caminho[melhor_j]);
        solucao.custoFinal += (melhorCusto - rota.custo);
        rota.custo = melhorCusto;
        melhorou = true;
        }
    }

    return melhorou;
}

bool aplicarSwapInterRotas2(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c, int Q) {
    bool melhorou = false;

    // cout << "===== INICIO aplicarSwapInterRotas =====" << endl;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++) {
        for (int r2 = r1 + 1; r2 < solucao.rotas.size(); r2++) {

            Rota &rota1 = solucao.rotas[r1];
            Rota &rota2 = solucao.rotas[r2];


            int melhorCusto = rota1.custo + rota2.custo;
            int melhor_i = -1, melhor_j = -1;

            for (int i = 1; i < rota1.caminho.size() - 1; i++) {
                for (int j = 1; j < rota2.caminho.size() - 1; j++) {
                    
                    swap(rota1.caminho[i], rota2.caminho[j]);

                    if (validaRota(rota1, d, Q) && validaRota(rota2, d, Q)) {
                        int novoCusto1 = custoRota(rota1, c);
                        int novoCusto2 = custoRota(rota2, c);
                        int novoCustoTotal = novoCusto1 + novoCusto2;

                        // cout << "   Rotas válidas. Custo total novo=" << novoCustoTotal
                        //      << " | Melhor custo atual=" << melhorCusto << endl;

                        if (novoCustoTotal < melhorCusto) {
                            // cout << "   Encontrada melhoria." << endl;
                            melhorCusto = novoCustoTotal;
                            melhor_i = i;
                            melhor_j = j;
                        }
                    } else {
                        // cout << "   Troca inválida (restrição violada)" << endl;
                    }

                    swap(rota1.caminho[i], rota2.caminho[j]); // desfaz a troca
                }
            }

            if (melhor_i != -1) {
                // cout << "\n>>> Aplicando melhoria final: "
                //      << "rota1[" << melhor_i << "]=" << rota1.caminho[melhor_i]
                //      << " <-> rota2[" << melhor_j << "]=" << rota2.caminho[melhor_j] << endl;

                swap(rota1.caminho[melhor_i], rota2.caminho[melhor_j]);

                int novoCusto1 = custoRota(rota1, c);
                int novoCusto2 = custoRota(rota2, c);

                // cout << "   Novo custo Rota1=" << novoCusto1
                //      << " | Novo custo Rota2=" << novoCusto2 << endl;
                // cout << "   Custo final antes=" << solucao.custoFinal;

                solucao.custoFinal += (novoCusto1 + novoCusto2) - (rota1.custo + rota2.custo);

                // cout << " | Depois=" << solucao.custoFinal << endl;

                rota1.custo = novoCusto1;
                rota2.custo = novoCusto2;

                melhorou = true;
            } else {
                // cout << "Nenhuma melhoria encontrada entre rotas " << r1 << " e " << r2 << endl;
            }
        }
    }

    // cout << "FIM aplicarSwapInterRotas" << endl;
    return melhorou;
}

bool aplicarSwap(Resultado& solucao,const vector<int>& d, const vector<vector<int>>& c,int Q) {
    bool melhorou = false;

    for (int r = 0 ; r < solucao.rotas.size(); r++) {


    Rota &rota = solucao.rotas[r];

    // std::cout << "Analisando rota " << r << " (caminho: ";
    // std::cout << ") | custo atual=" << rota.custo << "\n";

    int custoOriginal = rota.custo;
    int melhorCusto = custoOriginal;
    int melhor_i = -1, melhor_j = -1;

        for (int i = 1; i < rota.caminho.size() - 2; i++) {
            for (int j = i + 1; j < rota.caminho.size() - 1; j++) {

                int A = rota.caminho[i-1];
                int B = rota.caminho[i];
                int E = rota.caminho[j];
                int F = rota.caminho[j+1];

                int delta;

                // cout << "[i="<<i<<", j="<<j<<"]" << endl;

                if (j == i + 1) {
                    // vizinhos: ... A - B - E - F ...
                    // remove: (A,B),(B,E),(E,F) e adiciona (A,E),(E,B),(B,F)
                    delta = (c[A][E] + c[E][B] + c[B][F])
                          - (c[A][B] + c[B][E] + c[E][F]);

                } else {
                    // geral: ... A - B - C ... D - E - F ...
                    int C = rota.caminho[i+1];
                    int D = rota.caminho[j-1];
                    // remove: (A,B),(B,C),(D,E),(E,F) e adiciona (A,E),(E,C),(D,B),(B,F)
                    delta = (c[A][E] + c[E][C] + c[D][B] + c[B][F])
                          - (c[A][B] + c[B][C] + c[D][E] + c[E][F]);
    
                }

                // Se não melhora custo, nem valida
                if (delta >= 0) {
                    // std::cout << "  -> delta >= 0 (sem melhoria). Pulando validacao.\n";
                    continue;
                }

                // Validação: faz swap temporário só agora
                swap(rota.caminho[i], rota.caminho[j]);

                if (validaRota(rota, d, Q)){
                    int novoCusto = custoOriginal + delta;

                    if (novoCusto < melhorCusto) {
                    melhorCusto = novoCusto;
                    melhor_i = i;
                    melhor_j = j;
                }
            }

                swap(rota.caminho[i], rota.caminho[j]); // desfaz swap
            }
        }

        if (melhor_i != -1) {
            // std::cout << "Rota melhorada! Troquei estacao "
            //           << rota.caminho[melhor_i] << " (pos "<<melhor_i<<") com "
            //           << rota.caminho[melhor_j] << " (pos "<<melhor_j<<")"
            //           << " | custo antigo=" << rota.custo
            //           << " novo custo=" << melhorCusto << "\n";

            std::swap(rota.caminho[melhor_i], rota.caminho[melhor_j]);
            solucao.custoFinal += (melhorCusto - rota.custo);
            rota.custo = melhorCusto;
            melhorou = true;
        } else {
            // std::cout << "Nenhuma melhoria aplicada na rota " << r << ".\n";
        }

    }
            return melhorou;

}

bool aplicarSwapInterRotas(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c,int Q) {
    bool melhorou = false;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++) {
        for (int r2 = r1 + 1; r2 < solucao.rotas.size(); r2++) {
            Rota &rota1 = solucao.rotas[r1];
            Rota &rota2 = solucao.rotas[r2];

            // cout << "\n--- Comparando rotas " << r1 << " e " << r2
            //      << " (custos atuais: r1=" << rota1.custo
            //      << ", r2=" << rota2.custo << ") ---\n";

            int melhorDelta = 0;
            int melhor_i = -1, melhor_j = -1;
            int melhorDelta1 = 0, melhorDelta2 = 0;

            for (int i = 1; i < (int)rota1.caminho.size() - 1; i++) {
                for (int j = 1; j < (int)rota2.caminho.size() - 1; j++) {

                    int A = rota1.caminho[i-1], B = rota1.caminho[i], C = rota1.caminho[i+1];
                    int D = rota2.caminho[j-1], E = rota2.caminho[j], F = rota2.caminho[j+1];

                    int delta1 = (c[A][E] + c[E][C]) - (c[A][B] + c[B][C]);
                    int delta2 = (c[D][B] + c[B][F]) - (c[D][E] + c[E][F]);

                    int delta = delta1 + delta2;

                    //cout << "[i=" << i << ", j=" << j << "] delta1=" << delta1 << " delta2=" << delta2 << " total delta=" << delta << "\n";

                    if (delta >= 0) {
                        //cout << " Não melhorei aqui, bora de next" << endl;
                        continue;
                    }

                    // valida temporário
                    swap(rota1.caminho[i], rota2.caminho[j]);
                    //bool rotasValidas = validaRota(rota1, d, Q) && validaRota(rota2, d, Q);

                    if(validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                        if (delta < melhorDelta) {
                        melhorDelta = delta;
                        melhorDelta1 = delta1;
                        melhorDelta2 = delta2;
                        melhor_i = i;
                        melhor_j = j;
                     }   

                    }
                    swap(rota1.caminho[i], rota2.caminho[j]);
                }
            }

            if (melhor_i != -1) {
                //cout << ">> Aplicando troca entre rotas " << r1 << " e " << r2c << " | delta=" << melhorDelta << "\n";

                swap(rota1.caminho[melhor_i], rota2.caminho[melhor_j]);
                rota1.custo += melhorDelta1;
                rota2.custo += melhorDelta2;
                solucao.custoFinal += melhorDelta;

                melhorou = true;
            } else {
                //cout << "Nenhuma melhoria encontrada entre rotas " << r1 << " e " << r2 << ".\n";
            }
        }
    }

    return melhorou;
}


// Move 1 cliente de uma rota para outra (somente INTER-rotas) – best move, sem cópias
// bool aplicarRelocate(Resultado& solucao,
//                      const vector<int>& d,
//                      const vector<vector<int>>& c,
//                      int Q)
// {
//     int best_r1 = -1, best_i = -1;     // origem e índice do cliente
//     int best_r2 = -1, best_j = -1;     // destino e aresta (inserir entre j e j+1)
//     int melhorDelta = 0;               // delta < 0 é melhoria

//     // varre pares de rotas
//     for (int r1 = 0; r1 < (int)solucao.rotas.size(); ++r1) {
//         for (int r2 = 0; r2 < (int)solucao.rotas.size(); ++r2) {
//             if (r1 == r2) continue; // aqui só inter-rotas

//             Rota &rota1 = solucao.rotas[r1];
//             Rota &rota2 = solucao.rotas[r2];

//             const int L1 = (int)rota1.caminho.size();
//             const int L2 = (int)rota2.caminho.size();
//             if (L1 <= 3) continue;  // rota1 precisa ter pelo menos 1 cliente

//             // percorre cada cliente B de rota1 (sem depósitos)
//             for (int i = 1; i < L1 - 1; ++i) {
//                 int A = rota1.caminho[i - 1];
//                 int B = rota1.caminho[i];
//                 int C = rota1.caminho[i + 1];

//                 // delta de REMOVER B de rota1: (A-B-C) -> (A-C)
//                 int deltaRemove = c[A][C] - (c[A][B] + c[B][C]);

//                 // tenta inserir B em cada aresta (X-Y) de rota2
//                 for (int j = 0; j < L2 - 1; ++j) {
//                     int X = rota2.caminho[j];
//                     int Y = rota2.caminho[j + 1];

//                     // delta de INSERIR B entre (X-Y): (X-Y) -> (X-B-Y)
//                     int deltaInsert = (c[X][B] + c[B][Y]) - c[X][Y];
//                     int delta = deltaRemove + deltaInsert;
//                     if (delta >= 0) continue; // só consideramos melhoria

//                     // ---- APLICA TEMPORÁRIO (sem cópias) ----
//                     // guardar B e executar move
//                     int cliente = B;
//                     rota1.caminho.erase(rota1.caminho.begin() + i);
//                     rota2.caminho.insert(rota2.caminho.begin() + (j + 1), cliente);

//                     bool ok = validaRota(rota1, d, Q) && validaRota(rota2, d, Q);

//                     // ---- ROLLBACK imediato ----
//                     rota2.caminho.erase(rota2.caminho.begin() + (j + 1));
//                     rota1.caminho.insert(rota1.caminho.begin() + i, cliente);

//                     if (!ok) continue;

//                     if (delta < melhorDelta) {
//                         melhorDelta = delta;
//                         best_r1 = r1; best_i = i;
//                         best_r2 = r2; best_j = j;
//                     }
//                 }
//             }
//         }
//     }

//     // aplica o melhor movimento encontrado (uma única vez)
//     if (best_r1 != -1) {
//         Rota &rOrig = solucao.rotas[best_r1];
//         Rota &rDest = solucao.rotas[best_r2];

//         int A = rOrig.caminho[best_i - 1];
//         int B = rOrig.caminho[best_i];
//         int C = rOrig.caminho[best_i + 1];

//         int X = rDest.caminho[best_j];
//         int Y = rDest.caminho[best_j + 1];

//         int deltaRemove = c[A][C] - (c[A][B] + c[B][C]);
//         int deltaInsert = (c[X][B] + c[B][Y]) - c[X][Y];
//         int delta = deltaRemove + deltaInsert; // < 0

//         // aplica definitivo
//         rOrig.caminho.erase(rOrig.caminho.begin() + best_i);
//         rDest.caminho.insert(rDest.caminho.begin() + (best_j + 1), B);

//         // atualiza custos incrementalmente (sem recomputar a rota inteira)
//         rOrig.custo += deltaRemove;
//         rDest.custo += deltaInsert;
//         solucao.custoFinal += delta;

//         return true;
//     }

//     return false; // nenhuma melhoria
// }


bool aplicarRelocate(Resultado& solucao,const vector<int>& d, const vector<vector<int>>& c, int Q){
    int melhor_r1 = -1,melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;
    int melhorValor = 0;

    for (int r1 = 0; r1 < (int)solucao.rotas.size(); r1++) {
        for (int r2 = 0; r2 < (int)solucao.rotas.size(); r2++) {
            if (r1 == r2) continue;

            Rota &rota1 = solucao.rotas[r1];
            Rota &rota2 = solucao.rotas[r2];

            int L1 = rota1.caminho.size();
            int L2 = rota2.caminho.size();
                if (L1 <= 3) 
                 continue; 

            for (int i = 1; i < L1 - 1; i++) {
                int A = rota1.caminho[i - 1];
                int B = rota1.caminho[i];
                int C = rota1.caminho[i + 1];

                int valor_removido = c[A][C] - (c[A][B] + c[B][C]);
                std::cout << "\n[Relocate] Testando cliente " << B
                          << " da rota " << r1 << " (pos=" << i << ")\n";
                std::cout << "  deltaRemove = " << valor_removido << "\n";

                for (int j = 0; j < L2 - 1; j++) {
                    int X = rota2.caminho[j];
                    int Y = rota2.caminho[j + 1];

                    int valor_inserido = (c[X][B] + c[B][Y]) - c[X][Y];
                    int novo_valor = valor_removido + valor_inserido;

                    std::cout << "    Inserindo entre (" << X << "," << Y
                              << ") da rota " << r2
                              << " -> deltaInsert=" << valor_inserido
                              << " deltaTotal=" << novo_valor;

                    if (novo_valor >= 0) {
                        std::cout << " [descartado: não melhora]\n";
                        continue;
                    }

                    // aplica temporário
                    int cliente = B;
                    rota1.caminho.erase(rota1.caminho.begin() + i);
                    rota2.caminho.insert(rota2.caminho.begin() + (j + 1), cliente);

                    bool ok = validaRota(rota1, d, Q) && validaRota(rota2, d, Q);

                   
                    // rollback
                    rota2.caminho.erase(rota2.caminho.begin() + (j + 1));
                    rota1.caminho.insert(rota1.caminho.begin() + i, cliente);

                    if (!ok) {
                        std::cout << " [inválido]\n";
                        continue;
                    }

                    std::cout << " [VÁLIDO]";
                    if (novo_valor < melhorValor) {
                        std::cout << " **novo melhor movimento**";
                        melhorValor = novo_valor;
                        melhor_r1 = r1; melhor_i = i;
                        melhor_r2 = r2; melhor_j = j;
                    }
                    std::cout << "\n";
                }
            }
        }
    }

    if (melhor_r1 != -1) {
        Rota &rOrig = solucao.rotas[melhor_r1];
        Rota &rDest = solucao.rotas[melhor_r2];

        int A = rOrig.caminho[melhor_i - 1];
        int B = rOrig.caminho[melhor_i];
        int C = rOrig.caminho[melhor_i + 1];

        int X = rDest.caminho[melhor_j];
        int Y = rDest.caminho[melhor_j + 1];

        int valor_removido = c[A][C] - (c[A][B] + c[B][C]);
        int valor_inserido = (c[X][B] + c[B][Y]) - c[X][Y];
        int novo_valor = valor_removido + valor_inserido;

        std::cout << "\n[Relocate] >>> Aplicando movimento final: cliente "
                  << B << " da rota " << melhor_i << " -> rota " << melhor_j
                  << " entre (" << X << "," << Y << ")"
                  << " | deltaFinal=" << novo_valor << "\n";

        rOrig.caminho.erase(rOrig.caminho.begin() + melhor_i);
        rDest.caminho.insert(rDest.caminho.begin() + (melhor_j + 1), B);

        rOrig.custo += valor_removido;
        rDest.custo += valor_inserido;
        solucao.custoFinal += novo_valor;

        return true;
    }

    std::cout << "\n[Relocate] Nenhuma melhoria encontrada.\n";
    return false;
}
