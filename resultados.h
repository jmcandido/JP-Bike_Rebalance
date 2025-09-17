#ifndef RESULTADOS_H
#define RESULTADOS_H

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <climits>


using namespace std;

struct Rota {
   vector<int> caminho;
   int custo = 0;
};

struct Resultado {
    int custoFinal = 0;
    vector<Rota> rotas;
};

bool lerInstancia(const string& nomeArquivo, int &n, int &m, int &Q,  vector<int>& d, vector<vector<int>>& c);

int custoRota(const Rota& r, const vector<vector<int>>& c);

bool validaRota(const Rota& r, const vector<int>& d, const int Q);

void imprimirResultado(const Resultado& res);

void gravaResultado(const string& pasta, const string& instancia, const Resultado& res);

#endif




