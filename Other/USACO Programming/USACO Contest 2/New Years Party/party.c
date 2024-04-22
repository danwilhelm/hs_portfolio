/*
PROG: party
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *filein, *fileout;

#define min(a,b)  (((a) < (b)) ? (a) : (b))

typedef struct stAVAILDISHES  {
	int dishnum;
	int cowsavail;
} AVAILDISHES;

typedef struct stCOWCANCOOK  {
	int ndishes;
	int nused;
	
	int dish[100];
} COWCANCOOK;


AVAILDISHES availdishes[100];
COWCANCOOK  cowcook[200];
int maxfoodpertype[100];

int QSortLowNums(const AVAILDISHES *dish1, const AVAILDISHES *dish2);

int main(void)
{
	int i, j, k;
	int stopnum, currdish, dishesassigned;
	int maxdishes = 0, cowsbringingdish = 0;

	int ncows, maxdishespercow, foodtypes;
	
	filein  = fopen("party.in", "r");
	fileout = fopen("party.out", "w");

	
	/******* INPUT *******/
	
	fscanf(filein, "%i %i %i", &ncows, &maxdishespercow, &foodtypes);

	for(i=0;i<foodtypes;i++)
		fscanf(filein, "%i", &maxfoodpertype[i]);

	for(i=0;i<ncows;i++)  {
		fscanf(filein, "%i", &cowcook[i].ndishes);		
		
		for(j=0;j<cowcook[i].ndishes;j++)
			fscanf(filein, "%i", &cowcook[i].dish[j]);
	}

	
	/******* LOW-NUMS TABLE *******/

	for(i=0;i<foodtypes;i++)  {
		availdishes[i].dishnum   = i+1;
		availdishes[i].cowsavail = 0;
	}

	for(i=0;i<ncows;i++)
		for(j=0;j<cowcook[i].ndishes;j++)
			availdishes[cowcook[i].dish[j]-1].cowsavail++;

	qsort(availdishes, foodtypes, sizeof(AVAILDISHES), (int(*)(const void*,const void*))QSortLowNums);

	/*for(i=0;i<foodtypes;i++)
		printf("%i %i\n", availdishes[i].dishnum, availdishes[i].cowsavail);*/


	/******* ASSIGN DISHES *******/
	
	for(i=0;i<ncows;i++)
		cowcook[i].nused = 0;

	for(i=0;i<foodtypes;i++)  {
		
		stopnum			 = min(maxfoodpertype[availdishes[i].dishnum], availdishes[i].cowsavail);
		currdish		 = availdishes[i].dishnum;
		dishesassigned	 = 0;
		cowsbringingdish = 0;
		
		/*printf("\n\nCurrent Dish: %i\n", currdish);*/

		for(j=0;j<ncows;j++)  {
			
			for(k=0;k<cowcook[j].ndishes;k++)  {

				//printf("%i %i\t", k, cowcook[j].dish[k]);

				if(cowcook[j].dish[k] == currdish && cowcook[j].nused < maxdishespercow)  {

					cowcook[j].nused++;
					cowsbringingdish++;
					maxdishes++;

					/*printf("Cow %i\t", j, currdish);*/

					if(dishesassigned >= stopnum || cowsbringingdish >= maxfoodpertype[currdish-1])
						goto DoneFoodType;
				}
			}
		}
DoneFoodType:

		dishesassigned = 0;	// only for a statement after the label
	}


	/******* DISPLAY MAX DISHES BROUGHT *******/

	fprintf(fileout, "%i\n", maxdishes);

	fclose(filein), fclose(fileout);
}


int QSortLowNums(const AVAILDISHES *dish1, const AVAILDISHES *dish2)
{
	if(dish1->cowsavail < dish2->cowsavail)
		return -1;
	else
		return 1;
}
