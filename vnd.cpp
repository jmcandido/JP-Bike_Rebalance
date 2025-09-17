#include "vnd.h"

void VND(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, const int Q){

    int k = 1;
    int vizinhancas = 4;

    while (k <= vizinhancas){

        bool melhorou = false;

        switch (k){
        case 1:
            melhorou = aplicarReinsertion(solucao, d, c, Q);
            break;
        case 2:
            melhorou = aplicarSwap(solucao, d, c, Q);
            break;
        case 3:
            melhorou = aplicarReinsertionInter(solucao, d, c, Q);
            break;
        case 4:
            melhorou = aplicarSwapInter(solucao, d, c, Q);
            break;
        }

        if (melhorou){
            k = 1; // volta para a primeira vizinhança
        }
        else{
            k++; // vai para a próxima vizinhança
        }
    }
}

void RVND(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, const int Q){

    vector<int> vizinhancas = {1, 2, 3, 4};

    random_device seed;
    mt19937 gerador(seed());

    while (!vizinhancas.empty()){

        uniform_int_distribution<int> distribuicao(0, vizinhancas.size() - 1);

        int indice = distribuicao(gerador); // sorteia um indice válido
        int k = vizinhancas[indice];        

        bool melhorou = false;

        switch (k){
        case 1:
            melhorou = aplicarReinsertion(solucao, d, c, Q);
            break;
        case 2:
            melhorou = aplicarSwap(solucao, d, c, Q);
            break;
        case 3:
            melhorou = aplicarReinsertionInter(solucao, d, c, Q);
            break;
        case 4:
            melhorou = aplicarSwapInter(solucao, d, c, Q);
            break;
        }

        if (melhorou){
            // reinicia todas as vizinhanças
            vizinhancas = {1, 2, 3, 4};
        }
        else{
            // remove a vizinhança testada
            vizinhancas.erase(vizinhancas.begin() + indice);
        }
    }
}

