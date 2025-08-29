#ifndef GULOSO_H
#define GULOSO_H

#include "utilitarios.h"

int escolherVizinhoMaisProximo(int atual, 
                               const std::vector<int>& candidatos,
                               const std::vector<std::vector<int>>& c);

Rota construirRota(int n, int Q, const std::vector<int>& d,
                   const std::vector<std::vector<int>>& c, 
                   std::vector<char>& visitado);

Resultado guloso(int n, int m, int Q, 
                 const std::vector<int>& d,
                 const std::vector<std::vector<int>>& c);

#endif
