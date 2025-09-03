#include "guloso.h"
#include <iostream>
using namespace std;

int escolherVizinhoMaisProximo(int atual, 
                               const vector<int>& candidatos,
                               const vector<vector<int>>& c) {
    
    int melhor = candidatos[0];
    int melhorCusto = c[atual][melhor];
    for (int j : candidatos) {
        if (c[atual][j] < melhorCusto) {
            melhor = j;
            melhorCusto = c[atual][j];
        }
    }
    return melhor;
}

bool construirRota(int n, int Q, const vector<int>& d,
                   const vector<vector<int>>& c, vector<bool>& visitado,
                   int &naoVisitados, Rota &rota) {
    
    rota.custo = 0;
    rota.caminho.push_back(0); // começa no depósito
    vector<int> candidatos_viaveis;
    int atual = 0;

    while (true) {
       
        candidatos_viaveis.clear();

        // monta lista de candidatos viáveis
        for (int i = 1; i <= n; i++) {
            if (visitado[i]) 
                continue;

            rota.caminho.push_back(i);

            if(validaRota(rota, d, Q))
                candidatos_viaveis.push_back(i);
    
            rota.caminho.pop_back();
        }

        if (candidatos_viaveis.empty())
             break;

        // escolhe o vizinho mais próximo entre os viáveis
        int prox = escolherVizinhoMaisProximo(atual, candidatos_viaveis, c);

        // adiciona o cliente escolhido
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        visitado[prox] = true;
        naoVisitados--;
        atual = prox;
    }

    // fecha rota no depósito, se tiver clientes
    if (rota.caminho.size() > 1) {
        rota.caminho.push_back(0);
        rota.custo += c[atual][0];
        return true;
    }

    return false;
}


Resultado guloso(int n, int m, int Q, const vector<int>& d,
                 const vector<vector<int>>& c) {
                    
    Resultado res;
    
    vector<bool> visitado(n + 1, false);
    int naoVisitados = n;

    for (int i = 0; i < m && naoVisitados > 0; i++) {
        Rota rota;

        if (construirRota(n, Q, d, c, visitado, naoVisitados, rota)) {
            res.rotas.push_back(rota);
            res.custoFinal += rota.custo;
        }
    }

    if (naoVisitados > 0) {
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) cout << i << " ";
        }
        cout << endl;
    }
    
    return res;
}

