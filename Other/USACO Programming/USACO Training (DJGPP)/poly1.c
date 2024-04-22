//### PROGRAM
/*
ID: mnansga001
PROG: poly1
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

typedef struct stTERM  {
	long exps[3];
    char sign;
} TERM;

const int COEFF = 0;
const int XEXP  = 1;
const int YEXP  = 2;
// for memseting (4 spaces in an int)
const int STRSP = (' ' << 24) + (' ' << 16) + (' ' << 8) + ' ';

long aterms, bterms, pterms;
char stra[85], strb[85];
TERM polya[50], polyb[50], product[100];
FILE *filein, *fileout;

long ParsePolyStr(char *str, TERM *terms);
long MultiplyPolys(void);
void SimplifyPoly(TERM *terms, long numterms);
void WritePoly(TERM *terms, long numterms);

long NumDigits(long num);
int QSortTerm(const TERM *term1, const TERM *term2);

int main(void)
{
    int i, starta = 1, startb = 1;

    filein  = fopen("poly1.in", "r");
    fileout = fopen("poly1.out", "w");

    fscanf(filein, "%s %s", &stra[1], &strb[1]);

    //make the beginning like a normal +/-
    if(stra[1] != '-')  {
    	stra[0] = '+';
        starta = 0;
    }
    if(strb[1] != '-')  {
    	strb[0] = '+';
        startb = 0;
    }

    aterms = ParsePolyStr(&stra[starta], polya);
    bterms = ParsePolyStr(&strb[startb], polyb);

    pterms = MultiplyPolys();
    SimplifyPoly(product, pterms);
    WritePoly(product, pterms);

    fclose(filein), fclose(fileout);
    return 0;
}

void WritePoly(TERM *terms, long numterms)
{
    char expout[250], polyout[250];
    long polypos = 0, endexp = 0;
	long i, first = 0;
    TERM *term;

    //clear the lines to spaces
    memset(expout, STRSP, 63);
    memset(polyout, STRSP, 63);

    for(i=0;i<numterms;i++)  {
    	term = &terms[i];

    	if(term->exps[0] != 0)  {
            //for the initial negative sign
    		if(!first && term->exps[0] < 0)
            	polyout[polypos++] = '-';

            //for +/- inclusions
    		if(first && term->exps[0] > 0)  {
            	sprintf(&polyout[polypos], " + ");
                polypos += 3;
            }
            else if(first && term->exps[0] < 0)  {
                sprintf(&polyout[polypos], " - ");
                polypos += 3;
            }

            //tells if the inital - sign is included
            first = 1;

            //print the coeff
            if(abs(term->exps[0]) > 1 ||
            (term->exps[1] == 0 && term->exps[2] == 0))  {
            	sprintf(&polyout[polypos], "%i", abs(term->exps[0]));
            	polypos += NumDigits(term->exps[0]);
                polyout[polypos] = ' ';
            }

            //print the x and exp
            if(term->exps[1] >= 1)  {
                polyout[polypos++] = 'x';

                if(term->exps[1] > 1)  {
	            	sprintf(&expout[polypos], "%i", term->exps[1]);
    	        	polypos += NumDigits(term->exps[1]);
                    expout[polypos] = ' ';
			  endexp = polypos;
                }
            }

            //print the y and exp
            if(term->exps[2] >= 1)  {
            	polyout[polypos++] = 'y';

                if(term->exps[2] > 1)  {
	            	sprintf(&expout[polypos], "%i", term->exps[2]);
    	            polypos += NumDigits(term->exps[2]);
                    expout[polypos] = ' ';
			  endexp = polypos;
                }
            }
    	}
    }

    sprintf(&expout[endexp], "\n\0");
    sprintf(&polyout[polypos], "\n\0");

    fprintf(fileout, "%s%s", expout, polyout);
}

long NumDigits(long num)
{
	int i;

	if(abs(num / 10000) >= 1)
    	return 5;
	if(abs(num / 1000) >= 1)
    	return 4;
	if(abs(num / 100) >= 1)
    	return 3;
    if(abs(num / 10) >= 1)
    	return 2;

	return 1;
}


int QSortTerm(const TERM *term1, const TERM *term2)
{
    //sorts x's incr order, y's dec order
	if(term1->exps[1] > term2->exps[1])
       	return -1;
    else if(term1->exps[1] < term2->exps[1])
        return 1;
    else  {
    	if(term1->exps[2] > term2->exps[2])
        	return 1;
    	else
        	return -1;
    }

	return 0;
}

void SimplifyPoly(TERM *terms, long numterms)
{
	long i, j;
    TERM *term, *testterm;

    for(i=0;i<numterms;i++)  {
		term = &terms[i];

        //check to see if there are similar exps
        //and if so make one coeff 0 and combine
		for(j=i+1;j<numterms;j++)  {
            testterm = &terms[j];

    		if(term->exps[1] == testterm->exps[1] &&
        	   term->exps[2] == testterm->exps[2])  {
               	term->exps[0] += testterm->exps[0];
                testterm->exps[0] = 0;
            }
		}
    }

    //sort the terms
    qsort(terms, numterms, sizeof(TERM), QSortTerm);
}

long ParsePolyStr(char *str, TERM *terms)
{
    long i, currterm = -1;
    long state = 0, laststate = 0, posnum = 0;
    char chr, nextchr, tempnum[5];
    TERM *term;

	for(i=0;str[i]!='\0';i++)  {
		chr = str[i];
        nextchr = str[i+1];

        switch(state)  {
            //+/- sign
        	case 0:
                term = &terms[++currterm];
                term->exps[0] = 1;
                term->exps[1] = 0;
                term->exps[2] = 0;

            	term->sign = chr;
            	break;
            //x symbol
            case 1:
                term->exps[1] = 1;
				break;
            //y symbol
            case 2:
            	term->exps[2] = 1;
            	break;
            //any number
            case 3:
                tempnum[posnum++] = chr;

                if(!isdigit(nextchr))  {
                    tempnum[posnum] = '\0';
                	term->exps[laststate] = atoi(tempnum);
                	posnum = 0;
            	}
                break;
        }

        if(state != 3) laststate = state;
    	if(nextchr == 'x') state = 1;
        if(nextchr == 'y') state = 2;
        if(isdigit(nextchr)) state = 3;
        if(nextchr == '+' || nextchr == '-') state = 0;
	}

    //make neg coeffs negative
	for(i=0;i<currterm+1;i++)
    	if(terms[i].sign == '-')
   			terms[i].exps[0] = -terms[i].exps[0];

    return currterm+1;
}

long MultiplyPolys(void)
{
    long i, j;
    long currterm = 0;

    //just multiply them together!
    for(i=0;i<aterms;i++)
    	for(j=0;j<bterms;j++)  {
        	product[currterm].exps[0] = polya[i].exps[0] * polyb[j].exps[0];
            product[currterm].exps[1] = polya[i].exps[1] + polyb[j].exps[1];
            product[currterm].exps[2] = polya[i].exps[2] + polyb[j].exps[2];
            currterm++;
        }

	return aterms*bterms;
}

//### END
