#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "resultados.h"
using namespace std;

void simularRota(const Rota& rota, const vector<int>& d, int Q, const vector<vector<int>>&c) {
    int demandas_acumuladas = 0;
    int min_demanda = 0;
    int max_demanda = 0;

    cout << "\n=== Simulando Rota " << rota.id << " ===\n";
    cout << "Caminho: ";
    for (int v : rota.caminho) cout << v << " ";
    cout << "\n";

    for (int valor : rota.caminho) {
        demandas_acumuladas += d[valor];
        min_demanda = min(min_demanda, demandas_acumuladas);
        max_demanda = max(max_demanda, demandas_acumuladas);

        cout << "Visitando " << valor
             << " | Demanda = " << d[valor]
             << " | Demanda acumulada = " << demandas_acumuladas << "\n";
    }

    int minima_viavel = -min_demanda;
    int maxima_viavel = Q - max_demanda;

    cout << "Intervalo viável de carga inicial: ["
         << minima_viavel << ", " << maxima_viavel << "]\n";

    if (minima_viavel <= maxima_viavel){
        cout << "Resultado: Rota " << rota.id << " É VIÁVEL\n";
        cout << "Custo da Rota = " << custoRota(rota,c) << endl;
    }  
    else
        cout << "Resultado: Rota " << rota.id << " NÃO é viável\n";

    cout << "=============================\n";
}


int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <caminho_da_instancia>\n";
        return 1;
    }

    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    string instancia = argv[1];
    if (!lerInstancia(instancia, n, m, Q, d, c)) {
        cerr << "Erro ao abrir instancia!\n";
        return 1;
    }

    cout << "Digite as rotas manualmente.\n";
    cout << "Use 0 para fechar uma rota e -1 para encerrar todas.\n";

    vector<Rota> rotas;
    Rota rota;
    rota.id = 0;
    rota.caminho.clear();
    rota.caminho.push_back(0); // começa do depósito

    while (true) {
        int cliente;
        if (!(cin >> cliente)) break;

        if (cliente == -1) {
            if (rota.caminho.size() > 1 && rota.caminho.back() != 0)
                rota.caminho.push_back(0); // fecha
            if (rota.caminho.size() >= 2) rotas.push_back(rota);
            break;
        } else if (cliente == 0) {
            if (rota.caminho.size() > 1 && rota.caminho.back() != 0)
                rota.caminho.push_back(0); // fecha
            if (rota.caminho.size() >= 2) rotas.push_back(rota);

            rota = Rota();
            rota.id = (int)rotas.size();
            rota.caminho.push_back(0); // nova rota
        } else {
            rota.caminho.push_back(cliente);
        }
    }

    // Garante fechamento da última rota, se o usuário não digitar 0 ou -1 ao final
    if (!rotas.empty() && rotas.back().caminho.back() != 0) {
        rotas.back().caminho.push_back(0);
    }

    int custoTotal = 0;

    for (const Rota& r : rotas) {
        simularRota(r, d, Q, c);
        custoTotal += custoRota(r, c);
    }

    cout << "\n=== Resumo Geral ===\n";
    cout << "Total de rotas: " << rotas.size() << "\n";
    cout << "Custo total das rotas: " << custoTotal << "\n";

return 0;

}
