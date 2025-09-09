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
     cout << "Limite = " << limite << endl;
    vector<int> LCR;

    for (int v : viaveis) {
        if (c[atual][v] <= limite)
            LCR.push_back(v);
    }

    for(int i = 0; i < LCR.size();i++){
        cout << LCR[i] << " ";
    }
    cout << endl;

    int proximo = LCR[ rng() % LCR.size() ];
    
    return proximo;
}

bool construirRota_GRASP(int n, int Q,
                         const std::vector<int>& d,
                         const std::vector<std::vector<int>>& c,
                         std::vector<bool>& visitado,
                         int& naoVisitados,
                         Rota& rota,
                         double alpha,
                         std::mt19937& rng) {

    rota.custo = 0;
    rota.caminho.push_back(0); 
    vector<int> cand_viaveis;

    int atual = 0;

    while (true) {

        cand_viaveis.clear();

        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) {
                rota.caminho.push_back(i);

                if(validaRota(rota,d, Q)){
                    cand_viaveis.push_back(i);
                }

                rota.caminho.pop_back();
            }
        }

        if (cand_viaveis.empty())
         break;

        int prox = escolherGRASP(cand_viaveis, atual, c, alpha, rng);
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;
        atual = prox;
    }

    if (rota.caminho.size() > 1) {
        rota.caminho.push_back(0);
        rota.custo += c[atual][0];
        return true;
    }
    return false;
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
        Rota rota;
        if (construirRota_GRASP(n, Q, d, c, visitado, naoVisitados, rota, alpha, rng)) {
            res.custoFinal += rota.custo;
            res.rotas.push_back(rota);
        } 
    }
    return res;
}


Resultado GRASP(int n, int m, int Q,const std::vector<int>& d,const std::vector<std::vector<int>>& c,double alpha,int qtd_iteracoes,std::mt19937& rng){
    
    Resultado melhor;
    melhor.custoFinal = std::numeric_limits<int>::max();

    for (int i = 0; i < qtd_iteracoes; ++i) {
        // Construção GRASP
        std::cout << "Melhor solucao: " << melhor.custoFinal << '\n';
        Resultado sol = guloso_GRASP(n, m, Q, d, c, alpha, rng);

        std::cout << "Solucao Inicial Grasp: ";
        imprimirResultado(sol);
        std::cout << '\n';

        // Busca local
        VND(sol, d, c, Q);

        std::cout << "Solucao Vnd pos Grasp: ";
        imprimirResultado(sol);
        std::cout << '\n';

        // Atualiza melhor solução
        if (sol.custoFinal < melhor.custoFinal) {
            melhor = std::move(sol); 
        }
    }

    return melhor;
}
