
#include "resultados.h"
using namespace std;

int escolherGRASP(const std::vector<int>& viaveis,int atual,const std::vector<std::vector<int>>& c,double alpha,int, int);

Rota construirRota_GRASP(int n, int Q,
                         const std::vector<int>& d,
                         const std::vector<std::vector<int>>& c,
                         std::vector<bool>& visitado,
                         int& naoVisitados,
                         double alpha);


Resultado guloso_GRASP(int n, int m, int Q,
                       const std::vector<int>& d,
                       const std::vector<std::vector<int>>& c,
                       double alpha);


Resultado GRASP(int n, int m, int Q,
                             const std::vector<int>& d,
                             const std::vector<std::vector<int>>& c,
                             double alpha,
                             int qtd_iteracoes);