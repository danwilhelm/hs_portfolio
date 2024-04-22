/*
ID: mnansga001
PROG: clock
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *infile, *outfile;

void NumberToWord(char *word, int num);

int main(void)
{
	int hours, minutes;
	char hour[100], nexthour[100], minute[100];
	char diffminute[100], final[100];
	
	hour[0] = minute[0] = final[0] = '\0';
	nexthour[0] = nexthour[0] = diffminute[0] = '\0';

	
	// open files
	infile  = fopen("clock.in", "r");
	outfile = fopen("clock.out", "w");


	// load time
	fscanf(infile, "%i:%i", &hours, &minutes);

	
	// tricky: the next hour of 12:00 is 1:00
	if(hours == 12)
		NumberToWord(nexthour, 1);
	else
		NumberToWord(nexthour, hours+1);

	NumberToWord(hour, hours);
	NumberToWord(minute, minutes);
	NumberToWord(diffminute, 60-minutes);

	//printf("%s %s %s %s", hour, nexthour, minute, diffminute);


	if(minutes == 0)  {
		strcpy(final, hour);
		strcat(final, " o'clock");
	}
	else if(minutes == 15)  {
		strcpy(final, "quarter past ");
		strcat(final, hour);
	}
	else if(minutes == 45)  {
		strcpy(final, "quarter to ");
		strcat(final, nexthour);
	}
	else if(minutes > 45)  {
		strcpy(final, diffminute);
		strcat(final, " to ");
		strcat(final, nexthour);
	}
	else  {
		strcpy(final, hour);
		strcat(final, " ");
		strcat(final, minute);
	}

	
	
	final[0] -= 32;

	//printf("%s\n", final);

	// write to file
	fprintf(outfile, "%s\n", final);


	// close the files
	fclose(infile), fclose(outfile);
	
	return 0;
}

void NumberToWord(char *word, int num)
{
	switch(num)
	{
	case 10: strcat(word, "ten");		break;
	case 11: strcat(word, "eleven");	break;
	case 12: strcat(word, "twelve");	break;
	case 13: strcat(word, "thirteen");	break;
	case 14: strcat(word, "fourteen");	break;
	case 15: strcat(word, "fifteen");	break;
	case 16: strcat(word, "sixteen");	break;
	case 17: strcat(word, "seventeen");	break;
	case 18: strcat(word, "eighteen");	break;
	case 19: strcat(word, "nineteen");	break;
	};

	if(num >= 10 && num <= 20)
		return;


	switch(num/10)
	{
	case 2: strcat(word, "twenty");	break;
	case 3: strcat(word, "thirty");	break;
	case 4: strcat(word, "forty");	break;
	};

	if(num/10 && num%10)
		strcat(word, "-");

	switch(num%10)
	{
	case 1: strcat(word, "one");	break;
	case 2: strcat(word, "two");	break;
	case 3: strcat(word, "three");	break;
	case 4: strcat(word, "four");	break;
	case 5: strcat(word, "five");	break;
	case 6: strcat(word, "six");	break;
	case 7: strcat(word, "seven");	break;
	case 8: strcat(word, "eight");	break;
	case 9: strcat(word, "nine");	break;
	};

	//printf("\n%s\n", word);


	return;
}
