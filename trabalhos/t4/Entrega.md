ELC 139 - Programaçao Paralela
Nome: Leonardo Marcuzzo

O código desenvolvido está em [floyd_openmp](floyd_openmp)

**Particionamento**\
O particionamento foi feito através dos laços **i** e **j**, já que o laço **k** apresenta dependências

**Comunicação**\
As variáveis **w** e **s** pertencentes aos grafos são compartilhadas entre as threads.
O laço **k** também é compartilhado por causa da dependência.

**Aglomeração e Mapeamento**
Os laços foram divididos em 2,4,6 e 8 threads.

**Graficos**
Tempo(s) x Threads
![](plot_tempo.png)

Speedup
![](speedup_floyd.png)

Nao tenho certeza se os resultados estao errados, pois na 3ª referencia eles apresentam resultados parecidos.

**Referências**\
[3.9 Case Study: Shortest-Path Algorithms](http://www.mcs.anl.gov/~itf/dbpp/text/node35.html)\
[CS 5220: Project 3 All-Pairs Shortest Path](http://jontse.com/courses/files/cornell/cs5220/project03_path.pdf)\
[Performance Analysis of Parallel Algorithm over Sequential using OpenMP](http://www.iosrjournals.org/iosr-jce/papers/Vol16-issue2/Version-10/J0162105862.pdf)