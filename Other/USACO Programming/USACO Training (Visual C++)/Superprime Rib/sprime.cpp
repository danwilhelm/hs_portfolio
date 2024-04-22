/*
ID: mnansga001
PROG: sprime
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void search(int digits);
int IsPrime(int num);

int start = 0, end = 0, currp = 0;
int prime[10000], maxdigits;
int onesdigit[] = {1,3,7,9};

FILE *infile, *outfile;

int main(void)
{
	int i;
	
	// open the files
	infile  = fopen("sprime.in", "r");
	outfile = fopen("sprime.out", "w");

	// read in the data
	fscanf(infile, "%i", &maxdigits);

	// find the 1-digit primes
	for(i=2;i<10;i++)  {
		if(IsPrime(i))
			prime[currp++] = i;
	}

	start = 0, end = currp;

	//for(i=0;i<currp;i++)
	//	printf("%i\n", prime[i]);

	search(2);
	
	// print to the file
	for(i=start;i<end;i++)
		fprintf(outfile, "%i\n", prime[i]);
	
	// close the files
	fclose(infile), fclose(outfile);	

	return 0;
}


void search(int digits)
{
	int i, j, temp;

	if(digits == maxdigits+1)
		return;
	
	for(i=start;i<end;i++)  {
		temp = prime[i] * 10;
		for(j=0;j<4;j++)  {
			if(IsPrime(temp + onesdigit[j]))
				prime[currp++] = temp + onesdigit[j];
		}	
	}

	start = i, end = currp;

	search(digits+1);
}


int IsPrime(int num)
{
	int i;

	for(i=2;i*i<=num;i++)  {
		if(num % i == 0)
			return 0;
	}

	return 1;
}

