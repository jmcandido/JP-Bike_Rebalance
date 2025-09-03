#include "resultados.h"
#include <fstream>

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

    out << res.custoFinal << endl;
    out << res.rotas.size() << endl;
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
    for (int i = 0; i + 1 < r.caminho.size(); i++) {
        custo += c[r.caminho[i]][r.caminho[i+1]];
    }
    return custo;
}

bool validaRota(const Rota& r, const vector<int>& d, int Q) {
    int demandas_acumuladas = 0;
    int min_demanda = 0;
    int max_demanda = 0;

    for (int valor : r.caminho) {
        demandas_acumuladas += d[valor];
        min_demanda = min(min_demanda, demandas_acumuladas);
        max_demanda = max(max_demanda, demandas_acumuladas);
    }


    int minima_viavel = -min_demanda;       // carga mínima necessária para não ficar negativa
    int maxima_viavel = Q - max_demanda;    // carga máxima permitida para não estourar Q

    return (minima_viavel <= maxima_viavel);  
    
    // existe pelo menos um valor inicial viável
}

// Imprime solução
void imprimirResultado(const Resultado& res) {
    cout << res.custoFinal << endl;
    cout << res.rotas.size() << endl;
    for (const Rota &r : res.rotas) {
        for (int i = 0; i < (int)r.caminho.size(); i++) {
            cout << r.caminho[i] << (i+1 < r.caminho.size() ? ' ' : '\n');
        }
    }
}
