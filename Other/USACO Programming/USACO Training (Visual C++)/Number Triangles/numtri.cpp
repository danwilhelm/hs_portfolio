/*
ID: mnansga001
PROG: numtri
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct stNUMTRI  {
	unsigned char num;
	short maxsum;
} NUMTRI;

int search(void);

#define GetIndex(row,col)	(int)((row*row+row)/2+col)
#define max(a,b)			(((a) > (b)) ? (a) : (b))

int maxrows, largestsum = 0;
NUMTRI tri[500500];
FILE *infile, *outfile;

int main(void)
{
	int i, temp;
	
	// open the files
	infile  = fopen("numtri.in", "r");
	outfile = fopen("numtri.out", "w");

	// read in the data
	fscanf(infile, "%i", &maxrows);

	temp = (maxrows*maxrows+maxrows)/2;
	for(i=0;i<temp;i++)
		fscanf(infile, "%i", &tri[i]);

	largestsum = search();

/*	for(i=0;i<maxrows;i++)  {
		for(j=0;j<=i;j++)
			printf("%i(%i) ", tri[GetIndex(i,j)].num, tri[GetIndex(i,j)].maxsum);
		printf("\n");
	}*/

	//printf("%i", largestsum);

	// print to the files
	fprintf(outfile, "%i\n", largestsum);

	// close the files
	fclose(infile), fclose(outfile);
	
	return 0;
}


int search(void)
{
	int row, col, base;

	if(maxrows == 1)
		return tri[0].num;
	
	// calculate the highest sum possible for each tri spot
	tri[0].maxsum = tri[0].num;

	for(row=1;row<maxrows;row++)  {
		
		base = GetIndex(row, 0);
		tri[base].maxsum = tri[base-row].maxsum + tri[base].num;
		
		for(col=base+1;col<base+row;col++)  {
			tri[col].maxsum  = max(tri[col-row].maxsum, tri[col-row-1].maxsum);
			tri[col].maxsum += tri[col].num;
		}

		tri[col].maxsum = tri[col-row-1].maxsum + tri[col].num;
	}

	// find the largest # on the bottom row	
	for(col=base;col<base+maxrows;col++)  {
		//printf("%i ", tri[col].maxsum);

		if(tri[col].maxsum > largestsum)
			largestsum = tri[col].maxsum;
	}

	return largestsum;
}