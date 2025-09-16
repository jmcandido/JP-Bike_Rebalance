#include "grasp.h"
#include "vnd.h"


// Escolhe via LCR usando somente índices de clientes
int escolherGRASP(const vector<int>& viaveis,int atual,const vector<vector<int>>& c,double alpha, int gmin, int gmax) {

    double limite = gmin + alpha * (gmax - gmin);

    vector<int> LCR;

    for (int v : viaveis) {
        if (c[atual][v] <= limite)
            LCR.push_back(v);
    }

    random_device seed;
    mt19937 gerador(seed());

    uniform_int_distribution<int> sorteio(0, LCR.size() - 1);

    int prox = LCR[sorteio(gerador)]; 
    
    return prox;
}

Rota construirRota_GRASP(int n, int Q,
                         const vector<int>& d,
                         const vector<vector<int>>& c,
                         vector<bool>& visitado,
                         int& naoVisitados,
                         double alpha) {
    
    Rota rota;                        
    rota.custo = 0;
    rota.caminho.push_back(0); 

    vector<int> cand_viaveis;
    int atual = 0;

    int cargaAtual = 0;
    int carga_minima = 0;
    int carga_maxima = 0;
    int g;
    //int max_clientes = 0;
    

    while (true) {

        cand_viaveis.clear();
        int gmin = INT_MAX;
        int gmax = INT_MIN;

        // monta lista de candidatos viáveis 
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) {

                int novaCarga = cargaAtual + d[i];           
                int novoMin = min(carga_minima, novaCarga);
                int novoMax = max(carga_maxima, novaCarga);

                if (-novoMin <= Q - novoMax) { // valida candidato
                    cand_viaveis.push_back(i);
                    g = c[atual][i];
                    
                    if(g < gmin) 
                        gmin = g;
                    if (g > gmax) 
                        gmax = g;
                }

            }
        }

        if (cand_viaveis.empty())
            break;

        // escolha via GRASP
        int prox = escolherGRASP(cand_viaveis, atual, c, alpha, gmin,gmax);

        // adiciona cliente escolhido
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;

        // atualiza acumuladores incrementais
        cargaAtual += d[prox];
        carga_minima = min(carga_minima, cargaAtual);
        carga_maxima = max(carga_maxima, cargaAtual);
        atual = prox;
    }

    // fecha rota
    rota.caminho.push_back(0);
    rota.custo += c[atual][0];
    return rota;
}


Resultado guloso_GRASP(int n, int m, int Q,
                       const vector<int>& d,
                       const vector<vector<int>>& c,
                       double alpha) {
    Resultado res;
    vector<bool> visitado(n, false);
    int naoVisitados = n;

    
    while (naoVisitados > 0){
        Rota rota = construirRota_GRASP(n, Q, d, c, visitado, naoVisitados, alpha);
        res.custoFinal += rota.custo;
        res.rotas.push_back(rota);  
    }
    
    return res;

    }

Resultado GRASP(int n, int m, int Q,const vector<int>& d,const vector<vector<int>>& c,double alpha,int qtd_iteracoes){
    
    Resultado melhor;
    Resultado S;
    melhor.custoFinal = INT_MAX;

    for (int i = 0; i < qtd_iteracoes; i++) {
        
        S = guloso_GRASP(n, m, Q, d, c, alpha);

        // Busca local
        //RVND(S, d, c, Q);

        if (S.custoFinal < melhor.custoFinal) {
            melhor = std::move(S); 
        }
    }

    return melhor;
}
