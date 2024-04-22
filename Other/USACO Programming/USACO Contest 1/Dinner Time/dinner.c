/*
PROG: dinner
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *filein, *fileout;

int main(void)
{
	int i, j, k, ncows, ncol;
	unsigned short cow[50000], tmpcow, choice;
	unsigned short row[50001][3], col[50000][3];
	unsigned short used[50001];
	unsigned short solution[50000];

	// Make used
	memset(used, 0, sizeof(unsigned short)*50000);
	
	filein  = fopen("dinner.in", "r");
	fileout = fopen("dinner.out", "w");

	// Get input
	fscanf(filein, "%i", &ncows);
	for(i=0;i<ncows;i++)
		fscanf(filein, "%i", &cow[i]);

	// Make cols
	memset(col, 0xFFFF, sizeof(unsigned short)*50000*3);
	for(i=0;i<ncows;i++)  {
		tmpcow = cow[i];

		col[i][1] = tmpcow;
		if(tmpcow-1 > 0) col[i][0] = tmpcow-1;
		if(tmpcow+1 <= ncows) col[i][2] = tmpcow+1;
	}
	
	// Make rows
	memset(row, 0xFFFF, sizeof(unsigned short)*50001*3);
	for(i=0;i<ncows;i++)  {
		for(j=-1;j<2;j++)  {
			tmpcow = cow[i]+j;

			for(k=0;k<3;k++)  {
				if(row[tmpcow][k] == 0xFFFF)  {
					row[tmpcow][k] = i;
					break;
				}
			}
		}
	}

	for(ncol=0;ncol<ncows;ncol++)  {
		choice = 0xFFFF;

		// check to see if digit is last time it can appear
		for(i=0;i<3;i++)  {
			if(!used[col[ncol][i]] && col[ncol][i] != 0xFFFF)  {

				if(row[col[ncol][i]][2] != 0xFFFF)  {
					if(row[col[ncol][i]][2] == ncol)  {
						printf("%");
						choice = col[ncol][i];
					}
				}
				else  {			
					//printf("%i (%i col), %i\t", col[ncol][i], ncol, row[col[ncol][i]][1]);
					if(row[col[ncol][i]][1] == ncol)  {
						printf("%");
						choice = col[ncol][i];
					}
				}
			}
		}

		// otherwise, get the lowest possible digit
		if(choice == 0xFFFF)  {
			for(i=0;i<3;i++)  {
				if(!used[col[ncol][i]] && col[ncol][i] != 0xFFFF)
					if(col[ncol][i] < choice)
						choice = col[ncol][i];
			}
		}

		solution[ncol] = choice;
		used[choice] = 1;
	}

/*	for(i=0;i<ncows;i++)
		printf("%i: %i %i %i\n", i, col[i][0], col[i][1], col[i][2]);

	printf("\n");
	for(i=1;i<=ncows;i++)
		printf("%i: %i %i %i\n", i, row[i][0], row[i][1], row[i][2]);

	printf("\n");
	for(i=0;i<ncows;i++)
		printf("%i ", solution[i]);*/
			

	for(i=0;i<ncows;i++)
		fprintf(fileout, "%i\n", solution[i]);

	fclose(filein), fclose(fileout);
}