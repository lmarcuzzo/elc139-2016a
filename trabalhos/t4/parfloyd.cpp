// g++ -o parfloyd parfloyd.cpp -fopenmp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

class Floyd
{
   public:
   
      Floyd() : 
         n(0), 
         mat(NULL)
      { }
      
      bool operator==(const Floyd& f)
      {
         if (n != f.n)
            return false;
            
         for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
               if (mat[i][j] != f.mat[i][j])
                  return false;

         return true;
      }
      
      void print()
      {
         for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
               printf("%4d", mat[i][j]);
            printf("\n");
         }
         printf("\n");
      }

      void generate(const int size, const int d, const int mw)
      {
         n = size;
         mat = new int*[n];
         for (int i = 0; i < n; i++)
            mat[i] = new int[n];
      
         srand(n);
   
         for (int i = 0; i < n; i++)
         {
            for (int j = 0; j < n; j++)
            {
               if (i == j)
                  mat[i][j] = 0;
               else if (rand() % (100/d) > 0)
                  mat[i][j] = 0;
               else
                  mat[i][j]= rand() % mw + 1;
            }
         }
      }

      void seqCalculate() {

         int i, j, k;

         for (k = 0; k < n; k++)
         {
            for (i = 0; i < n; i++)
            {
               for (j = 0; j < n; j++)
               {
                  if ((i != j) && (mat[i][k] * mat[k][j] != 0))
                     if ((mat[i][j] == 0) || (mat[i][k] + mat[k][j] < mat[i][j]))
                        mat[i][j] = mat[i][k] + mat[k][j];
               }
            }
         }
      }

      void parCalculate() {

         int i, j, k;

         for (k = 0; k < n; k++)
         {
            #pragma omp parallel for private(i,j)
            for (i = 0; i < n; i++)
            {
               for (j = 0; j < n; j++)
               {
                  if ((i != j) && (mat[i][k] * mat[k][j] != 0))
                     if ((mat[i][j] == 0) || (mat[i][k] + mat[k][j] < mat[i][j]))
                        mat[i][j] = mat[i][k] + mat[k][j];
               }
            }
         }
      }
      
   private:
      int n;                       // número de nós do grafo
      int** mat;                   // matriz de adjacência
      static const int noEdge = 0; // valor que indica ausência de aresta
};


int main(int argc, char** argv)
{
   const int MaxWeight = 10;
   const int Density = 80;
   
   if (argc != 2)
   {
      std::cout << "Uso: " << argv[0] << " <tamanho do grafo>" << std::endl;
      return 1;
   }
   
   int size = atoi(argv[1]);
   
   if (size <= 0)
   {
      std::cout << "Erro: tamanho do grafo deve ser maior que zero" << std::endl;
      return 1;
   }
   
   try {
      double time;
      Floyd f1;
         
      time = omp_get_wtime();
      f1.generate(size, Density, MaxWeight);
      time = omp_get_wtime() - time;
      printf("Time (generate): %12f\n", time);

      time = omp_get_wtime();
      f1.seqCalculate();
      time = omp_get_wtime() - time;
      printf("Time (seq): %12f\n", time);
      
      Floyd f2;
      time = omp_get_wtime();
      f2.generate(size, Density, MaxWeight);
      time = omp_get_wtime() - time;
      printf("Time (generate): %12f\n", time);

      time = omp_get_wtime();
      f2.parCalculate();
      time = omp_get_wtime() - time;
      printf("Time (par): %12f\n", time);
      
      if (f1 == f2)
         printf("Resultados iguais\n");
      else
         printf("Resultados diferentes\n");
         
   }
   catch (std::bad_alloc)
   {  
      std::cerr << "Erro: impossivel alocar memoria" << std::endl;
      return 1;
   }
   
   return 0;
}

