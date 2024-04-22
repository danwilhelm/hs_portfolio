/*
ID: mnansga001
PROG: pprime
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int FindPalindromes(int digits, int npal);
int IsPrime(long number);
int QSortPPal(const long *pal1, const long *pal2);

long palindrome[10000], primepal[1000];
long begin, end;
FILE *filein, *fileout;

char onesdigit[] = "1379";

int main(void)
{
    int i, npal = 0, pindex = 0;

	filein  = fopen("pprime.in", "r");
    fileout = fopen("pprime.out", "w");

    fscanf(filein, "%i %i", &begin, &end);

	// find all palindromes for each digit
	for(i=1;i<=8;i++)
		npal = FindPalindromes(i, npal);

    // find prime palindromes
	for(i=0;i<npal;i++)
		if(palindrome[i] >= begin && palindrome[i] <= end)
			if(IsPrime(palindrome[i]))
				primepal[pindex++] = palindrome[i];
    
	// ... numerical order
	qsort(primepal, pindex, sizeof(long), (int(*)(const void*,const void*))QSortPPal);
		
	// and to the file
	for(i=0;i<pindex;i++)
		fprintf(fileout, "%i\n", primepal[i]);

    fclose(filein), fclose(fileout);
	return 0;
}

int QSortPPal(const long *pal1, const long *pal2)
{
	if(*pal1 > *pal2)
		return 1;
	else
		return -1;
}

int FindPalindromes(int digits, int npal)
{
    long i, x;
    char j, tempnum[100];

/* a palindrome is comprised of i (an inside digit, 0-9),
   o (outside digit, 1,3,7,or 9 (for possible palindromic primes only)), 
   and a number xxx (an inside possibly multi-digited number, 0 to 10^(the # of xs)
   Referred to as 'padding' in the code)
   
	So the palindrome sequence (1-8 digits) goes something like this:
	(Note that the xs are actually 2 groups (split by a '/') where the second group
	is just a reverse of the first).
	i, o/o, oio, ox/xo, oxixo, oxx/xxo, oxxixxo, oxxx/xxxo*/
	
	int padding	= (int)((digits-2)/2);	// the number of digits of 'padding' (xs)
	int middle	= (int)(digits/2);		// the index of the middle 

	if(digits == 1)  {
		padding = 0;
		palindrome[npal++] = 5;		// must include 5 b/c it's not 1,3,7, or 9 :)
	}

	tempnum[digits] = '\0';
	for(x=0;x<pow(10,padding);x++)  {

		// ensures that single digits for padding are printed w/ 0s in front of them
		// this is the 'quick + dirty' way of doing it - only works if digits < 10
		if(digits >= 2 && digits < 6)		
			sprintf(&tempnum[1], "%i", x);
		else if(digits >= 6 && digits < 8)
			sprintf(&tempnum[1], "%.2i", x);
		else if(digits >= 8 && digits < 10)
			sprintf(&tempnum[1], "%.3i", x);

		// copy the padding # reversed to the other side of the palindrome
		for(i=0;i<padding;i++)
			tempnum[digits-i-2] = tempnum[i+1];

		// now insert the end numbers (1, 3, 7, or 9)
		for(i=0;i<4;i++)  {
			tempnum[0] = onesdigit[i];
			tempnum[digits-1] = tempnum[0];

			// for odd numbers, print and increment the center digit
			if(digits % 2 && digits > 1)
				for(j='0';j<='9';j++)  {
					tempnum[middle] = j;
					
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
	long divide = 2, stop = (long)sqrt(number);

    while(divide <= stop)  {
    	if(number % divide == 0)
            return 0;
        divide++;
    }

    return 1;
}