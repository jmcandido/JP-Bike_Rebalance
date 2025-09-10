#include "grasp.h"
#include "vnd.h"


// Escolhe via LCR usando somente índices de clientes
int escolherGRASP(const std::vector<int>& viaveis,int atual,const std::vector<std::vector<int>>& c,double alpha,std::mt19937& rng) {

    int gmin = INT_MAX;  // “+infinito” para começar
    int gmax = INT_MIN;  // “-infinito” para começar
    int g;

    for (int v : viaveis) {
        g = c[atual][v];       
        gmin = min(gmin, g);
        gmax = max(gmax, g);
    }

    double limite = gmin + alpha * (gmax - gmin);
    // cout << "gmin = " << gmin << endl;
    // cout << "gmax = " << gmax << endl;
    // cout << "alpha = " << alpha << endl;
    // cout << "Limite = " << limite << endl;
    vector<int> LCR;

    for (int v : viaveis) {
        if (c[atual][v] <= limite)
            LCR.push_back(v);
    }

    // for(int i = 0; i < LCR.size();i++){
    //     cout << LCR[i] << " ";
    // }
    // cout << endl;

    int proximo = LCR[ rng() % LCR.size() ];
    
    return proximo;
}

Rota construirRota_GRASP(int n, int Q,
                         const std::vector<int>& d,
                         const std::vector<std::vector<int>>& c,
                         std::vector<bool>& visitado,
                         int& naoVisitados,
                         double alpha,
                         std::mt19937& rng) {
    
    Rota rota;                        
    rota.custo = 0;
    rota.caminho.push_back(0); 

    std::vector<int> cand_viaveis;
    int atual = 0;

    // acumuladores incrementais
    int cargaAtual = 0;
    int minCarga = 0;
    int maxCarga = 0;

    while (true) {
        cand_viaveis.clear();

        // monta lista de candidatos viáveis incrementalmente
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) {
               int novaCarga = cargaAtual + d[i];
               
                int novoMin = std::min(minCarga, novaCarga);
                int novoMax = std::max(maxCarga, novaCarga);

                if (-novoMin <= Q - novoMax) {
                    cand_viaveis.push_back(i);
                }

            }
        }

        if (cand_viaveis.empty())
            break;

        // escolha via GRASP
        int prox = escolherGRASP(cand_viaveis, atual, c, alpha, rng);

        // adiciona cliente escolhido
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;

        // atualiza acumuladores incrementais
        cargaAtual += d[prox];
        minCarga = std::min(minCarga, cargaAtual);
        maxCarga = std::max(maxCarga, cargaAtual);
        atual = prox;
    }

    // fecha rota
    rota.caminho.push_back(0);
    rota.custo += c[atual][0];
    return rota;
}


Resultado guloso_GRASP(int n, int m, int Q,
                       const std::vector<int>& d,
                       const std::vector<std::vector<int>>& c,
                       double alpha,
                       std::mt19937& rng) {
    Resultado res;
    std::vector<bool> visitado(n + 1, false);
    int naoVisitados = n;

    for (int i = 0; i < m && naoVisitados > 0; i++) {
        Rota rota = construirRota_GRASP(n, Q, d, c, visitado, naoVisitados, alpha, rng);
            res.custoFinal += rota.custo;
            res.rotas.push_back(rota);  
    }

    return res;

    }

Resultado GRASP(int n, int m, int Q,const std::vector<int>& d,const std::vector<std::vector<int>>& c,double alpha,int qtd_iteracoes,std::mt19937& rng){
    
    Resultado melhor;
    Resultado S;
    melhor.custoFinal = std::numeric_limits<int>::max();

    for (int i = 0; i < qtd_iteracoes; i++) {
        // Construção GRASP
        // std::cout << "Melhor solucao: " << melhor.custoFinal << '\n';
        S = guloso_GRASP(n, m, Q, d, c, alpha, rng);

        cout << "Solucao Inicial Grasp: ";
        imprimirResultado(S);
        cout << endl;

        // Busca local
        VND(S, d, c, Q);

        // std::cout << "Solucao Vnd pos Grasp: ";
        // imprimirResultado(S);
        // std::cout << '\n';

        // Atualiza melhor solução
        if (S.custoFinal < melhor.custoFinal) {
            melhor = std::move(S); 
        }
    }

        // std::cout << "Solucao Vnd pos Grasp: ";
        // imprimirResultado(S);
        // std::cout << '\n';


    return melhor;
}