bool aplicarSwapInter(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, const int Q){

    int melhor_r1 = -1, melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;

    int melhor_valor = 0;
    int melhor_valr1 = 0, melhor_valr2 = 0;

    int novo_valr1;
    int novo_valr2;
    int novo_valor;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++){
        Rota &rota1 = solucao.rotas[r1];

        if (rota1.caminho.size() > 2){

            for (int r2 = r1 + 1; r2 < solucao.rotas.size(); r2++){
                Rota &rota2 = solucao.rotas[r2];

                if (rota2.caminho.size() > 2){

                    for (int i = 1; i < rota1.caminho.size() - 1; i++){

                        for (int j = 1; j < rota2.caminho.size() - 1; j++){

                            // troca  (rota1[i]) por (rota2[j])

                            // custo da troca (reconectando arestas nos dois lados)

                             novo_valr1 = (c[rota1.caminho[i - 1]][rota2.caminho[j]] + c[rota2.caminho[j]][rota1.caminho[i + 1]]) 
                                           - (c[rota1.caminho[i - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota1.caminho[i + 1]]); // efeito na rota1

                             novo_valr2 = (c[rota2.caminho[j - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota2.caminho[j + 1]]) - 
                                             (c[rota2.caminho[j - 1]][rota2.caminho[j]] + c[rota2.caminho[j]][rota2.caminho[j + 1]]); // efeito na rota2

                             novo_valor = novo_valr1 + novo_valr2;

                            if ((novo_valor < melhor_valor)){

                                // valida temporariamente as duas rotas
                                swap(rota1.caminho[i], rota2.caminho[j]);

                                if (validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                                    melhor_valor = novo_valor;

                                    melhor_valr1 = novo_valr1;
                                    melhor_valr2 = novo_valr2;

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
        }
    }

    // aplica o melhor movimento  encontrado
    if (melhor_r1 != -1){

        Rota &rota1 = solucao.rotas[melhor_r1];
        Rota &rota2 = solucao.rotas[melhor_r2];

        swap(rota1.caminho[melhor_i], rota2.caminho[melhor_j]);

        // Atualiza custos
        rota1.custo += melhor_valr1;
        rota2.custo += melhor_valr2;
        solucao.custoFinal += melhor_valor;

        return true;
    }

    return false;
}

bool aplicarSwap(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, const int Q){
    int melhor_r = -1, melhor_i = -1, melhor_j = -1;
    int melhor_valor = 0;
    int novo_valor;

    for (int r = 0; r < solucao.rotas.size(); ++r){
        Rota &rota = solucao.rotas[r];

        if (rota.caminho.size() > 3){ // 0-X-0 não há o que trocar

            for (int i = 1; i < rota.caminho.size() - 2; i++){ // garante que sempre exista i+1 (cliente válido)

                for (int j = i + 1; j < rota.caminho.size() - 1; j++){ // evita que j seja o depósito final

                    if (j == i + 1){
                        novo_valor = (c[rota.caminho[i - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                                   - (c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j + 1]]);
                    }
                    else{
                        novo_valor = (c[rota.caminho[i - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[i + 1]] + c[rota.caminho[j - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                                   - (c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[i + 1]] + c[rota.caminho[j - 1]][rota.caminho[j]] + c[rota.caminho[j]][rota.caminho[j + 1]]);
                    }

                    if (novo_valor < melhor_valor){ // compensa checar

                        swap(rota.caminho[i], rota.caminho[j]);

                        if (validaRota(rota, d, Q)){
                            melhor_valor = novo_valor;
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

        swap(rota.caminho[melhor_i], rota.caminho[melhor_j]);
        rota.custo += melhor_valor;
        solucao.custoFinal += melhor_valor;

        return true;
    }

    return false;
}

bool aplicarReinsertionInter(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, const int Q){

    int melhor_r1 = -1, melhor_i = -1;
    int melhor_r2 = -1, melhor_j = -1;
    int melhorValor = 0;
    int melhor_removido = 0;
    int melhor_inserido = 0;

    int valor_inserido;
    int valor_removido;
    int novo_valor;

    for (int r1 = 0; r1 < solucao.rotas.size(); r1++){

        Rota &rota1 = solucao.rotas[r1];

        if (rota1.caminho.size() > 2){

            for (int r2 = 0; r2 < solucao.rotas.size(); r2++){

                if (r1 != r2){

                    Rota &rota2 = solucao.rotas[r2];

                    for (int i = 1; i < rota1.caminho.size() - 1; i++){

                        valor_removido = c[rota1.caminho[i - 1]][rota1.caminho[i + 1]] - (c[rota1.caminho[i - 1]][rota1.caminho[i]] + c[rota1.caminho[i]][rota1.caminho[i + 1]]);

                        for (int j = 0; j < rota2.caminho.size() - 1; j++){

                            valor_inserido = (c[rota2.caminho[j]][rota1.caminho[i]] + c[rota1.caminho[i]][rota2.caminho[j + 1]]) - c[rota2.caminho[j]][rota2.caminho[j + 1]];

                            novo_valor = valor_removido + valor_inserido;

                            if (novo_valor < melhorValor){
                                int cliente = rota1.caminho[i]; // nao perco a referência correta do índice, porque o valor foi salvo antes da modificação do vetor

                                rota1.caminho.erase(rota1.caminho.begin() + i);
                                rota2.caminho.insert(rota2.caminho.begin() + (j + 1), cliente);

                                if (validaRota(rota1, d, Q) && validaRota(rota2, d, Q)){
                                    melhorValor = novo_valor;
                                    melhor_r1 = r1;
                                    melhor_i = i;

                                    melhor_r2 = r2;
                                    melhor_j = j;

                                    melhor_removido = valor_removido;
                                    melhor_inserido = valor_inserido;
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

        int cliente = rota_origem.caminho[melhor_i];
        rota_origem.caminho.erase(rota_origem.caminho.begin() + melhor_i);
        rota_destino.caminho.insert(rota_destino.caminho.begin() + (melhor_j + 1), cliente);

        rota_origem.custo += melhor_removido;
        rota_destino.custo += melhor_inserido;
        solucao.custoFinal += melhorValor;

        return true;
    }

    return false;
}

bool aplicarReinsertion(Resultado &solucao, const vector<int> &d, const vector<vector<int>> &c, const int Q){

    int melhor_r = -1;
    int melhor_i = -1;
    int melhorValor = 0;

    int novo_valor;
    int valor_inserido;
    int valor_removido;
    int posicao_inserida;

    int melhor_pos_inserida = -1;
    int melhor_cliente = -1;

    for (int r = 0; r < solucao.rotas.size(); r++){

        Rota &rota = solucao.rotas[r];

        // para cada cliente  na rota
        for (int i = 1; i < rota.caminho.size() - 1; i++){

            valor_removido = c[rota.caminho[i - 1]][rota.caminho[i + 1]] 
                            -(c[rota.caminho[i - 1]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[i + 1]]);

            for (int j = 0; j < rota.caminho.size() - 1; j++){

                if (!(j == i || j == i - 1)){

                    valor_inserido = (c[rota.caminho[j]][rota.caminho[i]] + c[rota.caminho[i]][rota.caminho[j + 1]]) 
                                   - c[rota.caminho[j]][rota.caminho[j + 1]];

                    novo_valor = valor_removido + valor_inserido; // delta total

                    if (novo_valor < melhorValor){

                        if (j < i)                    // precisa ajustar  porque depois do erase, os índices à frente diminuem em 1.
                            posicao_inserida = j + 1; // Quando o arco está antes do cliente removido precisa compensar com +1.

                        else
                            posicao_inserida = j; // nao precisa ajustar, j já aponta certo.

                        int cliente = rota.caminho[i];
                        rota.caminho.erase(rota.caminho.begin() + i);
                        rota.caminho.insert(rota.caminho.begin() + posicao_inserida, cliente);

                        if (validaRota(rota, d, Q)){
                            melhorValor = novo_valor;
                            melhor_r = r;
                            melhor_i = i;
                            melhor_cliente = cliente;
                            melhor_pos_inserida = posicao_inserida;
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

        rota.caminho.erase(rota.caminho.begin() + melhor_i);
        rota.caminho.insert(rota.caminho.begin() + melhor_pos_inserida, melhor_cliente);

        // atualiza custos
        rota.custo += melhorValor;
        solucao.custoFinal += melhorValor;

        return true;
    }

    return false;
}
