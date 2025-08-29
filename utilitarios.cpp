#include "utilitarios.h"
#include <fstream>
#include <iostream>
using namespace std;

// Lê instância do arquivo
bool lerInstancia(const string& nomeArquivo, int &n, int &m, int &Q, 
                  vector<int>& d, vector<vector<int>>& c) {
    ifstream in(nomeArquivo);
    if (!in) return false;

    in >> n >> m >> Q;

    d.assign(n + 1, 0);
    for (int i = 1; i <= n; i++) in >> d[i];

    c.assign(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            in >> c[i][j];

    return true;
}

// Recalcula o custo de uma rota
int custoRota(const Rota& r, const vector<vector<int>>& c) {
    int custo = 0;
    for (int i = 0; i + 1 < (int)r.caminho.size(); i++) {
        custo += c[r.caminho[i]][r.caminho[i+1]];
    }
    return custo;
}

// Valida se uma rota respeita capacidade
bool validaRota(const Rota& r, const vector<int>& d, int Q) {
    int carga = 0;
    for (int v : r.caminho) {
        carga += d[v];
        if (carga < 0 || carga > Q) 
        return false;
    }
    return true;
}

// Valida todas as rotas do resultado
bool validaResultado(const Resultado& res, const vector<int>& d, int Q) {
    for (auto &r : res.rotas) {
        if (!validaRota(r, d, Q))
         return false;
    }
    return true;
}

// Imprime solução
void imprimirResultado(const Resultado& res) {
    cout << res.custoFinal << "\n";
    cout << res.rotas.size() << "\n";
    for (auto &r : res.rotas) {
        for (int i = 0; i < (int)r.caminho.size(); i++) {
            cout << r.caminho[i] << (i+1 < (int)r.caminho.size() ? ' ' : '\n');
        }
    }
}
