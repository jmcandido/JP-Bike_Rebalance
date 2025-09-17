
#include "resultados.h"

int escolherGRASP(const vector<int>& viaveis, int atual, const vector<vector<int>>& c,double alpha,int, int);

Rota construirRota_GRASP(const int n, const int Q,
                         const vector<int>& d,
                         const vector<vector<int>>& c,
                         vector<bool>& visitado,
                         int& naoVisitados,
                         double alpha);


Resultado guloso_GRASP(const int n, const int m, const int Q,
                       const vector<int>& d,
                       const vector<vector<int>>& c,
                       double alpha);


Resultado GRASP(const int n, const int m, const int Q,
                             const vector<int>& d,
                             const vector<vector<int>>& c,
                             double alpha,
                             int qtd_iteracoes);