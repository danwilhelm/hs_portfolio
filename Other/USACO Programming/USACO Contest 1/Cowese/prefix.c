/*
PROG: prefix
LANG: C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stWORDS  {
	char strword[100];
	unsigned short order;
} WORDS;

unsigned short order[20000];
int ord = 0;

WORDS word[20000];
FILE *filein, *fileout;

#define min(a,b)  (((a) < (b)) ? (a) : (b))

int QSortMaxWords(const int *word1, const int *word2);
int QSortWords(const WORDS *word1, const WORDS *word2);
int CompareWords(const WORDS *word1, const WORDS *word2);

int main(void)
{
	int i, nwords = 0;
	int wrd = 0, maxrun = 0, run = 0;
	int maxwords[50000];
	
	filein  = fopen("prefix.in", "r");
	fileout = fopen("prefix.out", "w");

	fscanf(filein, "%i", &nwords);

	// Get the data from the file
	for(i=0;i<nwords;i++)  {
		fscanf(filein, "%s", word[i].strword);
		word[i].order = i;
		//printf("%s", word[i].strword);
	}
	
	// Sort words in alpha order
	qsort(word, nwords, sizeof(WORDS), (int(*)(const void*,const void*))QSortWords);
	
	//for(i=0;i<nwords;i++)  {
	//	printf("%s %i\n", word[i].strword, word[i].order);
	//}

	// Get longest run
	for(i=0;i<nwords-1;i++)  {
		run = CompareWords(&word[i], &word[i+1]);

		if(run >= maxrun)
			maxrun = run;
	}

	//printf("%i\n", maxrun);
	
	// Get longest wrds w/ longest runs
	for(i=0;i<nwords-1;i++)  {
		run = CompareWords(&word[i], &word[i+1]);
		if(run == maxrun)  {
			if(maxwords[wrd-1] != i)
				maxwords[wrd++] = i;

			maxwords[wrd++] = i+1;
		}
	}

	//for(i=0;i<wrd;i++)
	//{
	//	printf("%s %i\n", word[maxwords[i]].strword, word[maxwords[i]].order);
	//}

	// Sort longest words in order they originally appeared
	qsort(maxwords, wrd, sizeof(int), (int(*)(const void*,const void*))QSortMaxWords);

	//for(i=0;i<wrd;i++)
	//{
	//	printf("%s %i\n", word[maxwords[i]].strword, word[maxwords[i]].order);
	//}

	//printf("%s\n%s\n", word[maxwords[0]].strword, word[maxwords[1]].strword);
	
	fprintf(fileout, "%s\n%s\n", word[maxwords[0]].strword, word[maxwords[1]].strword);

	fclose(filein), fclose(fileout);
}


int QSortMaxWords(const int *word1, const int *word2)
{
	if(word[*word1].order > word[*word2].order)
		return 1;
	else
		return -1;
}


int CompareWords(const WORDS *word1, const WORDS *word2)
{
	int run = 0;

	int minlen = min(strlen(word1->strword), strlen(word2->strword));

	for(run=0;run<minlen;run++)  {
		if(word1->strword[run] != word2->strword[run])
			return run;
	}
	
	return run;
}


int QSortWords(const WORDS *word1, const WORDS *word2)
{
	int c, minlen;
	
	minlen = min(strlen(word1->strword), strlen(word2->strword));

	for(c=0;c<minlen;c++)  {
		if(word1->strword[c] > word2->strword[c])
			return 1;
		
		if(word1->strword[c] < word2->strword[c])
			return -1;

		//if(c+1 >= maxmatch)  {
		//	maxmatch = c+1;

		//	order[ord++] = word1->order;
		//	order[ord++] = word2->order;
		//}
	}

	return 0;
}