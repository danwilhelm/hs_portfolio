//### PROGRAM
/*
ID: mnansga001
PROG: beads
*/

#include <stdio.h>

int GetSameBeads(int startpos, int dir);

#define MAX_BEADS	351

int totalbeads;
char beadstr[MAX_BEADS];
FILE *filein, *fileout;

int main(void)
{
    int i;
    int samebeads, maxbeads = 0;

	filein  = fopen("beads.in", "r");
    fileout = fopen("beads.out", "w");

    fscanf(filein, "%i %s", &totalbeads, beadstr);

    for(i=0;i<totalbeads;i++)  {
    	samebeads  = GetSameBeads(i, -1);
        samebeads += GetSameBeads(i, 1);

        if(samebeads > totalbeads)
        	samebeads = totalbeads;

        if(samebeads > maxbeads)
        	maxbeads = samebeads;
    }

    fprintf(fileout, "%i\n", maxbeads);

    fclose(filein), fclose(fileout);
	return 0;
}

int GetSameBeads(int startpos, int dir)
{
	int currpos = startpos, beads = 0;
    char beadchain = 0;

    if(dir < 0) currpos--;
    if(currpos < 0) currpos += totalbeads;

    while(!beadchain || beadstr[currpos] == beadchain || beadstr[currpos] == 'w')  {
        if(!beadchain && beadstr[currpos] != 'w')
        	beadchain = beadstr[currpos];

        currpos += dir;
        if(currpos < 0) currpos += totalbeads;
        if(currpos >= totalbeads) currpos -= totalbeads;

        if(++beads >= totalbeads)
		break;
    }

	return beads;
}

//### END
