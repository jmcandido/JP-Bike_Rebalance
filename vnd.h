
#ifndef VND_H
#define VND_H

#include "utilitarios.h"

bool aplicarSwap(Resultado& sol, const vector<int>& d, const vector<std::vector<int>>& c,int Q);
bool aplicarRelocate(Resultado& solucao, const vector<int>& d,const vector<vector<int>>& c, int Q);
bool aplicarSwapInter(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c,int Q);

#endif