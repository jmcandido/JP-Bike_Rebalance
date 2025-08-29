#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "utilitarios.h"

using namespace std;

// Escolhe cliente mais próximo entre candidatos
int escolherVizinhoMaisProximo(int atual, const vector<int>& candidatos,const vector<vector<int>>& c) {
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

// Constroi uma rota gulosa
Rota construirRota(int n, int Q, const vector<int>& d,const vector<vector<int>>& c, vector<char>& visitado) {
    Rota rota;
    rota.caminho.push_back(0);

    // calcula entregas e coletas restantes
    int faltamEntregas = 0;
    for (int i = 1; i <= n; i++) {
        if (!visitado[i] && d[i] < 0) 
            faltamEntregas += -d[i];
    }

    // define carga inicial

    int carga = (faltamEntregas > 0 ? min(Q, faltamEntregas) : 0);

    if (faltamEntregas > 0){
         carga = min(Q, faltamEntregas);
    }else{
        carga = 0;
    }

    int atual = 0;
    int proximo;

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

        proximo = escolherVizinhoMaisProximo(atual, candidatos, c);
        rota.caminho.push_back(proximo);
        rota.custo += c[atual][proximo];
        carga += d[proximo];
        visitado[proximo] = 1;
        atual = proximo;
    }

    return rota;
}

Resultado guloso(int n, int m, int Q, const vector<int>& d, const vector<vector<int>>& c) {
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

    // checa se todos foram atendidos
    for (int i = 1; i <= n; i++) {
        if (!visitado[i]) {
            cerr << "Erro: cliente " << i << " não foi atendido!\n";
            exit(1);
        }
    }

    return res;
}


int main() {
    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    if (!lerInstancia("instancia.txt", n, m, Q, d, c)) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    Resultado res = guloso(n, m, Q, d, c);
    imprimirResultado(res);

    return 0;
}
