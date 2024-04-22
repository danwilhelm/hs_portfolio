#include <stdio.h>
#include <strings.h>
#include <math.h>

const char FILE_IN[]  = "gift1.in";
const char FILE_OUT[] = "gift1.out";
FILE *filein, *fileout;

int OpenFiles(void);
void CloseFiles(void);

#define MAX_NAMECHRS	15
#define MAX_PEOPLE		10

// Holds a person's name and current cash balance
typedef struct stPERSON  {
	char name[MAX_NAMECHRS];
    long cash;
} PERSON;

// Returns a pointer to a person name[]
PERSON *FindPerson(char name[]);

PERSON person[MAX_PEOPLE];	// index of all people and their cash
long numpeople;				// number of total people

int main(void)
{
    int i, j;
	long numreceivers;	// (temp) number of receivers of a gift
    PERSON tempperson;	// (temp) a person
    PERSON *currperson;	// (temp) the current person working on

	if(!OpenFiles())
    	return 0;

    fscanf(filein, "%i", &numpeople);	// get total # of people

    for(i=0;i<numpeople;i++)			// get all people's names
    	fscanf(filein, "%s", person[i].name);

    for(i=0;i<numpeople;i++)  {
    	// get the current person's name
	    fscanf(filein, "%s", tempperson.name);
		// get # of receivers for current person
	    fscanf(filein, "%i %i", &tempperson.cash, &numreceivers);

        currperson = FindPerson(tempperson.name);

        // if the person isn't found
        if(!currperson)
        	fprintf(fileout, "No '%s' found.", tempperson.name);

		// the current person loses the cash he gives away
        currperson->cash -= tempperson.cash;

        // if a divide by 0 occurs
        if(numreceivers > 0)  {
      // if the cash doesn't divide evenly, otherwise calc equal divisions
        	currperson->cash += tempperson.cash % numreceivers;
        	tempperson.cash = (long)floor(tempperson.cash / numreceivers);
        }
        else
        	tempperson.cash = 0;

        for(j=0;j<numreceivers;j++)  {
        	// get the current receivers name
        	fscanf(filein, "%s", tempperson.name);
            currperson = FindPerson(tempperson.name);

            // give that person his gift!
            currperson->cash += tempperson.cash;
        }
    }

    // print results to file
    for(i=0;i<numpeople;i++)
        fprintf(fileout, "%s %i\n", person[i].name, person[i].cash);

    CloseFiles();
	return 0;
}


PERSON *FindPerson(char name[])
{
	int i;

    // search for a person and return a pointer to him
	for(i=0;i<numpeople;i++)
		if(strcmp(person[i].name, name) == 0)
			return &person[i];

    return NULL;
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
