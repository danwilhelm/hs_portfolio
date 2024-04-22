#include <stdio.h> 
#include <iostream.h>
#include <stdlib.h>

char HSName[10][4];
long HSScore[10]; 
char HSLevel[10];

/*char HSName[10][4]  = {"DKW", "HAW", "TYE", "CHA", "BBE", "ALE", 
					   "TAS", "EJK", "DXC", "MLQ"};

long HSScore[10]    = {130584, 128093, 100221, 95640, 88256, 56000,
					   43023 , 42013 , 40228 , 10374};

char HSLevel[10]	= {251, 56, 53, 54, 51, 52, 52, 49, 49, 49};*/ 

void AddHScore(char NewName[], long NewScore, char NewLevel);
void ReadHScores(char FileName[]);
void WriteHScores(char FileName[]);

/*void main(int argc, char *argv[])
{ 
	int i;
	char Name[] = "BFG";
	long HScore = atol(argv[1]);
	char Level  = '2';
	
	ReadHScores("HSCORES.DAT", HSName, HSScore);
	
	AddHScore(Name, HScore, Level);
	
	for(i=0;i<10;i++)
		cout << HSName[i] << '\t' << HSScore[i] << '\t' << HSLevel[i] << '\n';
	
	//WriteHScores("HSCORES.DAT");
}*/	


void AddHScore(char NewName[], long NewScore, char NewLevel)
{
	int i, index;
	
	if(HSScore[9] > NewScore)
		return;
	
	for(index=0;index<10;index++)
	{
		if(HSScore[index] < NewScore)
			break;
	}
	
	for(i=9;i>index;i--)
	{
		HSName[i][0] = HSName[i-1][0];
		HSName[i][1] = HSName[i-1][1];
		HSName[i][2] = HSName[i-1][2];
		HSScore[i] 	 = HSScore[i-1];
		HSLevel[i]   = HSLevel[i-1];
	}
	
	HSName[index][0] = NewName[0];
	HSName[index][1] = NewName[1];
	HSName[index][2] = NewName[2];
	HSScore[index] = NewScore;
	HSLevel[index] = NewLevel;
}


void ReadHScores(char FileName[])
{  
	FILE *fstream;
	int i;
		
	if((fstream = fopen(FileName, "r")) == NULL)
	{
		printf("High scores failed to write!");
		exit(0);
	}
	
	fseek(fstream, -80, SEEK_END);
	
	for(i=0;i<10;i++) 
	{
		fread(&HSName[i] , 3, 1, fstream);
		fread(&HSScore[i], 4, 1, fstream);
		HSLevel[i] = fgetc(fstream);
		
		HSScore[i] *= 500;
	}
	
	fclose(fstream);
}
	 


void WriteHScores(char FileName[])
{
	FILE *fstream;
	int i;
	long TempScore;
		
	if((fstream = fopen(FileName, "r+")) == NULL)
	{
		printf("High scores failed to write!");
		exit(0);
	} 
	
	fseek(fstream, -80, SEEK_END);
	
	for(i=0;i<10;i++) 
	{
		TempScore = HSScore[i] / 500;
		
		fwrite(&HSName[i] , 3, 1, fstream);
		fwrite(&TempScore, 4, 1, fstream); 
		fputc(HSLevel[i], fstream);
	}
	
	fclose(fstream);	
}  




	
	