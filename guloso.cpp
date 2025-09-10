#include "guloso.h"
#include <iostream>
using namespace std;

int escolherVizinhoMaisProximo(int atual, const vector<int>& candidatos_viaveis,const vector<vector<int>>& c) {
    
    int melhor = candidatos_viaveis[0];
    int melhorCusto = c[atual][melhor];
    for (int j : candidatos_viaveis) {
        if (c[atual][j] < melhorCusto) {
            melhor = j;
            melhorCusto = c[atual][j];
        }
    }
    return melhor;
}

Rota construirRota(int n, int Q,
                         const vector<int>& d,
                         const vector<vector<int>>& c,
                         vector<bool>& visitado,
                         int& naoVisitados) {
    
    Rota rota;                        
    rota.custo = 0;
    rota.caminho.push_back(0); 

    vector<int> cand_viaveis;
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
               
               int novoMin = min(minCarga, novaCarga);
               int novoMax = max(maxCarga, novaCarga);

                if (-novoMin <= Q - novoMax) 
                    cand_viaveis.push_back(i);
            }
        }

        if (cand_viaveis.empty())
            break;

        // escolha via GRASP
        int prox = escolherVizinhoMaisProximo(atual,cand_viaveis,c);

        // adiciona cliente escolhido
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;

        // atualiza acumuladores incrementais
        cargaAtual += d[prox];
        minCarga = min(minCarga, cargaAtual);
        maxCarga = max(maxCarga, cargaAtual);
        atual = prox;
    }

    // fecha rota
    rota.caminho.push_back(0);
    rota.custo += c[atual][0];
    return rota;
}


Resultado guloso(int n, int m, int Q, const vector<int>& d,const vector<vector<int>>& c) {
                    
    Resultado res;
    
    vector<bool> visitado(n + 1, false);
    int naoVisitados = n;

    for (int i = 0; i < m && naoVisitados > 0; i++) {

        Rota rota = construirRota(n, Q, d, c, visitado, naoVisitados);
        res.rotas.push_back(rota);
        res.custoFinal += rota.custo;
        
    }

    if (naoVisitados > 0) {
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) cout << i << " ";
        }
        cout << endl;
    }
    
    return res;
}

