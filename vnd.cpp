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
    int melhor_valor = 0;
    int melhor_valr1 = 0, melhor_valr2 = 0;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++){
        Rota &rota1 = solucao.rotas[r1];

        for (int r2 = r1 + 1; r2 < solucao.rotas.size(); r2++){
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

                    int valr1 = (c[rota1.caminho[i - 1]][rota2.caminho[j]] + c[rota2.caminho[j]][rota1.caminho[i + 1]]) 
                                - (c[rota1.caminho[i - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota1.caminho[i + 1]]); // efeito na rota1



                    int valr2 = (c[rota2.caminho[j - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota2.caminho[j + 1]]) 
                              - (c[rota2.caminho[j - 1]][rota2.caminho[j]] + c[rota2.caminho[j]][rota2.caminho[j + 1]]); // efeito na rota2

                    int valor = valr1 + valr2;


                    if ((valor < melhor_valor)){
                        // valida temporariamente as duas rotas
                        swap(rota1.caminho[i], rota2.caminho[j]);

                        if (validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                            melhor_valor = valor;
                            melhor_valr1 = valr1;
                            melhor_valr2 = valr2;
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

        int val_r1 = (c[A][E] + c[E][C]) - (c[A][B] + c[B][C]);
        int val_r2 = (c[D][B] + c[B][F]) - (c[D][E] + c[E][F]);
        int valor = val_r1 + val_r2;

        swap(rota1.caminho[i], rota2.caminho[j]);

        // Atualiza custos
        rota1.custo += val_r1;
        rota2.custo += val_r2;
        solucao.custoFinal += valor;

        return true;
    }

    return false;
}

bool aplicarSwapGeral(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, int Q){
    int melhor_r = -1, melhor_i = -1, melhor_j = -1;
    int melhor_valor = 0;
    int valor = 0;

    for (int r = 0; r < solucao.rotas.size(); ++r){
        Rota &rota = solucao.rotas[r];

        if (rota.caminho.size() > 3){ // 0-X-0 não há o que trocar

            for (int i = 1; i < rota.caminho.size() - 2; i++){ // garante que sempre exista i+1 (cliente válido)
                for (int j = i + 1; j < rota.caminho.size() - 1; j++){ // evita que j seja o depósito final. 

                    int A = rota.caminho[i - 1];
                    int B = rota.caminho[i];
                    int E = rota.caminho[j];
                    int F = rota.caminho[j + 1];

                    if (j == i + 1){
                        valor = (c[rota.caminho[i - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]])
                              - (c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j + 1]]);
                    }
                    else{
                        valor = (c[rota.caminho[i - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i + 1]] + c[rota.caminho[j - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                              - (c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[i + 1]] + c[rota.caminho[j - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j + 1]]);
                    }

                    if (valor < melhor_valor){ // compensa checar
                        swap(rota.caminho[i], rota.caminho[j]);

                        if (validaRota(rota, d, Q)){
                            melhor_valor = valor;
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

        if (j == i + 1){

            valor = (c[rota.caminho[i - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                  - (c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j + 1]]);
        }
        else{
            int C = rota.caminho[i + 1];
            int D = rota.caminho[j - 1];

            valor = (c[rota.caminho[i - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i + 1]] + c[rota.caminho[j - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                 - (c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[i + 1]] + c[rota.caminho[j - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j + 1]]);
        }

        // aplica efetivamente
        swap(rota.caminho[melhor_i], rota.caminho[melhor_j]);
        rota.custo += valor;
        solucao.custoFinal += valor;

        return true;
    }

    return false;
}

bool aplicarRelocateInter(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, int Q){

    // cout << "RELOCATE" << endl;
    int melhor_r1 = -1, melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;
    int melhorValor = 0;


    int valor_inserido;
    int valor_removido;
    int novo_valor;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++){

        Rota &rota1 = solucao.rotas[r1];

        if(rota1.caminho.size() > 2){

            for (int r2 = 0; r2 < solucao.rotas.size(); r2++){

            if (r1 != r2){
                
                Rota &rota2 = solucao.rotas[r2];

                    for (int i = 1; i < rota1.caminho.size() - 1; i++){
                    
                        valor_removido = c[rota1.caminho[i - 1]][rota1.caminho[i + 1]] 
                        - (c[rota1.caminho[i - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota1.caminho[i + 1]]);

                        for (int j = 0; j < rota2.caminho.size() - 1; j++){

                            valor_inserido = (c[rota2.caminho[j]][rota1.caminho[i]] + c[rota1.caminho[i]][rota2.caminho[j + 1]]) 
                                                - c[rota2.caminho[j]][rota2.caminho[j + 1]];

                            novo_valor = valor_removido + valor_inserido;

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
        
 }

    if (melhor_r1 != -1){
        Rota &rota_origem = solucao.rotas[melhor_r1];
        Rota &rota_destino = solucao.rotas[melhor_r2];

        valor_removido = c[rota_origem.caminho[melhor_i - 1]][rota_origem.caminho[melhor_i + 1]] 
                            -(c[rota_origem.caminho[melhor_i - 1]][rota_origem.caminho[melhor_i]] 
                            +c[rota_origem.caminho[melhor_i]][rota_origem.caminho[melhor_i + 1]]);


        valor_inserido = (c[rota_destino.caminho[melhor_j]][rota_origem.caminho[melhor_i]] + c[rota_origem.caminho[melhor_i]][rota_destino.caminho[melhor_j + 1]]) 
                            - c[rota_destino.caminho[melhor_j]][rota_destino.caminho[melhor_j + 1]];


        novo_valor = valor_removido + valor_inserido;

        int cliente = rota_origem.caminho[melhor_i];
        rota_origem.caminho.erase(rota_origem.caminho.begin() + melhor_i);
        rota_destino.caminho.insert(rota_destino.caminho.begin() + (melhor_j + 1), cliente);

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

    int novo_valor;
    int valor_inserido;
    int valor_removido;
    int posicao_inserida;

    for (int r = 0; r < solucao.rotas.size(); ++r){
        Rota &rota = solucao.rotas[r];
    

        // para cada cliente B na rota
        for (int i = 1; i < rota.caminho.size() - 1; i++){
            
            int valor_removido = c[rota.caminho[i - 1]][rota.caminho[i + 1]] 
                                -(c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[i + 1]]);

            for (int j = 0; j < rota.caminho.size() - 1; j++){

                if (!(j == i || j == i - 1)){

                    valor_inserido = (c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                                        - c[rota.caminho[j]][rota.caminho[j + 1]];

                    novo_valor = valor_removido + valor_inserido; // delta total

                    if (novo_valor < melhorValor){

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

        valor_removido = c[rota.caminho[melhor_i - 1]][rota.caminho[melhor_i + 1]] 
                          - (c[rota.caminho[melhor_i - 1]][rota.caminho[melhor_i]] 
                          + c[rota.caminho[melhor_i]][rota.caminho[melhor_i + 1]]);




        valor_inserido = (c[rota.caminho[melhor_j]][rota.caminho[melhor_i]]
                           + c[rota.caminho[melhor_i]][rota.caminho[melhor_j + 1]])
                           - c[rota.caminho[melhor_j]][rota.caminho[melhor_j + 1]];

        
        novo_valor = valor_inserido + valor_removido; // < 0

        if(melhor_j < melhor_i)
            posicao_inserida = melhor_j + 1;
        else
            posicao_inserida = melhor_j;


        rota.caminho.erase(rota.caminho.begin() + melhor_i);
        rota.caminho.insert(rota.caminho.begin() + posicao_inserida, B);

        // atualiza custos 
        rota.custo += novo_valor;
        solucao.custoFinal += novo_valor;

        return true;
    }

    return false; 
}
