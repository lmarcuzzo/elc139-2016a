#include <omp.h>
#include <stdio.h>
#define N 100000

int main() 
{
	int a[N], b[N], c[N];
	int i;

	for (i = 0; i < N; i++)
	{
		a[i] = b[i] = i; 
	}

  #pragma omp parallel for shared(a,b,c) private (i)
	for (i = 0; i < N; ++i)
	{
		c[i] = a[i] + b[i];
		printf("%d ", c[i]);
	}


  return 0;
}

