#include "grasp.h"
#include "vnd.h"


// Escolhe via LCR usando somente índices de clientes
int escolherGRASP(const std::vector<int>& viaveis,int atual,const std::vector<std::vector<int>>& c,double alpha,std::mt19937& rng, int gmin, int gmax) {

    // int gmin = INT_MAX;  // “+infinito” para começar
    // int gmax = INT_MIN;  // “-infinito” para começar
    // int g;

    // for (int v : viaveis) {
    //     g = c[atual][v];       
    //     gmin = min(gmin, g);
    //     gmax = max(gmax, g);
    // }

    double limite = gmin + alpha * (gmax - gmin);
    cout << "gmin = " << gmin << endl;
    cout << "gmax = " << gmax << endl;
    cout << "alpha = " << alpha << endl;
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

    uniform_int_distribution<int> dist(0, LCR.size() - 1);

    int proximo = LCR[dist(rng)];

    
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

    int cargaAtual = 0;
    int carga_minima = 0;
    int carga_maxima = 0;
    int g;
    

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

                if (-novoMin <= Q - novoMax) {
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
        int prox = escolherGRASP(cand_viaveis, atual, c, alpha, rng, gmin,gmax);

        // adiciona cliente escolhido
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;

        // atualiza acumuladores incrementais
        cargaAtual += d[prox];
        carga_minima = std::min(carga_minima, cargaAtual);
        carga_maxima = std::max(carga_maxima, cargaAtual);
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

        cout << "i = " << i + 1 << endl;
        // Construção GRASP
        // std::cout << "Melhor solucao: " << melhor.custoFinal << '\n';
        S = guloso_GRASP(n, m, Q, d, c, alpha, rng);

        cout << "Solucao Inicial Grasp: ";
        imprimirResultado(S);
        cout << endl;

        // Busca local
        VND(S, d, c, Q);

        std::cout << "Solucao Vnd pos Grasp: ";
        imprimirResultado(S);
        std::cout << '\n';

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
