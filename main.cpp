#include <iostream>
#include <vector>
#include <fstream>
using namespace std;



void exibirDados(int n, int m, int Q, 
                  const vector<int>& d, 
                  const vector<vector<int> >& c){
    cout << "n = " << n << endl;
    cout << "m = " << m << endl;
    cout << "Q = " << Q << endl;

    cout << "\nArray d:\n";
    for (int i = 0; i < n; i++) {
        cout << d[i] << " ";
    }
    cout << endl;

    cout << "\nMatriz c:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }

}


int main() {
    ifstream in("instancia.txt"); // arquivo de entrada
    if (!in) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    int n, m, Q;
    in >> n >> m >> Q;

    vector<int> d(n);
    for (int i = 0; i < n; i++) {
        in >> d[i];
    }

    vector< vector<int> > c(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> c[i][j];
        }
    }

    // Exibir dados lidos
    
    exibirDados(n,m,Q,d,c);


    return 0;
}
