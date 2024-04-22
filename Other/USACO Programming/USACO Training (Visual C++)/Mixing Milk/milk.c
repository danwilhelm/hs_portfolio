/*
ID: mnansga001
PROG: milk
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct stFARMER  {
	int price;
	int gallons;
} FARMER;

int SortFarmers(const FARMER *farmer1, const FARMER *farmer2);

int nfarmers, milkwanted;
FARMER farmer[5000];
FILE *infile, *outfile;

int main(void)
{
	int i, total = 0, milkdiff = 0, cost = 0;
	
	// open files
	infile  = fopen("milk.in", "r");
	outfile = fopen("milk.out", "w");


	// load farmers
	memset(farmer, 0, sizeof(FARMER)*5000);
	fscanf(infile, "%i %i", &milkwanted, &nfarmers);

	for(i=0;i<nfarmers;i++)
		fscanf(infile, "%i %i", &farmer[i].price, &farmer[i].gallons);


	// sort the farmers by prices
	qsort(farmer, nfarmers, sizeof(FARMER), (int(*)(const void*,const void*))SortFarmers);


	// loop through, buying from the cheapest to most expensive
	for(i=0;i<nfarmers;i++)
	{
		milkdiff = milkwanted - total;

		if(farmer[i].gallons >= milkdiff)  {
			cost  += milkdiff * farmer[i].price;
			total += milkdiff;
			break;
		} 
		else  {
			cost  += farmer[i].gallons * farmer[i].price;
			total += farmer[i].gallons;
		}
	}

	//printf("%i %i\n", cost, total);

	// write to file
	fprintf(outfile, "%i\n", cost);

	// close the files
	fclose(infile), fclose(outfile);
	
	return 0;
}

int SortFarmers(const FARMER *farmer1, const FARMER *farmer2)
{
	if(farmer1->price > farmer2->price)
		return 1;
	else
		return -1;
}