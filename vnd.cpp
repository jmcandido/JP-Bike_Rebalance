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

bool aplicarRelocateInter(Resultado& solucao,const vector<int>& d, const vector<vector<int>>& c, int Q){
    int melhor_r1 = -1,melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;
    int melhorValor = 0;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++) {
        for (int r2 = 0; r2 < solucao.rotas.size(); r2++) {
            if (r1 == r2)
                 continue;
           
            Rota &rota1 = solucao.rotas[r1];
            Rota &rota2 = solucao.rotas[r2];

            // int L1 = rota1.caminho.size();
            // int L2 = rota2.caminho.size();

                if (rota1.caminho.size() <= 3) 
                 continue; 

            for (int i = 1; i < rota1.caminho.size() - 1; i++) {
                int A = rota1.caminho[i - 1];
                int B = rota1.caminho[i];
                int C = rota1.caminho[i + 1];

                int valor_removido = c[A][C] - (c[A][B] + c[B][C]);
                // std::cout << "\n[Relocate] Testando cliente " << B
                //           << " da rota " << r1 << " (pos=" << i << ")\n";
                // std::cout << "  deltaRemove = " << valor_removido << "\n";

                for (int j = 0; j < rota2.caminho.size() - 1; j++) {
                    int X = rota2.caminho[j];
                    int Y = rota2.caminho[j + 1];

                    int valor_inserido = (c[X][B] + c[B][Y]) - c[X][Y];
                    int novo_valor = valor_removido + valor_inserido;

                    // std::cout << "    Inserindo entre (" << X << "," << Y
                    //           << ") da rota " << r2
                    //           << " -> deltaInsert=" << valor_inserido
                    //           << " deltaTotal=" << novo_valor;

                    if (novo_valor >= 0) {
                        // std::cout << " [descartado: não melhora]\n";
                        continue;
                    }

                    // aplica temporário
                    int cliente = B;
                    rota1.caminho.erase(rota1.caminho.begin() + i);
                    rota2.caminho.insert(rota2.caminho.begin() + (j + 1), cliente);


                    if(validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                         if (novo_valor < melhorValor) {
                        // std::cout << " **novo melhor movimento**";
                            melhorValor = novo_valor;
                            melhor_r1 = r1; 
                            melhor_i = i;
                            melhor_r2 = r2; 
                            melhor_j = j;
                     }

                    }

                    rota2.caminho.erase(rota2.caminho.begin() + (j + 1));
                    rota1.caminho.insert(rota1.caminho.begin() + i, cliente);
                }
            }
        }
    }

    if (melhor_r1 != -1) {
        Rota &rota_origem = solucao.rotas[melhor_r1];
        Rota &rota_destino = solucao.rotas[melhor_r2];

        int A = rota_origem.caminho[melhor_i - 1];
        int B = rota_origem.caminho[melhor_i];
        int C = rota_origem.caminho[melhor_i + 1];

        int X = rota_destino.caminho[melhor_j];
        int Y = rota_destino.caminho[melhor_j + 1];

        int valor_removido = c[A][C] - (c[A][B] + c[B][C]);
        int valor_inserido = (c[X][B] + c[B][Y]) - c[X][Y];
        int novo_valor = valor_removido + valor_inserido;

        // std::cout << "\n[Relocate] >>> Aplicando movimento final: cliente "
        //           << B << " da rota " << melhor_i << " -> rota " << melhor_j
        //           << " entre (" << X << "," << Y << ")"
        //           << " | deltaFinal=" << novo_valor << "\n";

        rota_origem.caminho.erase(rota_origem.caminho.begin() + melhor_i);
        rota_destino.caminho.insert(rota_destino.caminho.begin() + (melhor_j + 1), B);

        rota_origem.custo += valor_removido;
        rota_destino.custo += valor_inserido;
        solucao.custoFinal += novo_valor;

        return true;
    }

    // std::cout << "\n[Relocate] Nenhuma melhoria encontrada.\n";
    return false;
}

bool aplicarRelocate(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c,int Q){
    int melhor_r = -1;         
    int melhor_i = -1;         
    int melhor_j = -1;      
    int melhorValor = 0;   

    for (int r = 0; r < (int)solucao.rotas.size(); ++r) {
        Rota &rota = solucao.rotas[r];
        const int L = (int)rota.caminho.size();
        if (L <= 3) continue;  // 0-X-0: não há para onde reposicionar

        // para cada cliente B na rota 
        for (int i = 1; i < L - 1; ++i) {
            int A = rota.caminho[i - 1];
            int B = rota.caminho[i];
            int C = rota.caminho[i + 1];

            
            int deltaRemove = c[A][C] - (c[A][B] + c[B][C]);

            for (int j = 0; j < L - 1; j++) {
                
                if (j == i || j == i - 1)
                 continue;

                int X = rota.caminho[j];
                int Y = rota.caminho[j + 1];

                // custo de INSERIR B entre (X-Y): (X-Y) -> (X-B-Y)
                int deltaInsert = (c[X][B] + c[B][Y]) - c[X][Y];
                int novo_valor  = deltaRemove + deltaInsert; // delta total

                if (novo_valor >= 0) 
                    continue; // só melhorias estritas

                // ---- aplica temporário para validar ----
                // após remover i, o índice de inserção muda se j > i
                int posIns;

                if (j < i)
                    posIns = j + 1;
                else
                    posIns = j;

                rota.caminho.erase(rota.caminho.begin() + i);
                rota.caminho.insert(rota.caminho.begin() + posIns, B);

                bool ok = validaRota(rota, d, Q);

                // rollback
                rota.caminho.erase(rota.caminho.begin() + posIns);
                rota.caminho.insert(rota.caminho.begin() + i, B);

                if (!ok) continue;

                if (novo_valor < melhorValor) {
                    melhorValor = novo_valor;
                    melhor_r = r;
                    melhor_i = i;
                    melhor_j = j;
                }
            }
        }
    }

    // aplica melhor movimento encontrado
    if (melhor_r != -1) {
        Rota &rota = solucao.rotas[melhor_r];

        int A = rota.caminho[melhor_i - 1];
        int B = rota.caminho[melhor_i];
        int C = rota.caminho[melhor_i + 1];

        int X = rota.caminho[melhor_j];
        int Y = rota.caminho[melhor_j + 1];

        int deltaRemove = c[A][C] - (c[A][B] + c[B][C]);
        int deltaInsert = (c[X][B] + c[B][Y]) - c[X][Y];
        int novo_valor  = deltaRemove + deltaInsert; // < 0

        int posIns = (melhor_j < melhor_i ? melhor_j + 1 : melhor_j);
        rota.caminho.erase(rota.caminho.begin() + melhor_i);
        rota.caminho.insert(rota.caminho.begin() + posIns, B);

        // atualiza custos incrementalmente
        rota.custo += novo_valor;
        solucao.custoFinal += novo_valor;

        return true;
    }

    return false; // nenhuma melhoria
}
