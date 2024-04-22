#include <stdio.h>
#include <ctype.h>

const char FILE_IN[]  = "ride.in";
const char FILE_OUT[] = "ride.out";
FILE *filein, *fileout;

int OpenFiles(void);
void CloseFiles(void);

#define MODNUM		47			// the modulus number
#define MAX_STRCHRS	73			// maximum chars in a string
#define ASCII_ALPHA	'A' + 1		// the ASCII num to subtract so A=1

int main(void)
{
	char str1[MAX_STRCHRS], str2[MAX_STRCHRS];	// to hold both strings
    long long str1len = 1, str2len = 1;	 // the value of the *'s in each string
    long i;

	if(!OpenFiles())
		return 0;

    fgets(str1, MAX_STRCHRS, filein);		// get the strings
    fgets(str2, MAX_STRCHRS, filein);

    for(i=0;str1[i]!='\0';i++)		// for each letter (str1), find * value
    	if(isalpha(str1[i]))
    		str1len *= (str1[i] - ASCII_ALPHA);	// subtract ASCII code for A=1

    for(i=0;str2[i]!='\0';i++)		// (see above)
    	if(isalpha(str2[i]))
    		str2len *= (str2[i] - ASCII_ALPHA);

    if((str1len % MODNUM) == (str2len % MODNUM))	// find the mods!
    	fputs("GO", fileout);
    else
    	fputs("STAY", fileout);

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
