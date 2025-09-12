#include "vnd.h"
#include "resultados.h"
#include <algorithm>

void VND(Resultado& solucao, const vector<int>& d,const vector<vector<int>>& c,int Q) {
    int k = 1;
    int vizinhancas = 4; 

    while (k <= vizinhancas) {
        bool melhorou = false;
        //cout << "K = " << k << endl;
        switch (k) {
            case 1:
                melhorou = aplicarRelocate(solucao, d, c, Q); 
                break;
            case 2:
                melhorou = aplicarSwapGeral(solucao, d, c, Q);
                break;
            case 3:
                melhorou = aplicarRelocateInter(solucao, d, c, Q);
                break;
            case 4:
                melhorou = aplicarSwapInterRotasGeral(solucao, d, c, Q);
                break;
        }

        if (melhorou) {
            // cout << "Melhorei" << endl;
            k = 1; // volta para a primeira vizinhança
        } else {
            k++;   // vai para a próxima vizinhança
        }
    }
}

bool aplicarSwapInterRotasGeral(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, int Q){
    int melhor_r1 = -1, melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;
    int melhor_delta = 0;
    int melhor_delta1 = 0, melhor_delta2 = 0;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++){
        for (int r2 = r1 + 1; r2 < solucao.rotas.size(); r2++){
            Rota &rota1 = solucao.rotas[r1];
            Rota &rota2 = solucao.rotas[r2];

            for (int i = 1; i < rota1.caminho.size() - 1; i++){
                for (int j = 1; j < rota2.caminho.size() - 1; j++){

                    // vizinhança: troca  (rota1[i]) por (rota2[j])
                    int A = rota1.caminho[i - 1];
                    int B = rota1.caminho[i];
                    int C = rota1.caminho[i + 1];

                    int D = rota2.caminho[j - 1];
                    int E = rota2.caminho[j];
                    int F = rota2.caminho[j + 1];

                    // custo da troca (reconectando arestas nos dois lados)
                    int delta1 = (c[A][E] + c[E][C]) - (c[A][B] + c[B][C]); // efeito na rota1
                    int delta2 = (c[D][B] + c[B][F]) - (c[D][E] + c[E][F]); // efeito na rota2
                    int delta = delta1 + delta2;

                    if ((delta < melhor_delta)){
                        // valida temporariamente as duas rotas
                        swap(rota1.caminho[i], rota2.caminho[j]);

                        if (validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                            melhor_delta = delta;
                            melhor_delta1 = delta1;
                            melhor_delta2 = delta2;
                            melhor_r1 = r1;
                            melhor_i = i;
                            melhor_r2 = r2;
                            melhor_j = j;
                        }
                        swap(rota1.caminho[i], rota2.caminho[j]);
                    }
                }
            }
        }
    }

    // aplica o melhor movimento global encontrado
    if (melhor_r1 != -1){

        Rota &rota1 = solucao.rotas[melhor_r1];
        Rota &rota2 = solucao.rotas[melhor_r2];

        int i = melhor_i, j = melhor_j;

        int A = rota1.caminho[i - 1];
        int B = rota1.caminho[i];
        int C = rota1.caminho[i + 1];

        int D = rota2.caminho[j - 1];
        int E = rota2.caminho[j];
        int F = rota2.caminho[j + 1];

        int delta1 = (c[A][E] + c[E][C]) - (c[A][B] + c[B][C]);
        int delta2 = (c[D][B] + c[B][F]) - (c[D][E] + c[E][F]);
        int delta = delta1 + delta2;

        swap(rota1.caminho[i], rota2.caminho[j]);

        // Atualiza custos
        rota1.custo += delta1;
        rota2.custo += delta2;
        solucao.custoFinal += delta;

        return true;
    }

    return false;
}

