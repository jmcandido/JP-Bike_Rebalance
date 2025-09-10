
#ifndef VND_H
#define VND_H

#include "resultados.h"

void VND(Resultado& sol, const vector<int>& d, const vector<vector<int>>& c, int Q);

bool aplicarSwapGeral(Resultado& solucao, const vector<int>& d,const vector<vector<int>>& c, int Q);

bool aplicarSwapInterRotasGeral(Resultado& solucao,const vector<int>& d,const vector<vector<int>>& c, int Q);

bool aplicarRelocateInter(Resultado& solucao,const vector<int>& d, const vector<vector<int>>& c,int Q);
bool aplicarRelocate(Resultado& solucao, const vector<int>& d, const vector<vector<int>>& c,int Q);

#endif
