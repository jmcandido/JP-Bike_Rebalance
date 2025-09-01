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
                   const vector<vector<int>>& c, vector<bool>& visitado, int &naoVisitados, Rota &rota) {
    
    rota.custo = 0;
    rota.caminho.push_back(0);
    int atual = 0;
    vector<int> candidatos;
    int prox;
    
    while (true) {
    // monta candidatos
    candidatos.clear();

    for (int i = 1; i <= n; i++){
        if (!visitado[i]) 
            candidatos.push_back(i);
    } 

    if (candidatos.empty()) 
        break;

        prox = escolherVizinhoMaisProximo(atual, candidatos, c);

        rota.caminho.push_back(prox);
        if (validaRota(rota, d, Q)) {
            rota.custo += c[atual][prox];
            visitado[prox] = true;
            naoVisitados--;
            atual = prox;
        } else {
            rota.caminho.pop_back();
            break;
        }
    }

    // fecha rota no fim se tiver cliente
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
        cout << "[AVISO] " << naoVisitados 
             << " clientes não foram atendidos pelo guloso." << endl;
        
        // opcional: listar quais clientes ficaram de fora
        cout << "Clientes não atendidos: ";
        for (int i = 1; i <= n; i++) {
            if (!visitado[i]) cout << i << " ";
        }
        cout << endl;
    }

    return res;
}

