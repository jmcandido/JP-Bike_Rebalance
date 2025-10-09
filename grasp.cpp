#include "grasp.h"
#include "vnd.h"

// Escolhe via LCR
int escolherGRASP(const vector<int> &viaveis, int atual, const vector<vector<int>> &c, double alpha, int gmin, int gmax){

    double limite = gmin + alpha * (gmax - gmin);

    vector<int> LCR;

    for (int v : viaveis){
        if (c[atual][v] <= limite)
            LCR.push_back(v);
    }

    random_device seed;
    mt19937 gerador(seed());

    uniform_int_distribution<int> sorteio(0, LCR.size() - 1);

    int prox = LCR[sorteio(gerador)];

    return prox;
}

Rota construirRota_GRASP(const int n, const int Q, const vector<int> &d, const vector<vector<int>> &c, vector<bool> &visitado,int &naoVisitados,double alpha){

    Rota rota;
    rota.custo = 0;
    rota.caminho.push_back(0);

    int atual = 0;

    int cargaAtual = 0;
    int carga_minima = 0;
    int carga_maxima = 0;

    vector<int> cand_viaveis;

    while (true){

        cand_viaveis.clear();

        int gmin = INT_MAX;
        int gmax = INT_MIN;

        // monta lista de candidatos viáveis
        for (int i = 1; i <= n; i++){
            if (!visitado[i]){
                

                // Q = 10   
                int novaCarga = cargaAtual + d[i];
                int novoMin = min(carga_minima, novaCarga);
                int novoMax = max(carga_maxima, novaCarga);

                    //[10, 10]

                    //[11,10]

                if (-novoMin <= Q - novoMax){ // valida candidato
                    cand_viaveis.push_back(i);
                    gmin = min(gmin, c[atual][i]); // menor custo entre todos os candidatos
                    gmax = max(gmax, c[atual][i]); // maior custo entre todos os candidatos
                }
            }
        }

        if (cand_viaveis.empty())
            break;

        
        int prox = escolherGRASP(cand_viaveis, atual, c, alpha, gmin, gmax);

        // adiciona cliente escolhido
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;

        // atualiza acumuladores
        cargaAtual += d[prox];
        carga_minima = min(carga_minima, cargaAtual);
            // MIN(0, -10) = -10
        carga_maxima = max(carga_maxima, cargaAtual);

            // max(0, -10) = 0
        atual = prox;
    }

    // fecha rota
    rota.caminho.push_back(0);
    rota.custo += c[atual][0];
    return rota;
}

Resultado guloso_GRASP(const int n, const int m, const int Q, const vector<int> &d,const vector<vector<int>> &c, double alpha){

    Resultado res;
    res.custoFinal = 0;
    vector<bool> visitado(n + 1, false);
    int naoVisitados = n;

   
    for(int i = 0; i < m && naoVisitados > 0; i++){
        Rota rota = construirRota_GRASP(n, Q, d, c, visitado, naoVisitados, alpha);
        res.custoFinal += rota.custo;
        res.rotas.push_back(rota);
    }


    return res;
}

Resultado GRASP(const int n, const int m, const int Q, const vector<int> &d, const vector<vector<int>> &c, double alpha, int qtd_iteracoes){

    Resultado melhor;
    Resultado solucao;
    melhor.custoFinal = INT_MAX;

    for (int i = 0; i < qtd_iteracoes; i++){

        solucao = guloso_GRASP(n, m, Q, d, c, alpha);

        RVND(solucao, d, c, Q); // Busca local

        //VND(solucao, d, c, Q);

        if (solucao.custoFinal < melhor.custoFinal)
            melhor = std::move(solucao);
    }

    return melhor;
}
