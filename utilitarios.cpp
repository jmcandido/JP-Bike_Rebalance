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

    d.resize(n + 1);
    d[0] = 0;
    for (int i = 1; i <= n; i++) in >> d[i];

    c.resize(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            in >> c[i][j];

    return true;
}
// Grava solução em arquivo
bool gravarResultado(const string& nomeArquivo, const Resultado& res) {
    ofstream out(nomeArquivo);
    if (!out) return false;

    out << res.custoFinal << "\n";
    out << res.rotas.size() << "\n";
    for (auto &r : res.rotas) {
        for (int i = 0; i < (int)r.caminho.size(); i++) {
            out << r.caminho[i] << (i+1 < (int)r.caminho.size() ? ' ' : '\n');
        }
    }

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

bool validaRota(const Rota& r, const vector<int>& d, int Q) {
    int prefix = 0;
    int minPrefix = 0;
    int maxPrefix = 0;

    for (int v : r.caminho) {
        prefix += d[v];
        minPrefix = min(minPrefix, prefix);
        maxPrefix = max(maxPrefix, prefix);
    }

    int L = -minPrefix;       // carga mínima necessária para não ficar negativa
    int U = Q - maxPrefix;    // carga máxima permitida para não estourar Q

    return (L <= U);          // existe pelo menos um valor inicial viável
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
