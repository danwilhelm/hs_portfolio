//### PROGRAM
/*
ID: mnansga001
PROG: ride
*/

#include <stdio.h>
#include <ctype.h>

const char FILE_IN[]  = "ride.in";
const char FILE_OUT[] = "ride.out";
FILE *filein, *fileout;

int OpenFiles(void);
void CloseFiles(void);

#define MODNUM		47
#define MAX_STRCHRS	73			
#define ASCII_ALPHA	'A' + 1		

int main(void)
{
    long i;
	char str1[MAX_STRCHRS], str2[MAX_STRCHRS];
    long str1len = 1, str2len = 1;

	if(!OpenFiles())
		return 0;

    fscanf(filein, "%s", str1);
    fscanf(filein, "%s", str2);

    for(i=0;str1[i]!='\0';i++)		
    	if(isalpha(str1[i]))  {
    		str1len = (str1len * (str1[i] - ASCII_ALPHA)) % MODNUM;
        }

    for(i=0;str2[i]!='\0';i++)		
    	if(isalpha(str2[i]))  {
    		str2len = (str2len * (str2[i] - ASCII_ALPHA)) % MODNUM;
            printf("%i\t", str2[i] - ASCII_ALPHA);

                        }

    printf("%i %i", str1len, str2len);

    if(str1len == str2len)
    	fprintf(fileout, "GO\n");
    else
    	fprintf(fileout, "STAY\n");

    CloseFiles();
	return 0;
}

int OpenFiles(void)
{
    filein = fopen(FILE_IN, "r");
    if(!filein)  {
    	printf("Input file '%s' - not found!\n", FILE_IN);
        return 0;
    }

    fileout = fopen(FILE_OUT, "w");
	if(!fileout)  {
    	printf("Output file '%s' - error in opening!\n", FILE_OUT);
        return 0;
    }

 	return 1;
}

void CloseFiles(void)
{
	fclose(filein);
    fclose(fileout);
}

//### END
