[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2016a) > Trabalhos

# T6: Ray-tracer paralelo com MPI

## Deadlines

Entregas parciais até as aulas dos dias 31/05 e 02/06. Entrega final até dia 06/06, 23:59.

## Problema

O método de [ray tracing](https://pt.wikipedia.org/wiki/Ray_tracing) é popularmente utilizado em computação gráfica para renderização de imagens 3D. Basicamente, este método consiste em traçar raios de luz partindo de um observador, passando por uma grade e atingindo os objetos que compõem uma cena. Cada elemento da grade corresponde a um pixel na imagem gerada, sendo que a cor de cada pixel é basicamente determinada pela cor do objeto interceptado pelo raio.

Os algoritmos de ray tracing geralmente requerem uma quantidade significativa de operações para a geração de cada imagem. Dependendo de alguns dados de entrada (tamanho da imagem, número de objetos na cena, etc.) e da plataforma de execução, o tempo de renderização pode ser da ordem de minutos ou até horas. Felizmente, o algoritmo oferece oportunidades de paralelização que, se bem exploradas, podem levar a uma redução do tempo de processamento.

## Objetivo

O objetivo deste trabalho é paralelizar um programa de síntese de imagem baseado em ray tracing, para uma arquitetura paralela com memória distribuída, usando [troca de mensagens com MPI](../../slides/slides-troca-de-mensagens-2016a.pdf).


## Desenvolvimento

- Existem várias implementações de algoritmos de ray tracing disponíveis na internet. Neste trabalho, vamos partir de uma implementação em C++ disponível [aqui](http://www.ffconsultancy.com/languages/ray_tracer/comparison.html). Há outra implementação semelhante a essa disponível [aqui](sphereflake-orig.cc), para gerar uma figura ligeiramente diferente. Ambos os programas produzem uma imagem no formato PPM (Portable PixMap).

- Para compilar e executar este programa:

   ```
   g++ -std=c++0x -o ray ray.cpp
   ./ray > fig.ppm
   ```

- Analise o desempenho do programa sequencial e identifique oportunidades de paralelização.

- Antes de partir para a implementação, projete a versão paralela do programa, considerando a arquitetura escolhida. Reflita sobre as possibilidades de particionamento, comunicação, aglomeração e mapeamento.

- Até a aula do dia 31/05, implemente um programa separado usando MPI, com o esquema de comunicação necessário para dividir o trabalho entre vários processos.

- Até a aula do dia 02/06, implemente o restante da paralelização, inserindo o esquema de comunicação no programa original, e fazendo os ajustes necessários.

- Até dia 06/06, 23:59, produza tabelas e gráficos com tempos de execução, speedup e eficiência do programa.



## Entrega


No seu repositório da disciplina, na pasta `trabalhos/t6`, crie um documento `Entrega.md`, contendo:
 - Identificação da disciplina e do aluno;
 - Link para os programas desenvolvidos;
 - Dados sobre a plataforma de execução;
 - Tabelas, gráficos e comentários de análise de desempenho.
 - Referências.
 
