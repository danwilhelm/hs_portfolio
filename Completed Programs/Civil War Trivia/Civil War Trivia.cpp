/************************************************************************
	Civil War Game.cpp - by Daniel Wilhelm
	A civil war trivia game for the PC

	COMPILING NOTICE: REQUIRES THE XMedia LIBRARY TO COMPILE CORRECTLY!
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#include "XMedia.h"
#include "XMediaWin.h"

#include "DDFontEngine.cpp"

const int XRES	= 640;
const int YRES	= 480;
const int BPP	= 16;

#define PLAYER1A	DIK_A
#define PLAYER1B	DIK_S
#define PLAYER1C	DIK_D
#define PLAYER1D	DIK_F
#define PLAYER2A	DIK_NUMPAD4
#define PLAYER2B	DIK_NUMPAD5
#define PLAYER2C	DIK_NUMPAD6
#define PLAYER2D	DIK_NUMPADENTER

#define MAXQUESTIONCHR 200
#define MAXANSWERCHR   100
#define MAXNAMECHR	   50
#define MAXQUESTIONS   50
#define MAXANSWERS	   5
#define MAXCATEGORIES  5

struct QUESTION
{
	char	 question[MAXQUESTIONCHR];
	char	 answer[MAXANSWERS][MAXANSWERCHR];

	int		 numanswers;
	int		 valid;
	CSurface image;
};

struct CATEGORY
{
	char	 name[MAXNAMECHR];
	int		 numquestions;
	int		 valid;

	QUESTION question[MAXQUESTIONS];
};

int DrawQuestion(int qnum, int x, int y);
int DrawCategory(int x, int y);
int ParseQuestionFile(char *filename, QUESTION qarr[]);
int LoadCategory(char *catname, char *filename, CATEGORY *carr);
int DrawTextWrap(char *quest, int x, int y, int yspacing);
int PlayerGuess(int playernum, int playerguess);
int NewCategory(void);
int NewQuestion(void);


CATEGORY category[10];
int		 maxquestions = 0;
int		 playerscore[2];
char	 playerscorechr[2][10];
char	 timechr[10];
int		 activequestion = 0, activecategory = 0;
int		 answerorder[4];
int		 questionorder[50];
int		 currquestion = 0;
int		 lastquestion = 0;
QUESTION *questionptr;
int		 buttonlit[4];
CATEGORY *categoryptr;
int		 questionphase = FALSE;
int		 qrounds = 0;
int		 lastkey[8];
int		 rounds = 0;
int		 seconds = 0;
int		 maxc = 0;
int	     showcategories = TRUE;
int		 playercat = 0;
int		 catnum[4];


CXMedia  XMedia;
CSurface screen, background;
CSurface ansbutton[4][3];

TDDFontEngine *DDFontEngine=NULL;
TDDFont *DDFont=NULL;

int x = 0, newy = 0, currx = 0;

RECT cliprect[] = {{0,0, XRES, YRES}};

int Init(void)
{
	srand(timeGetTime());

	MyResources();
	CreateFullScreen("Civil War Trivia, by Daniel Wilhelm");

	if(XMedia.Create())
		xmError("DirectX 7.0 or higher is required.\nXMedia.Create() failed");

	if(XMedia.SetMode(&screen, XRES, YRES, BPP))
		xmError("XMedia.SetMode() failed");

	if(screen.Clip(cliprect, 1))
		xmError("No clip");

	if(background.Load(&XMedia, "warback0.bmp"))
		xmError("No background.");

	memset(category , 0, sizeof(CATEGORY) * MAXCATEGORIES);
	memset(buttonlit, 0, sizeof(int) * 4);

	LoadCategory("Trivia"		, "generalquestions.dat" , &category[0]);
	LoadCategory("Famous Quotes", "quotequestions.dat"	 , &category[1]);
	LoadCategory("Ordering"		, "orderingquestions.dat", &category[2]);
	LoadCategory("Daily Life"	, "lifequestions.dat"	 , &category[3]);
	LoadCategory("Causes"		, "causequestions.dat"	 , &category[4]);

	if(ansbutton[0][0].Load(&XMedia, "aunlit.bmp"))	xmError("Failed to load AUnlit.bmp");
	if(ansbutton[1][0].Load(&XMedia, "bunlit.bmp"))	xmError("Failed to load BUnlit.bmp");
	if(ansbutton[2][0].Load(&XMedia, "cunlit.bmp"))	xmError("Failed to load CUnlit.bmp");
	if(ansbutton[3][0].Load(&XMedia, "dunlit.bmp"))	xmError("Failed to load DUnlit.bmp");

	if(ansbutton[0][1].Load(&XMedia, "alit.bmp"))	xmError("Failed to load ALit.bmp");
	if(ansbutton[1][1].Load(&XMedia, "blit.bmp"))	xmError("Failed to load BLit.bmp");
	if(ansbutton[2][1].Load(&XMedia, "clit.bmp"))	xmError("Failed to load CLit.bmp");
	if(ansbutton[3][1].Load(&XMedia, "dlit.bmp"))	xmError("Failed to load DLit.bmp");

	if(ansbutton[0][2].Load(&XMedia, "awrong.bmp"))	xmError("Failed to load AWrong.bmp");
	if(ansbutton[1][2].Load(&XMedia, "bwrong.bmp"))	xmError("Failed to load BWrong.bmp");
	if(ansbutton[2][2].Load(&XMedia, "cwrong.bmp"))	xmError("Failed to load CWrong.bmp");
	if(ansbutton[3][2].Load(&XMedia, "dwrong.bmp"))	xmError("Failed to load DWrong.bmp");

	playerscore[0] = 0;
	playerscore[1] = 0;

    DDFontEngine = new TDDFontEngine(&XMedia);
    if((DDFont = new TDDFont(DDFontEngine, "Bimini.ddf", TEXTCOLOR(255,255,255))) == false)
		xmError("NO WORK!");
    DDFontEngine->UseFont(DDFont);

	sprintf(playerscorechr[0], "%i", playerscore[0]);
	sprintf(playerscorechr[1], "%i", playerscore[1]);

	NewCategory();

	showcategories = TRUE;
	questionphase  = FALSE;

	return TRUE;
}

int Main(void)
{
	if(XMedia.GetInput())
		xmError("Input retrieval failed!");

	if(ButtonDown(DIK_ESCAPE))
		gamerunning = FALSE;

	if(ButtonDown(PLAYER1A) && !lastkey[0]) PlayerGuess(0, 0);
	if(ButtonDown(PLAYER1B) && !lastkey[1]) PlayerGuess(0, 1);
	if(ButtonDown(PLAYER1C) && !lastkey[2]) PlayerGuess(0, 2);
	if(ButtonDown(PLAYER1D) && !lastkey[3]) PlayerGuess(0, 3);

	if(ButtonDown(PLAYER2A) && !lastkey[4]) PlayerGuess(1, 0);
	if(ButtonDown(PLAYER2B) && !lastkey[5]) PlayerGuess(1, 1);
	if(ButtonDown(PLAYER2C) && !lastkey[6]) PlayerGuess(1, 2);
	if(ButtonDown(PLAYER2D) && !lastkey[7]) PlayerGuess(1, 3);

	screen.Clear();
	
	screen.Blit(&background, 0, 0, BLIT_OPAQUE);
	
	if(!showcategories)
	{
		DrawQuestion(questionorder[activequestion], 30, 30);

		if(questionphase)
		{
			if(qrounds++ > 20)
			{
				NewQuestion();
				qrounds = 0;
				questionphase = FALSE;
			}
		}	
	}
	else
		DrawCategory(30, 30);

	DDFontEngine->DrawText(&screen, 0, YRES - 30, playerscorechr[0]);
	DDFontEngine->DrawText(&screen, XRES - DDFontEngine->StrLen(playerscorechr[1]), YRES - 30, playerscorechr[1]);
	DDFontEngine->DrawText(&screen, XRES / 2 -  DDFontEngine->StrLen(timechr) / 2, YRES - 30, timechr);

	if(++rounds >= 30)
	{
		seconds--;
		
		if(seconds <= 0 && !showcategories)
			NewCategory();
		
		if(seconds <= 0 && showcategories)
			PlayerGuess(0, 0);

		rounds = 0;
		sprintf(timechr, "%i", seconds);
	}

	if(gamerunning == FALSE)
	{
		DDFont->SetTextColor(TEXTCOLOR(0, 0, 255));
		DDFontEngine->DrawText(&screen, XRES / 2 - DDFontEngine->StrLen("Civil War Trivia") / 2, YRES - 60, "Civil War Trivia");
		DDFontEngine->DrawText(&screen, XRES / 2 - DDFontEngine->StrLen("by Daniel Wilhelm") / 2, YRES - 30, "by Daniel Wilhelm");

		XMedia.Flip();

		Sleep(2500);
		while(!ButtonDown(DIK_ESCAPE) && !ButtonDown(DIK_SPACE) && !ButtonDown(DIK_RETURN))
			XMedia.GetInput();
	}

	XMedia.Flip();
		
	return TRUE;
}


int Shutdown(void)
{
	for(int i=0;i<4;i++)
		for(int j=0;j<2;j++)
			ansbutton[i][j].Release();
	
	background.Release();
	screen.Release();
	XMedia.Release();

	return TRUE;
}


int DrawCategory(int x, int y)
{
	int i;
	
	DDFontEngine->DrawText(&screen, 0, 0, "Choose a category");

	for(i=0;i<4;i++)
	{
		DDFontEngine->DrawText(&screen, x + 30, i*35+y, category[catnum[i]].name);
		screen.Blit(&ansbutton[i][0], x, i*35+5+y);
	}

	return 0;
}


int DrawQuestion(int qnum, int x, int y)
{
	int i, newy;

    DDFontEngine->DrawText(&screen, 0, 0, categoryptr->name);

	newy = DrawTextWrap(categoryptr->question[qnum].question, x, y, 30);
    
	for(i=0;i<questionptr->numanswers;i++)
	{
		DDFontEngine->DrawText(&screen, x + 30, i*35+newy+y, categoryptr->question[qnum].answer[answerorder[i]]);
		screen.Blit(&ansbutton[i][buttonlit[i]], x, i*35+newy+5+y);
	}
	
	return 0;
}


int NewCategory(void)
{
	int i, j;

	//activecategory = 0;
	//categoryptr	   = &category[activecategory];

	for(i=0;i<4;i++)
	{
		catnum[i] = rand()%maxc;
	
		for(j=0;j<i;j++)
			if(catnum[j] == catnum[i])
				i--;
	}

	for(i=0;i<8;i++)
		lastkey[i] = 0;

	showcategories = TRUE;
	questionphase  = FALSE;
	seconds = 5;

	return 0;
}


int NewQuestion(void)
{
	int i, j;
	int randq;
	int maxa;
	int validq = 0, lasti = 0;

	if(currquestion > categoryptr->numquestions)
		currquestion = 0;

	lastquestion   = activequestion;
	activequestion = questionorder[currquestion++];

	maxa = categoryptr->question[activequestion].numanswers;

	/*for(i=0;i<maxq;i++)
	{
		questionorder[i] = rand%maxq;

		for(j=0;j<i;j++)
			if(questionorder[j] == questionorder[i])
				i--;
	}
		
		activequestion = rand()%maxq;

		if(categoryptr->question[activequestion].valid)
		{
			i--;

			if(lasti == i)	rounds++;
			else			rounds = 0;
			
			if(rounds == 500)
				NewCategory();				
		}

		lasti = i;
	}*/
	
	for(i=0;i<maxa;i++)
	{
		answerorder[i] = rand()%maxa;

		for(j=0;j<i;j++)
			if(answerorder[j] == answerorder[i])
				i--;
	}

	questionptr = &category[activecategory].question[activequestion];

	for(i=0;i<4;i++)
		buttonlit[i] = 0;

	for(i=0;i<8;i++)
		lastkey[i] = 0;

	return 0;
}


