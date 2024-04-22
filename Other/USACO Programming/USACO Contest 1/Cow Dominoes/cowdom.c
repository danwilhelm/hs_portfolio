/*
PROG: cowdom
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct stDOMINO  {
	int num1;
	int num2;
} DOMINO;

int ndominoes;
DOMINO domino[40];
FILE *filein, *fileout;

int QSortDominoes(const DOMINO *dom1, const DOMINO *dom2);
int power(int a, int b);

int main(void)
{
	int i, sum1 = 0, sum2 = 0;
	
	filein  = fopen("cowdom.in", "r");
	fileout = fopen("cowdom.out", "w");

	fscanf(filein, "%i", &ndominoes);

	for(i=0;i<ndominoes;i++)
		fscanf(filein, "%i %i", &domino[i].num1, &domino[i].num2);

	
	qsort(domino, ndominoes, sizeof(DOMINO), (int(*)(const void*,const void*))QSortDominoes);

	//for(i=0;i<ndominoes;i++)
	//	printf("%i %i\n", domino[i].num1, domino[i].num2);

	for(i=0;i<ndominoes;i++)  {
		sum1 += (int)(power(10, (ndominoes-i-1)) * domino[i].num1);
		sum2 += (int)(power(10, (ndominoes-i-1)) * domino[i].num2);
		
		//printf("%i\n", (int)(pow(10, (ndominoes-i-1)) * domino[i].num1));
	}

	//printf("%i %i = %i", sum1, sum2, sum1+sum2);

	fprintf(fileout, "%i\n", sum1+sum2);

	fclose(filein), fclose(fileout);
}


int power(int a, int b)
{
	int product = 1, i;

	for(i=1;i<=b;i++)
		product *= a;
	
	return product;
}


int QSortDominoes(const DOMINO *dom1, const DOMINO *dom2)
{
	if(dom1->num1 + dom1->num2 > dom2->num1 + dom2->num2)
		return -1;
	else
		return 1;
}
