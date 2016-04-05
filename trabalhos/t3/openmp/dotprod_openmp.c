#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */ 
void fill(double *a, int size, double value)
{  
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

int main(int argc, char **argv) 
{
	int nthreads, wsize, repeat, k, i;
	long start_time, end_time;
   
   	//Vetores e soma
	double *a, *b, c = 0.0;

	//Args
	if ((argc != 4)) {
		printf("Uso: %s <nthreads> <worksize> <repetitions>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

   	nthreads = atoi(argv[1]); 
   	wsize = atoi(argv[2]);  // worksize = tamanho do vetor de cada thread
   	repeat = atoi(argv[3]);

   	//Seta quantidade de threads para nthreads
   	omp_set_num_threads(nthreads);

   	//Aloca vetores a e b e preenche eles
	a = (double *) malloc(wsize*nthreads*sizeof(double));
	fill(a, wsize*nthreads, 0.01);
	
	b = (double *) malloc(wsize*nthreads*sizeof(double));
	fill(b, wsize*nthreads, 1.0);
	
	start_time = wtime();
	//Apenas o laço interno é dividido entre threads
	for (k = 0; k < repeat; k++) {
	  		c = 0.0;
    		#pragma omp parallel for private(i) shared (a,b) reduction(+:c)
    		for (i = 0; i < wsize*nthreads; i++)  {
        			c += (a[i] * b[i]);
        		}

    	}
	end_time = wtime();
   	
   	printf("%f\n", c);
   	printf("%d thread(s), %ld usec\n", nthreads, (long) (end_time - start_time));

  return 0;
}

