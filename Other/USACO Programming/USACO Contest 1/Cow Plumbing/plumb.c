/*
ID: mnansga001
PROG: plumb
*/

#include <stdio.h>
#include <stdlib.h>

FILE *filein, *fileout;

int main(void)
{
	filein  = fopen("plumb.in", "r");
	fileout = fopen("plumb.out", "w");

	fscanf(filein, "");
	
	
	
	
	fprintf(fileout, "");

	fclose(filein), fclose(fileout);
}