bool aplicarSwapGeral(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, int Q){
    int melhor_r = -1, melhor_i = -1, melhor_j = -1;
    int melhor_delta = 0;
    int delta = 0;

    for (int r = 0; r < solucao.rotas.size(); ++r){
        Rota &rota = solucao.rotas[r];

        if (rota.caminho.size() > 2)
        { // 0-X-0 não há o que trocar
            for (int i = 1; i < rota.caminho.size() - 2; i++)
            {
                for (int j = i + 1; j < rota.caminho.size() - 1; j++)
                {
                    int A = rota.caminho[i - 1];
                    int B = rota.caminho[i];
                    int E = rota.caminho[j];
                    int F = rota.caminho[j + 1];

                    if (j == i + 1)
                    {
                        // ... A - B - E - F ...
                        delta = (c[A][E] + c[E][B] + c[B][F]) - (c[A][B] + c[B][E] + c[E][F]);
                    }
                    else
                    {
                        // ... A - B - C ... D - E - F ...
                        int C = rota.caminho[i + 1];
                        int D = rota.caminho[j - 1];
                        delta = (c[A][E] + c[E][C] + c[D][B] + c[B][F]) - (c[A][B] + c[B][C] + c[D][E] + c[E][F]);
                    }

                    // if (j == i + 1) {
                    //     delta = (c[rota.caminho[i-1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j+1]])
                    //           - (c[rota.caminho[i-1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j+1]]);
                    // } else {
                    //     delta = (c[rota.caminho[i-1]][rota.caminho[j]]  + c[rota.caminho[j]][rota.caminho[i+1]]
                    //             + c[rota.caminho[j-1]][rota.caminho[i]]
                    //             + c[rota.caminho[i]][rota.caminho[j+1]])
                    //           - (c[rota.caminho[i-1]][rota.caminho[i]]
                    //             + c[rota.caminho[i]][rota.caminho[i+1]]
                    //             + c[rota.caminho[j-1]][rota.caminho[j]]
                    //             + c[rota.caminho[j]][rota.caminho[j+1]]);
                    // }

                    if (delta < melhor_delta){ // compensa checar
                        swap(rota.caminho[i], rota.caminho[j]);

                        if (validaRota(rota, d, Q)){
                            melhor_delta = delta;
                            melhor_r = r;
                            melhor_i = i;
                            melhor_j = j;
                        }

                        swap(rota.caminho[i], rota.caminho[j]);
                    }
                }
            }
        }
    }

    // aplica o melhor movimento encontrado
    if (melhor_r != -1){

        Rota &rota = solucao.rotas[melhor_r];

        int i = melhor_i, j = melhor_j;

        int A = rota.caminho[i - 1];
        int B = rota.caminho[i];

        int E = rota.caminho[j];
        int F = rota.caminho[j + 1];

        if (j == i + 1)
        {
            delta = (c[A][E] + c[E][B] + c[B][F]) - (c[A][B] + c[B][E] + c[E][F]);
        }
        else
        {
            int C = rota.caminho[i + 1];
            int D = rota.caminho[j - 1];
            delta = (c[A][E] + c[E][C] + c[D][B] + c[B][F]) - (c[A][B] + c[B][C] + c[D][E] + c[E][F]);
        }

        // aplica efetivamente
        swap(rota.caminho[melhor_i], rota.caminho[melhor_j]);
        rota.custo += delta;
        solucao.custoFinal += delta;

        return true;
    }

    return false;
}