int PlayerGuess(int playernum, int playerguess)
{
	int i, j;
	int maxq;
	
	if(!questionphase)
	{
		if(!showcategories)
		{
			if(answerorder[playerguess] == 0)
			{
				buttonlit[playerguess] = 1;	
				playerscore[playernum] += 50;

				questionphase = TRUE;
			}
			else
			{
				buttonlit[playerguess] = 2;
				playerscore[playernum] -= 50;
			}

			lastkey[playerguess + playernum*4] = 1;

			sprintf(playerscorechr[0], "%i", playerscore[0]);
			sprintf(playerscorechr[1], "%i", playerscore[1]);
		}
		
		if(showcategories && seconds <= 3)
		{
			activecategory = catnum[playerguess];
			categoryptr    = &category[activecategory];
			
			maxq = categoryptr->numquestions;
			
			for(i=0;i<maxq;i++)
			{
				questionorder[i] = rand()%maxq;

				for(j=0;j<i;j++)
					if(questionorder[j] == questionorder[i])
						i--;
			}

			questionphase  = FALSE;
			showcategories = FALSE;

			NewQuestion();

			seconds = 60;
		}
	}

	return 0;
}


int LoadCategory(char *catname, char *filename, CATEGORY *carr)
{
	int quest;
	
	strcpy(carr->name, catname);
	
	if(!(quest = ParseQuestionFile(filename, carr->question)))
		return FALSE;

	carr->numquestions = quest;

	maxc++;
	
	return TRUE;
}



