#ifndef RESULTADOS_H
#define RESULTADOS_H

#include <vector>
#include <string>
#include <iostream>
using namespace std;

// Estruturas básicas
struct Rota {
   vector<int> caminho;
   int custo = 0;
   int id;
};

struct Resultado {
    int custoFinal = 0;
    vector<Rota> rotas;
};

bool lerInstancia(const std::string& nomeArquivo, int &n, int &m, int &Q,  std::vector<int>& d, std::vector<std::vector<int>>& c);

int custoRota(const Rota& r, const std::vector<std::vector<int>>& c);

bool validaRota(const Rota& r, const std::vector<int>& d, int Q);

void imprimirResultado(const Resultado& res);

void gravaResultado(const string& pasta, const string& instancia, const Resultado& res);

#endif




