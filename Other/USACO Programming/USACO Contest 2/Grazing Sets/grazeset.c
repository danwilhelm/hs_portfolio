/*
PROG: grazeset
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

FILE *filein, *fileout;

float cowangle[10000], fenceangle, middleangle;

int QSortAngles(const int *angle1, const int *angle2);


int main(void)
{
	int i, j, k;
	int ncows, nfences;
	int maxcows, mincows, minrange = 0xFFFF, currcow, cows;
	float begpos, endpos;
	
	filein  = fopen("grazeset.in", "r");
	fileout = fopen("grazeset.out", "w");

	
	/******* INPUT *******/
	
	fscanf(filein, "%i %i", &ncows, &nfences);

	for(i=0;i<ncows;i++)
		fscanf(filein, "%f", &cowangle[i]);

	qsort(cowangle, ncows, sizeof(float), (int(*)(const void*,const void*))QSortAngles);

	fenceangle = 360/(float)nfences;

	
	/*for(i=0;i<ncows;i++)
		printf("%f\n", cowangle[i]);*/
	
	
	/******* DETERMINE MAX/MIN # COWS PER FENCE POSITION *******/
	
	for(i=0;i<ncows-1;i++)  {
		
		cows = 0, maxcows = 0, mincows = 0xFFFF, currcow = 0;
		
		middleangle = (cowangle[i] + cowangle[i+1])/2;

		for(j=0;j<nfences;j++)  {
			
			for(k=0;k<ncows;k++)  {

				begpos = middleangle + j*fenceangle;
				endpos = begpos + fenceangle;

				if(endpos > 360)  {

					endpos -= 360;

					if(cowangle[k] < endpos || cowangle[k] > begpos)
						cows++;
				}
				else  {
				
					if(cowangle[k] > begpos && cowangle[k] < endpos)
						cows++;
				}
			}
										
			
			maxcows = max(cows, maxcows);
			mincows = min(cows, mincows);

			/*printf("%i %i %i %i\n", cows, maxcows, mincows, maxcows-mincows);*/
		}
		
		if(maxcows - mincows < minrange) 
			minrange = maxcows - mincows;

		/*printf("%i\n", minrange);*/
	}

	
	fprintf(fileout, "%i\n", minrange-1);

	fclose(filein), fclose(fileout);
}


int QSortAngles(const int *angle1, const int *angle2)
{
	if(angle1 > angle2)
		return -1;
	else
		return 1;
}
