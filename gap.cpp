#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double heuristica, otimo;

    cout << "Digite o valor da heuristica: ";
    cin >> heuristica;
    cout << "Digite o valor do otimo: ";
    cin >> otimo;

    if (otimo == 0) {
        cout << "Erro: o valor otimo nao pode ser zero!" << endl;
        return 1;
    }

    double gap = ((heuristica - otimo) / otimo) * 100.0;

    cout << fixed << setprecision(2);
    cout << "GAP = " << gap << "%" << endl;

    return 0;
}
