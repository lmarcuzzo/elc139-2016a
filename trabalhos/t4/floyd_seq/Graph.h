#ifndef _GRAPH_H
#define _GRAPH_H

/**
 Grafo orientado ac�clico ponderado.
 Os pesos das arestas s�o n�meros inteiros positivos maiores que zero.
 */
class Graph
{ 
   public:
      /**
        Constr�i grafo vazio.
        */
      Graph();
      
      /**
        Destrutor.
        */
      ~Graph();

      /**
        Operador de atribui��o.
        */
      Graph& operator=(const Graph& g);
      
      /**
        Gera um grafo de tamanho s, criando arestas aleatoriamente.
        @param[in]   s   Tamanho do grafo.
        @param[in]   d   Densidade da matriz de adjac�ncia (%)
        @param[in]   mw  Peso m�ximo de uma aresta.
        */
      void generate(const int s, const int d, const int mw);
                       
      /**
        Retorna o tamanho (n�mero de n�s) do grafo.
        */
      int size() const;
      
      /**
        Retorna o peso da aresta que parte do n� i e chega em j.
        */
      int getWeight(int i, int j) const;

      /**
        Define o peso da aresta entre os n�s i e j.
        */
      void setWeight(int i, int j, int w);
      
      /**
        Insere uma aresta entre os n�s i e j, com peso w.
        */
      void insertEdge(int i, int j, int w);
      
      /**
        Remove a aresta entre os n�s i e j.
        */
      void removeEdge(int i, int j);
      
      /**
        Determina se existe uma aresta partindo do n� i e chegando em j.
        */
      bool hasEdge(int i, int j) const;
      
      /**
        Salva o grafo em um arquivo.
        @param[in]   fname   Nome do arquivo.
        */
      int writeTo(char* fname);
      
      /**
        Carrega o grafo a partir de um arquivo. 
        CUIDADO: N�o faz nenhuma valida��o do formato do arquivo.
        @param[in]   fname   Nome do arquivo.
        */
      int readFrom(char* fname);
      
      /**
        Operador de sa�da em um stream.
        */
      friend std::ostream& operator<< (std::ostream& s, const Graph& g);
      
      /**
        Operador de entrada a partir de um stream.
        */
      friend std::istream& operator>> (std::istream& s, Graph& g);
               
   private:
      /**
        Gera ou n�o uma aresta entre i e j, considerando aa densidade desejada
        para a matriz de adjac�ncia.
        @param[in]   d   Densidade da matriz de adjac�ncia (%)
        @param[in]   mw  Peso m�ximo da aresta.
        */      
      void generateEdge(int i, int j, int d, int mw);
      
   private:
      int n;                       // n�mero de n�s do grafo
      int* mat;                    // matriz de adjac�ncia
      static const int noEdge = 0; // valor que indica aus�ncia de aresta
};

/**
 Codificador de um grafo para o formato DOT (ver GraphViz em http://www.graphviz.org/).
 */
class DotGraphEncoder
{
   private:
      const Graph& g;
   public:
      DotGraphEncoder(const Graph& g_);
      friend std::ostream& operator<< (std::ostream& s, const DotGraphEncoder& e);
};


#endif //_GRAPH_H
