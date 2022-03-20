/******************************************************************************
* FILE: omp_bug6.c
* DESCRIPTION:
*   This program compiles and runs fine, but produces the wrong result.
*   Compare to omp_orphan.c.
* AUTHOR: Blaise Barney  6/05
* LAST REVISED: 06/30/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

float a[VECLEN], b[VECLEN];

// The bug is caused by making the list item (sum) private by declaring sum
// again in the function dotprod inside the parallel region. The list item, sum,
// used in the reduction clause must be shared instead of private.
// To solve the bug, I move all parallelism into the function.

float dotprod ()
{
int i,tid;
float inner_sum;

#pragma omp parallel private(tid, i)
{
tid = omp_get_thread_num();
#pragma omp for reduction(+:inner_sum)
  for (i=0; i < VECLEN; i++)
    {
    inner_sum = inner_sum + (a[i]*b[i]);
    printf("  tid= %d i=%d\n",tid,i);
    }
}
return inner_sum;
}


int main (int argc, char *argv[]) {
int i;
float sum;

for (i=0; i < VECLEN; i++)
  a[i] = b[i] = 1.0 * i;
sum = 0.0;

sum = dotprod();

printf("Sum = %f\n",sum);

}
