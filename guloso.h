#ifndef GULOSO_H
#define GULOSO_H

#include "utilitarios.h"

int escolherVizinhoMaisProximo(int atual, 
                               const std::vector<int>& candidatos,
                               const std::vector<std::vector<int>>& c);

bool construirRota(int n, int Q, const std::vector<int>& d,
                   const std::vector<std::vector<int>>& c, 
                   std::vector<bool>& visitado, int &naoVisitados, Rota &rota);

Resultado guloso(int n, int m, int Q, 
                 const std::vector<int>& d,
                 const std::vector<std::vector<int>>& c);

#endif
