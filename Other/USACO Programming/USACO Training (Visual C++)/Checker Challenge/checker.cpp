/*
ID: mnansga001
PROG: checker
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

void printboard(void);
void saveboard(void);
void search(int col);
int attacked(int row, int col);
int queenrow(int col);
void swap(int col1, int col2);

int SortBoards(const int *row1, const int *row2);

#define max(a,b)  (((a) > (b)) ? (a) : (b))

int boardsize, board[13][13];
int nsolutions = 0, solution[100][13];
FILE *infile, *outfile;

int main(void)
{
	int i, j;
	
	memset(board, 0, sizeof(int)*13*13);
	memset(solution, 0, sizeof(int)*100*13);
	
	// open the files
	infile  = fopen("checker.in", "r");
	outfile = fopen("checker.out", "w");

	// read in the data
	fscanf(infile, "%i", &boardsize);
	
	search(0);
	
/*	int c = 4, r = 0;
	board[2][2] = 1;
	board[c][r] = 2;
	printboard();
	printf("%i\n", attacked(r,c));*/
	
	//qsort(solution, nsolutions, sizeof(int)*7, (int(*)(const void*, const void*))SortBoards);

	for(i=0;i<nsolutions;i++)  {		
		for(j=0;j<boardsize;j++)
			printf("%i ", solution[i][j]);
		
		printf("\n");
	}
	
	printf("%i", nsolutions);
		
	// print to the files
	fprintf(outfile, "\n");

	// close the files
	fclose(infile), fclose(outfile);	

	return 0;
}

int SortBoards(const int *row1, const int *row2)
{
	int i;
	
	for(i=0;i<boardsize;i++)  {
		if(*(row1 + i) < *(row2 + i))
			return -1;
		else if(*(row2 + i) < *(row1 + i))
			return 1;
	}

	return 0;
}


void search(int col)
{
	int i, j, swaps;
	
/*	if(col == boardsize)  {
		saveboard();
		return;
	}

	for(i=0;i<boardsize;i++)  {
		if(!attacked(i, col))  {
			//printboard();
			
			board[col][i] = 1;
			search(col+1);
			board[col][i] = 0;
		}
	}*/

	for(i=0;i<boardsize;i++)
		board[i][rand()%boardsize] = 1;

	
	do  {
	swaps = 0;
	for(i=0;i<boardsize;i++)  {
		for(j=i+1;j<boardsize;j++)  {
			if(attacked(i,queenrow(i)) || attacked(j,queenrow(j)))  {
				swap(i,j);
				//printboard();
				if(attacked(i,queenrow(i)) || attacked(j,queenrow(j)))  {
					swap(i,j);
				}
				else  {
					swaps++;
				}
			}
		}
	}
	} while(swaps != 0);

	saveboard();

	return;
}


void swap(int col1, int col2)
{
	int oldcol1 = queenrow(col1), oldcol2 = queenrow(col2);
	
	board[col1][oldcol2] = 1;
	board[col2][oldcol1] = 1;

	board[col1][oldcol1] = 0;
	board[col2][oldcol2] = 0;

	return;
}


int queenrow(int col)
{
	int row;

	for(row=0;row<boardsize;row++)
		if(board[col][row] == 1)
			return row;

	return -1;
}


int attacked(int row, int col)
{
	int i;
	
	// check straight paths
	for(i=0;i<boardsize;i++)  {
		if(board[i][row] == 1)
			return 1;
	}


	// up + left
	i=0;
	while(col-i >= 0 && row-i >= 0)  {
		if(board[col-i][row-i] == 1)
			return 1;
		
		i++;
	}

	// down + left
	i=0;
	while(col-i >= 0 && row+i < boardsize)  {
		if(board[col-i][row+i] == 1)
			return 1;

		i++;
	}


	return 0;
}

void saveboard(void)
{
	int row, col;
	
	printboard();
	for(col=0;col<boardsize;col++)
		solution[nsolutions][col] = queenrow(col)+1;

	nsolutions++;
}

void printboard(void)
{
	int i;

	for(i=0;i<boardsize;i++)  {
		for(int j=0;j<boardsize;j++)  {
			if(board[j][i] == 0)
				printf("- ");
			if(board[j][i] == 1)
				printf("Q ");
		}
		
		printf("\n");
	}
	printf("\n");

	while(!_getch());
}