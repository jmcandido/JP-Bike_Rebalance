JP-Bike Rebalance 

Solucionador heurístico para o rebalanceamento de sistemas de bicicletas compartilhadas (variante de VRP com coleta/entrega em um único depósito), desenvolvido como projeto da disciplina Análise e Projeto de Algoritmos (UFPB/CI).

Cidades com bike-sharing precisam manter as estações balanceadas ao longo do dia. Nosso programa planeja rotas para uma frota de veículos que coletam/entregam bicicletas entre as estações, respeitando capacidade e minimizando o custo total percorrido. O modelo considera:

Um depósito (vértice 0) e n estações (1..n).

Demandas q_i: positivas (doam bikes) e negativas (recebem bikes).

m veículos idênticos com capacidade Q.

Custo c_ij em cada arco (distância).

Cada estação é visitada exatamente uma vez; veículos podem sair e retornar carregados; a carga no veículo nunca pode ser negativa nem exceder Q.


