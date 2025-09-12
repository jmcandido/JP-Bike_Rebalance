#include "resultados.h"


Resultado ILS(const std::vector<int>& d,
                        const std::vector<std::vector<int>>& c,
                        int Q,
                        const Resultado& res,
                        int iteracoes,
                        int parada,
                        std::mt19937& rng);


bool perturbarSwapIntra(Resultado& sol,
                        const std::vector<int>& d,
                        const std::vector<std::vector<int>>& c,
                        int Q,
                        std::mt19937& rng);


bool perturbarSwapInter(Resultado& sol,
                        const std::vector<int>& d,
                        const std::vector<std::vector<int>>& c,
                        int Q,
                        std::mt19937&);



bool perturbar(Resultado& s,
               const std::vector<int>& d,
               const std::vector<std::vector<int>>& c,
               int Q,
               std::mt19937& rng,
               int tentativas);
