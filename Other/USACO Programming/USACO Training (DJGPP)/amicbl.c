//### PROGRAM
/*
ID: mnansga001
PROG: poly1
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

int IsAmicable(int num);
int SumFactors(int num);

FILE *filein, *fileout;

int main(void)
{
    int min, max;
    int num, i;

	filein  = fopen("amicbl.in", "r");
	fileout = fopen("amicbl.out", "w");

	fscanf(filein, "%i %i", &min, &max);

    for(i=min;i<=max;i++)  {
    	num = IsAmicable(i);
        if(num)
        	fprintf(fileout, "%i ", num);
    }

    fclose(filein), fclose(fileout);
    return 0;
}


int IsAmicable(int num)
{
	int firstsum = SumFactors(num);

    if(num == SumFactors(firstsum))
		return num;

    return 0;
}

int SumFactors(int num)
{
    int i = 2;
    int mod, divfactor;
    int sumfactors = 1;
	int maxfactor  = sqrt(num);

    while(i <= maxfactor)
    {
        mod = num % i;

    	if(!mod)
        {
            divfactor = num / i;

        	sumfactors += i;
            if(divfactor != i)
	        	sumfactors += divfactor;

            //printf("%i\n%i\n", i, divfactor);
        }

    	i++;
    }

    //printf("\nSUM = %i, %i\n\n", sumfactors, maxfactor);

    return sumfactors;
}

