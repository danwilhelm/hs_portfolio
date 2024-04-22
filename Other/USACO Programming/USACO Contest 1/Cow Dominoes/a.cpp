/*
PROG: cowdom
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *filein, *fileout;

int QSortDominoes(const DOMINO *dom1, const DOMINO *dom2);


int main(void)
{
	int i;
	
	filein  = fopen("cowdom.in", "r");
	fileout = fopen("cowdom.out", "w");

	fscanf(filein, "%i", &ndominoes);

	for(i=0;i<ndominoes;i++)
		fscanf(filein, "%i %i", &domino[i].num1, &domino[i].num2);

	
	qsort(domino, ndominoes, sizeof(DOMINO), (int(*)(const void*,const void*))QSortDominoes);
	fprintf(fileout, "%i\n", sum1+sum2);

	fclose(filein), fclose(fileout);
}


int QSortDominoes(const DOMINO *dom1, const DOMINO *dom2)
{
	if(dom1->num1 + dom1->num2 > dom2->num1 + dom2->num2)
		return -1;
	else
		return 1;
}
