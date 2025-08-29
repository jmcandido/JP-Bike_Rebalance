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

Rota construirRota(int n, int Q, const vector<int>& d,
                   const vector<vector<int>>& c, vector<char>& visitado) {
    Rota rota;
    rota.caminho.push_back(0);

    int faltamEntregas = 0;
    for (int i = 1; i <= n; i++) {
        if (!visitado[i] && d[i] < 0) 
            faltamEntregas += -d[i];
    }

    int carga = (faltamEntregas > 0 ? min(Q, faltamEntregas) : 0);
    int atual = 0;
    vector<int> candidatos;

    while (true) {
        candidatos.clear();
        for (int i = 1; i <= n; i++) {
            if (visitado[i]) continue;
            if (d[i] < 0 && carga >= -d[i]) 
                candidatos.push_back(i);
            else if (d[i] > 0 && carga + d[i] <= Q) 
                candidatos.push_back(i);
        }

        if (candidatos.empty()) {
            rota.caminho.push_back(0);
            rota.custo += c[atual][0];
            break;
        }

        int prox = escolherVizinhoMaisProximo(atual, candidatos, c);
        rota.caminho.push_back(prox);
        rota.custo += c[atual][prox];
        carga += d[prox];
        visitado[prox] = 1;
        atual = prox;
    }

    return rota;
}

Resultado guloso(int n, int m, int Q, const vector<int>& d,
                 const vector<vector<int>>& c) {
    Resultado res;
    vector<char> visitado(n + 1, 0);

    for (int k = 0; k < m; k++) {
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) { todasVisitadas = false; break; }
        }
        if (todasVisitadas) break;

        Rota rota = construirRota(n, Q, d, c, visitado);
        res.rotas.push_back(rota);
        res.custoFinal += rota.custo;
    }

    return res;
}