int ParseQuestionFile(char *filename, QUESTION qarr[])
{
	int  qnum = 0, ansnum = 0;
	char nextchr[20];
	FILE *fstream;
	
	if(!(fstream = fopen(filename, "r")))
		return FALSE;

	fgets(nextchr, 20, fstream);
	while(nextchr[0] != '`')
	{
		fgets(qarr[qnum].question, 200, fstream);
		nextchr[0] = qarr[qnum].question[0];		

		ansnum = 0;
		while(nextchr[0] != '~' && nextchr[0] != '`')
		{
			fgets(qarr[qnum].answer[ansnum], 100, fstream);		
	
			nextchr[0] = qarr[qnum].answer[ansnum][0];
			ansnum++;
		}
		
		qarr[qnum].numanswers = ansnum - 1;
		qnum++;
	};

	maxquestions = qnum;
	fclose(fstream);

	return qnum;
}


int DrawTextWrap(char *quest, int x, int y, int yspacing)
{
	int  i;
	int  lastword = 0, lastphrase = 0;
	BOOL notfirstline = FALSE;
	
	for(i=0;i<strlen(quest);i++)
	{
		if(quest[i] == ' ' || i == strlen(quest)-1)
		{
			if(DDFontEngine->StrLen(&quest[lastphrase], i - lastphrase) + x > XRES)
			{
				DDFontEngine->DrawText(&screen, x, y, &quest[lastphrase+notfirstline], lastword - lastphrase);

				lastphrase = lastword;
				y += yspacing;
				notfirstline = TRUE;
			}

			lastword = i;
		}
	}

	DDFontEngine->DrawText(&screen, x, y, &quest[lastphrase+notfirstline], i - lastphrase);
	y += yspacing;

	return y;
}
