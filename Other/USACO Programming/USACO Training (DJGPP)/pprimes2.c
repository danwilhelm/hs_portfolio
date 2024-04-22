/*
ID: mnansga001
PROG: pprime
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int FindPalindromes(int digits, int npal);
int IsPrime(long number);

long palindrome[10000000];
long begin, end;
FILE *filein, *fileout;

char onesdigit[] = "1379";

int main(void)
{
    int i, npal = 0;

	filein  = fopen("pprime.in", "r");
    fileout = fopen("pprime.out", "w");

    fscanf(filein, "%i %i", &begin, &end);

	for(i=1;i<=8;i++)
		npal = FindPalindromes(i, npal);

    for(i=0;i<npal;i++)
		if(palindrome[i] >= begin && palindrome[i] <= end)
			if(IsPrime(palindrome[i]))
				fprintf(fileout, "%i\n", palindrome[i]);

    fclose(filein), fclose(fileout);
	return 0;
}

int FindPalindromes(int digits, int npal)
{
    long i, x, startindex;
    char j, tempnum[100];

	int padding    = floor((digits-2)/2);
	int paddigits  = floor(digits/2);

	if(digits == 1)
	{
		padding = 0;
		palindrome[npal++] = 5;
	}

	tempnum[digits] = '\0';
	for(x=0;x<pow(10,padding);x++)  {

		if(digits >= 2 && digits < 6)
			sprintf(&tempnum[1], "%i", x);
		else if(digits >= 6 && digits < 8)
			sprintf(&tempnum[1], "%.2i", x);
		else if(digits >= 8 && digits < 10)
			sprintf(&tempnum[1], "%.3i", x);

		for(i=0;i<padding;i++)
			tempnum[digits-i-2] = tempnum[i+1];

		for(i=0;i<4;i++)  {
			tempnum[0] = onesdigit[i];
			tempnum[digits-1] = tempnum[0];

			if(digits % 2 && digits > 1)
				for(j='0';j<='9';j++)  {
					tempnum[paddigits] = j;
					
					palindrome[npal++] = atoi(tempnum);
				}
			else
				palindrome[npal++] = atoi(tempnum);
		}
	}

	return npal;
}


int IsPrime(long number)
{
	long divide = 2, stop = floor(sqrt(number));

    while(divide <= stop)  {
    	if(number % divide == 0)
            return 0;
        divide++;
    }

    return 1;
}