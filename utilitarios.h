#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <vector>
#include <string>
#include <iostream>
using namespace std;

// Estruturas básicas
struct Rota {
   vector<int> caminho;
   int custo = 0;
};

struct Resultado {
    int custoFinal = 0;
    vector<Rota> rotas;
};



// Funções utilitárias
bool lerInstancia(const std::string& nomeArquivo, int &n, int &m, int &Q,  std::vector<int>& d, std::vector<std::vector<int>>& c);

int custoRota(const Rota& r, const std::vector<std::vector<int>>& c);

bool validaRota(const Rota& r, const std::vector<int>& d, int Q);

void imprimirResultado(const Resultado& res);

bool gravarResultado(const string& nomeArquivo, const Resultado& res);
#endif




