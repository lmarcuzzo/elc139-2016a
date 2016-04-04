Nome: Leonardo Marcuzzo
Matrícula: 201130409

(a) O perfil é afetado pelas opções de configuração?
Sim, na chamada da função com parametros (3000000,10), a função de alocação dos vetores usou 18% do tempo de execução para alocar, enquanto que com valores maiores(300000000,90), a alocação usou apenas 3% do tempo de execução. Em ambos os casos, a função que usou o maior tempo foi a função dot_prod.

(b) Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
A função dot_prod poderia ser paralelizada, pois sempre é a função que ocupa mais tempo de processamento.