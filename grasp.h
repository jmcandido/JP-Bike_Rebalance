
#include "resultados.h"
#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include <climits>

using namespace std;

Resultado guloso_GRASP(int n, int m, int Q,const vector<int>& d,const vector<vector<int>>& c, double alpha, std::mt19937& rng);

bool construirRota_GRASP(int n, int Q,
                         const std::vector<int>& d,
                         const std::vector<std::vector<int>>& c,
                         std::vector<bool>& visitado,
                         int& naoVisitados,
                         Rota& rota,
                         double alpha,
                         std::mt19937& rng);


Resultado guloso_GRASP(int n, int m, int Q,
                       const std::vector<int>& d,
                       const std::vector<std::vector<int>>& c,
                       double alpha,
                       std::mt19937& rng);


Resultado GRASP(int n, int m, int Q,
                             const std::vector<int>& d,
                             const std::vector<std::vector<int>>& c,
                             double alpha,
                             int qtd_iteracoes,
                             std::mt19937& rng);