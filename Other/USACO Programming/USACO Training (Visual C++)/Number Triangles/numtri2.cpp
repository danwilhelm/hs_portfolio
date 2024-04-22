/*
ID: mnansga001
PROG: numtri
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int search(int row, int col);

int maxrows, largestsum = 0, sum = 0;
unsigned char numtri[1000][1000];
FILE *infile, *outfile;

int main(void)
{
	int col, row;
	
	// open the files
	infile  = fopen("numtri.in", "r");
	outfile = fopen("numtri.out", "w");

	// read in the data
	fscanf(infile, "%i", &maxrows);

	for(row=1;row<=maxrows;row++)
		for(col=0;col<row;col++)  {
			fscanf(infile, "%i", &numtri[col][row-1]);
			//printf("%i", numtri[col][row-1]);
		}

/*	for(row=0;row<maxrows;row++)  {
		for(col=0;col<=row;col++)
			printf("%i", numtri[col][row]);
		
		printf("\n");
	}*/

	search(0,0);	

	//printf("%i", largestsum);

	// print to the files
	fprintf(outfile, "%i\n", largestsum);

	// close the files
	fclose(infile), fclose(outfile);
	
	return 0;
}


int search(int row, int col)
{
	int i;

	if(row == maxrows)  {
		if(sum > largestsum) largestsum = sum;
		return 0;
	}

	//printf("(%i,%i)%i+%i\t", row, col, sum, numtri[row][i]);

	for(i=col;i<col+2;i++)  {
		sum += numtri[i][row];
		search(row+1, i);
		sum -= numtri[i][row];
	}

	return 0;
}