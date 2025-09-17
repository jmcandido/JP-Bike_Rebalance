#include "resultados.h"
#include "grasp.h"

int main(int argc, char *argv[]){
    
    if (argc < 4){
        cerr << "Uso: " << argv[0] << " instancia.txt alpha iteracoes" << endl;
        return -1;
    }

    string instancia = argv[1];

    double alpha = stod(argv[2]);
    int qtd_iteracoes = stoi(argv[3]);

    int n, m, Q;
    vector<int> d;
    vector<vector<int>> c;

    if (!lerInstancia(instancia, n, m, Q, d, c)){
        cerr << "Erro ao abrir instancia!\n";
        return -1;
    }

    auto inicio = chrono::high_resolution_clock::now();

    Resultado solucao = GRASP(n, m, Q, d, c, alpha, qtd_iteracoes);

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    imprimirResultado(solucao);

    //gravaResultado("resultados/guloso", instancia, solucao);
    //gravaResultado("resultados/vnd", instancia, solucao);
    //gravaResultado("resultados/grasp", instancia, solucao);

    
    cout << "\nTempo total de execução: " << duracao.count() << " ms" << endl;

    return 0;
}