bool aplicarRelocateInter(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, int Q){

    // cout << "RELOCATE" << endl;
    int melhor_r1 = -1, melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;
    int melhorValor = 0;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++){

        Rota &rota1 = solucao.rotas[r1];

        //cout << "Iteracao: " << r1 << endl;
        

       // cout << endl;

        if(rota1.caminho.size() <= 2){
            // cout << "Não tem ninguem para retirar " << endl;
            continue;
        }   

        for (int r2 = 0; r2 < solucao.rotas.size(); r2++){

            if (r1 != r2){
                
                Rota &rota2 = solucao.rotas[r2];

                    // cout << "Comparando Rota: " << r1 + 1<< " com rota " << r2 + 1 << endl;
                    // cout << "R1: ";

                    // for(int estacao: rota1.caminho){

                    //         cout << estacao << " ";
                    // }
                    
                    // cout << endl;
                    // cout << "R2: ";
                    
                    // for(int estacao: rota2.caminho){

                    //         cout << estacao << " ";
                    // }
                    
                    // cout << endl;

                    for (int i = 1; i < rota1.caminho.size() - 1; i++){
                        //cout << "vou checar meu fi para i = "  << i << endl;
                        int A = rota1.caminho[i - 1];
                        int B = rota1.caminho[i];
                        int C = rota1.caminho[i + 1];

                        int valor_removido = c[rota1.caminho[i - 1]][rota1.caminho[i + 1]] 
                        - (c[rota1.caminho[i - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota1.caminho[i + 1]]);

                        for (int j = 0; j < rota2.caminho.size() - 1; j++){
                            //cout << "vou checar meu fi para j = "  << j << endl;
                            int X = rota2.caminho[j];
                            int Y = rota2.caminho[j + 1];

                            int valor_inserido = (c[rota2.caminho[j]][rota1.caminho[i]] + c[rota1.caminho[i]][rota2.caminho[j + 1]]) 
                                                - c[rota2.caminho[j]][rota2.caminho[j + 1]];

                            int novo_valor = valor_removido + valor_inserido;

                            if (novo_valor < melhorValor) {
                                int cliente = rota1.caminho[i]; //nao perco a referência correta do índice, porque o valor foi salvo antes da modificação do vetor

                                rota1.caminho.erase(rota1.caminho.begin() + i);
                                rota2.caminho.insert(rota2.caminho.begin() + (j + 1), cliente);

                                if (validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                                    melhorValor = novo_valor;
                                    melhor_r1 = r1;
                                    melhor_i = i;
                                    melhor_r2 = r2;
                                    melhor_j = j;
                                }

                                rota2.caminho.erase(rota2.caminho.begin() + (j + 1));
                                rota1.caminho.insert(rota1.caminho.begin() + i, cliente);
                                
                            } 
                        }
                    }
                
            }
        }
    }

    if (melhor_r1 != -1){
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

        rota_origem.caminho.erase(rota_origem.caminho.begin() + melhor_i);
        rota_destino.caminho.insert(rota_destino.caminho.begin() + (melhor_j + 1), B);

        rota_origem.custo += valor_removido;
        rota_destino.custo += valor_inserido;
        solucao.custoFinal += novo_valor;

        return true;
    }
    return false;
}

bool aplicarRelocate(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, int Q){
    int melhor_r = -1;
    int melhor_i = -1;
    int melhor_j = -1;
    int melhorValor = 0;

    for (int r = 0; r < solucao.rotas.size(); ++r){
        Rota &rota = solucao.rotas[r];
    

        // para cada cliente B na rota
        for (int i = 1; i < rota.caminho.size() - 1; i++){
            // int A = rota.caminho[i - 1];
            // int B = rota.caminho[i];
            // int C = rota.caminho[i + 1];

            int valor_removido = c[ rota.caminho[i - 1]][rota.caminho[i + 1]] 
                                -(c[ rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[i + 1]]);

            for (int j = 0; j < rota.caminho.size() - 1; j++){

                if (!(j == i || j == i - 1)){

                    int X = rota.caminho[j];
                    int Y = rota.caminho[j + 1];
                    // custo de INSERIR B entre (X-Y): (X-Y) -> (X-B-Y)
                    int valor_inserido = (c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                                        - c[rota.caminho[j]][rota.caminho[j + 1]];

                    int novo_valor = valor_removido + valor_inserido; // delta total

                    if (novo_valor < melhorValor){

                        int posicao_inserida;

                        if (j < i) //precisa ajustar  porque depois do erase, os índices à frente diminuem em 1.
                            posicao_inserida = j + 1;
                        else
                            posicao_inserida = j; // nao precisa ajustar, j já aponta certo.

                        int cliente = rota.caminho[i];
                        rota.caminho.erase(rota.caminho.begin() + i);
                        rota.caminho.insert(rota.caminho.begin() + posicao_inserida, cliente);

                        if (validaRota(rota, d, Q)){
                            melhorValor = novo_valor;
                            melhor_r = r;
                            melhor_i = i;
                            melhor_j = j;
                        }

                        rota.caminho.erase(rota.caminho.begin() + posicao_inserida);
                        rota.caminho.insert(rota.caminho.begin() + i, cliente);

                        }
                }
            }
        }
    }

    // aplica melhor movimento encontrado
    if (melhor_r != -1){
        Rota &rota = solucao.rotas[melhor_r];

        int A = rota.caminho[melhor_i - 1];
        int B = rota.caminho[melhor_i];
        int C = rota.caminho[melhor_i + 1];

        int X = rota.caminho[melhor_j];
        int Y = rota.caminho[melhor_j + 1];

        int deltaRemove = c[A][C] - (c[A][B] + c[B][C]);
        int deltaInsert = (c[X][B] + c[B][Y]) - c[X][Y];
        int novo_valor = deltaRemove + deltaInsert; // < 0

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
