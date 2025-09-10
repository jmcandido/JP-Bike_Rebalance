#ifndef GULOSO_H
#define GULOSO_H

#include "resultados.h"

int escolherVizinhoMaisProximo(int atual, 
                               const std::vector<int>& candidatos,
                               const std::vector<std::vector<int>>& c);

Rota construirRota(int n, int Q, const vector<int>& d,
                   const vector<vector<int>>& c, 
                   std::vector<bool>& visitado, int &naoVisitados);

Resultado guloso(int n, int m, int Q, 
                 const vector<int>& d,
                 const vector<vector<int>>& c);

#endif
