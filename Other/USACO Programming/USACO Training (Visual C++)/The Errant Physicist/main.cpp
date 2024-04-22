//### PROGRAM
/*
ID: mnansga001
PROG: poly1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TERM
{
	int coeff;
	int power[2];
};

struct POLYNOMIAL
{
	int nterms;
	
	TERM term[100];
};

POLYNOMIAL MultiplyPolys(POLYNOMIAL poly1, POLYNOMIAL poly2);
POLYNOMIAL ParsePoly(char polystr[]);

int main(void)
{
	POLYNOMIAL inpoly1, inpoly2, outpoly;
	char inpolystr1[200], inpolystr2[200], outpolystr[200];
	FILE *infile, *outfile;

	infile = fopen("poly1.in", "r");
	outfile = fopen("poly1.out", "w");

	fscanf(infile, "%s %s", inpolystr1, inpolystr2);

	inpoly1 = ParsePoly(inpolystr1);
	inpoly2 = ParsePoly(inpolystr2);

	outpoly = MultiplyPolys(inpoly1, inpoly2);
	
	fclose(outfile);
	fclose(infile);
		
	return 0;
}

POLYNOMIAL MultiplyPolys(POLYNOMIAL poly1, POLYNOMIAL poly2)
{
	int i, j, currterm = 0;
	POLYNOMIAL outpoly;
	
	outpoly.nterms = poly1.nterms * poly2.nterms;
	
	for(i=0;i<poly1.nterms;i++)
	{
		for(j=0;j<poly2.nterms;i++)
		{
			outpoly.term[currterm].coeff = poly1.term[i].coeff * poly2.term[j].coeff;

			outpoly.term[currterm].power[0] = poly1.term[i].power[0] + poly2.term[j].power[0];
			outpoly.term[currterm].power[1] = poly1.term[i].power[1] + poly2.term[j].power[1];

			currterm++;
		}
	}

	for(i=0;i<outpoly.nterms;i++)
		for(j=i;j<outpoly.nterms;j++)
			if(outpoly.term[i].power[0] == outpoly.term[j].power[0])
				if(outpoly.term[i].power[1] == outpoly.term[j].power[1])
				{
					outpoly.term[i].coeff += outpoly.term[j].coeff;

					outpoly.term[j].coeff = 0;
				}

	return outpoly;
}

POLYNOMIAL ParsePoly(char polystr[])
{
	POLYNOMIAL poly;
	
	int tempindex = 0, nterm = 0, i;
	char chr, lastchr = '\0', temp[10];

	
	for(i=0;i<strlen(polystr);i++)
		if(polystr[i] == '\n' || polystr[i] == '\0')
			break;
	
	polystr[i] = '+';
	
	polystr[i+1] = '\0';


	memset(&poly, 0, sizeof(POLYNOMIAL));

	printf("%s\n", polystr);

	for(i=0;i<strlen(polystr);i++)
	{
		printf("%c", chr);
		
		switch(chr)
		{
		case '+': case '-':
		case 'x': case 'y':
			temp[tempindex] = '\0';

			switch(lastchr)
			{
			case '+': case '-':
				if(tempindex == 0)
				{
					if(lastchr == '+')
						poly.term[nterm].coeff = 1;
					else
						poly.term[nterm].coeff = -1;
				}
				else
				{
					if(lastchr == '+')
						poly.term[nterm].coeff = atoi(temp);
					else
						poly.term[nterm].coeff = -atoi(temp);
				}

				break;
			
			case 'x':
				if(tempindex == 0)
					poly.term[nterm].power[0] = 1;
				else
					poly.term[nterm].power[0] = atoi(temp);
				break;

			case 'y':
				if(tempindex == 0)
					poly.term[nterm].power[1] = 1;
				else
					poly.term[nterm].power[1] = atoi(temp);
				break;
			};

			if(chr == '+' || chr == '-')
				nterm++;

			tempindex = 0;
			lastchr = chr;
		
			break;

		default:
			temp[tempindex] = chr;
			tempindex++;
		};
	}

	poly.nterms = nterm+1;

	for(i=0;i<poly.nterms;i++)
		printf("%ix%iy%i\n", poly.term[i].coeff, poly.term[i].power[0], poly.term[i].power[1]);

	return poly;
}

//### END
