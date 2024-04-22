/*
ID: mnansga001
PROG: barn1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct stSTALL
{
	int occupied;
	int boarded;
} STALL;

int CalcLongestRun(int begin, int end);
int DivideBoard(void);

int maxboards, nstalls, ncows;
int startboard, endboard;
STALL stall[200];
FILE *infile, *outfile;

int main(void)
{
	int i, tempstall, blocked = 0;
	
	memset(stall, 0, sizeof(STALL)*200);
	
	// open files
	infile  = fopen("barn1.in", "r");
	outfile = fopen("barn1.out", "w");


	// load stall data
	fscanf(infile, "%i %i %i", &maxboards, &nstalls, &ncows);

	for(i=0;i<ncows;i++)  {
		fscanf(infile, "%i", &tempstall);
		stall[tempstall-1].occupied = 1;
	}
	

	// make the first board placement 
	for(i=0;i<nstalls;i++)
		stall[i].boarded = 1;
	
	// find the first cow
	for(i=0;i<nstalls;i++)  {
		if(stall[i].occupied) 
			break;

		stall[i].boarded = 0;
	}

	startboard = i;

	// find the last cow
	for(i=nstalls-1;i>=0;i--)  {
		if(stall[i].occupied) 
			break;

		stall[i].boarded = 0;
	}


	endboard = i;

	//printf("%i %i", startboard, endboard);

	
	// divide the board maxboards times
	for(i=1;i<maxboards;i++)
		if(DivideBoard())
			break;

	// count the number of blocked cells
	for(i=0;i<nstalls;i++)
		if(stall[i].boarded)
			blocked++;


	printf("\n\n%i\n", blocked);

	// write to file
	fprintf(outfile, "%i\n", blocked);

	// close the files
	fclose(infile), fclose(outfile);
	
	return 0;
}


int DivideBoard(void)
{
	int i, startdivide;
	
	startdivide = CalcLongestRun(startboard, endboard);

	printf("%i\n", startdivide);

	for(i=startdivide;i<=endboard;i++)  {
		if(!stall[i].occupied)
			stall[i].boarded = 0;
		else
			break;
	}
	
	return 0;
}


int CalcLongestRun(int begin, int end)
{
	int i, longest = 0, longestindex = 0;
	int startrun = 0, run = 0;
	
	for(i=begin;i<=end;i++)  {
		if(stall[i].boarded)  {
			if(!stall[i].occupied)  {
				run++;
				if(!startrun) startrun = i;
			}
			else  {
				if(run > longest)  {
					longest = run;
					longestindex = startrun;
				}

				run = 0, startrun = 0;
			}
		}
	}

	return longestindex;
}
