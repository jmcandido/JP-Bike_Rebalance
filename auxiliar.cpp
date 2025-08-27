#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// calcula custo da rota
int calcularCusto(const vector<int>& caminho, const vector<vector<int>>& c) {
    int custo = 0;
    for (int i = 0; i < (int)caminho.size()-1; i++) {
        custo += c[caminho[i]][caminho[i+1]];
    }
    return custo;
}

// simula a carga e mostra passo a passo
bool simularCarga(const vector<int>& caminho, const vector<int>& d, int Q, int cargaInicial) {
    int carga = cargaInicial;
    cout << "\nSimulacao de carga:\n";
    for (int idx = 0; idx < (int)caminho.size(); idx++) {
        int v = caminho[idx];
        carga += d[v];
        cout << "Visita " << v << " | Demanda=" << d[v]
             << " | Carga=" << carga << endl;
        if (carga < 0 || carga > Q) {
            cout << "Falha: carga invalida em " << v << endl;
            return false;
        }
    }
    cout << "✅ Rota viavel!\n";
    return true;
}

int main() {
    ifstream in("instancia1000.txt");
    if (!in) {
        cout << "Erro ao abrir o arquivo!\n";
        return 1;
    }

    int n, m, Q;
    in >> n >> m >> Q;

    vector<int> d(n+1);
    d[0] = 0;
    for (int i = 1; i <= n; i++) in >> d[i];

    vector<vector<int>> c(n+1, vector<int>(n+1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            in >> c[i][j];

    cout << "Digite a rota (terminar com -1): ";
    vector<int> rota;
    while (true) {
        int x; cin >> x;
        if (x == -1) break;
        rota.push_back(x);
    }

    int custo = calcularCusto(rota, c);
    cout << "\n=== Resultado ===\n";
    cout << "Custo da rota: " << custo << endl;

    int cargaInicial;
    cout << "Digite a carga inicial do caminhao: ";
    cin >> cargaInicial;

    simularCarga(rota, d, Q, cargaInicial);

    return 0;
}
