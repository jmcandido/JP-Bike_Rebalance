
#ifndef VND_H
#define VND_H

#include "resultados.h"

bool aplicarSwap(Resultado& solucao, const vector<int>& d, const vector<std::vector<int>>& c,int Q);
bool aplicarSwap2(Resultado& solucao, const vector<int>& d, const vector<std::vector<int>>& c,int Q);

bool aplicarSwapInterRotas(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c,int Q);
bool aplicarSwapInterRotas2(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c,int Q);



#endif