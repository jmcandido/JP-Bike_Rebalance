
#ifndef VND_H
#define VND_H

#include "resultados.h"

void VND(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c, int Q);
void RVND(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c, int Q);

bool aplicarSwap(Resultado& solucao, const vector<int>& d,const vector<vector<int>>& c, int Q);
bool aplicarSwapInter(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c, int Q);

bool aplicarReinsertionInter(Resultado& solucao,const vector<int>& d, const vector<vector<int>>& c,int Q);
bool aplicarReinsertion(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c,int Q);

#endif
