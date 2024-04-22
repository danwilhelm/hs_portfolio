/*
PROG: pasture
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int pole[3000], npoles; 
FILE *filein, *fileout;


int main(void)
{
	int i, n, j, sum;
	int minsum = 0xFFFF, beginning = 0, end = 0;
	
	filein  = fopen("pasture.in", "r");
	fileout = fopen("pasture.out", "w");

	fscanf(filein, "%i", &npoles);

	for(i=0;i<npoles;i++)
		fscanf(filein, "%i", &pole[i]);

	for(i=0;i<npoles;i++)  {
		for(n=i+1;n<npoles;n++)  {

			sum = 0;
			for(j=i;j<=n;j++)
				sum += pole[j];

			if(abs(sum) < minsum)  {
				
				minsum = abs(sum);
				beginning = i;
				end = n;

				if(minsum == 0)
					goto Done;
			}			
		}
	}
	
Done:
	
	fprintf(fileout, "%i %i %i\n", beginning+1, end+1, minsum);

	fclose(filein), fclose(fileout);
}