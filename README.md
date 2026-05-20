JP-Bike Rebalance

Solucionador heuristico para rebalanceamento de sistemas de bicicletas compartilhadas, uma variante de VRP com coleta/entrega em um unico deposito.

O programa planeja rotas para uma frota de veiculos que coletam e entregam bicicletas entre estacoes, respeitando capacidade e minimizando o custo total percorrido.

Modelo considerado:

- Vertice 0 como deposito e vertices 1..n como estacoes.
- Demandas q_i positivas para estacoes que doam bikes e negativas para estacoes que recebem bikes.
- m veiculos identicos com capacidade Q.
- Custo c_ij em cada arco.
- Cada estacao deve ser visitada exatamente uma vez.
- A carga do veiculo nunca pode ser negativa nem exceder Q.

## CLI

```bash
make programa
./programa instancias/n12_q20.txt 0.35 120
```

## Interface web

O projeto tambem possui uma camada web mantendo o backend em C++.

```bash
make servidor
./servidor 8080
```

Depois acesse `http://localhost:8080`.

A interface lista as instancias da pasta `instancias`, executa o GRASP/RVND no backend C++ e renderiza as rotas em um grafo no canvas. A visualizacao foi pensada para instancias pequenas, onde a leitura das rotas ainda e clara.
