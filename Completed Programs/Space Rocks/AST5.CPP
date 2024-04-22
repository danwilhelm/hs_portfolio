/*****************************************************
				  SPACE ROCKS v1.01
					ASTEROID.CPP
				  by Danny Wilhelm
				  	  04/11/99
*****************************************************/
#include "asteroid.h"
#include "extract2.cpp" 
#include "sound.cpp"
#include "hscore.cpp"
                                                           
#define NOFILL			0
#define RECTFILL		1
#define LASTLEVELCHAR	'C'  
#define VERSTRING		"Space Rocks, v1.01 by Columbus North High School's CGCT"

void MainMenu(void); 
void GetHScore(char LevelChar);
void Rect(int StartX, int StartY, int EndX, int EndY, int Color, int Fill);
void MsgBox(int X, int Y, char *ShowStr, int StrLen);
void NextLevel(void);
void FadeToBlack(void);
void FadeToPalette(PALETTE Palette); 
void ShowNextBox(void);
void DispScore(void);
void DispShieldLvl(void); 
void ShowLogo(void);

PALETTE GamePalette, FakePalette;

long LevelTimes[9] = {0, 4200, 8400, 12600, 16800, 21000, 25200, 29400, 33600};
int Level = 1, Lives = 4, SoundOn = 1;
char LivesStr[3] = "x3";
int AstOut = 0, NoDeath = FALSE;
int LaserWait = 0, ShipFrame = 0, Retry = FALSE;
int ShieldLeft = MAX_SHIELDS, ShieldOn = FALSE, ShieldRep = 0;
int ShieldTimer = 0, WeapSpeed = 40;
char PlanetNames[9][9] = {""      , "Cronopia", "Mermore", "Blasteel", "Luna",
						  "Ophius", "Tenopia" , "Exius"  , "Scuur" };

int SoundStatus = 0;


void main(int argc, char *argv[])
{   
	int i;
	
	if(argv[1][0] == 'd')
		NoDeath = TRUE;
	
	if(argv[1][0] == 's')
		SoundOn = FALSE; 
	
	if(argv[1][0] == 'v')
		Exit_Program(VERSTRING);
	
	Initialize();
	ShowLogo();
	
	MainMenu(); 
	
	ShowNextBox();
	
	UpdateScore(0);
	DispScore();
	DispShieldLvl();	
		
	while(KeyB_Check() != SC_ESCAPE)		//the game loop (while no key pressed)
	{
		Mouse_Status();                     //check the mouse, if any movement or
		Mouse_Movement();					// buttons pressed, take care of them
		if(Mouse_MickeyX || Mouse_MickeyY)
			Move_Player();
		if(Mouse_Button_Status)
			Fire_Weapon(); 
			
		Wait_Until_VRT();
		Time++;
		
		if(Time == 3)					//once every 5 times move the stars and the 
		{
			MoveAsteroids();
			MoveStars();				//  points (if needed). 
			if(ShieldOn == TRUE && ++ShieldTimer >= Level - 1)
			{
				ShieldLeft--;
				ShieldTimer = 0;
				Vert_Line(242 + ShieldLeft,16, 20, BLACK);
			}
			Time = 0;
		}
		
		if(Time == 1)
			MoveAsteroids();
		
		if(WeaponOn)					//if the weapon is on, move it every time
			MoveLaser(); 
			
		if(LaserWait > 0)
			if(++LaserWait == WeapSpeed)
				LaserWait = 0; 
		
		if(TimeLeft > TIME_MASK)
		{
			TimeLeft--;
			_ltoa(TimeLeft, StrTimeLeft, 10);
			for(i=0;i<5;i++)
				StrTimeLeft[i] = ' ';
		} 
	
		if(SoundStatus && SoundOn)
		{
			Sound(SoundStatus, 10000); 
			SoundStatus -= 100;
			if(SoundStatus < 400)
				SoundStatus = 0;			
		}			
		
		DispBigText(StrTimeLeft, 100,5, 46, 4);	// display time left 
		DispBigText(LivesStr   ,   0,0, 46, 4);
		
		if(ShieldOn == FALSE && ShieldRep == 0)
		{
			Put_Blank(PlayerX, PlayerY, Ship[ShipFrame], BLACK);
			ShipFrame = 0;
			Put(PlayerX, PlayerY, Ship[ShipFrame]);
			ShieldRep = 1;
		}
			
		ShieldOn = FALSE;
	}
	
	Exit_Program(VERSTRING);
}

void ShowLogo(void)
{
	int i;
	
	BUFPIC LogoPic;
		
	for(i=0;i<255;i++)
		Set_Palette(0, 0, 0, i);
	RLEDAT_GetPic(RLEDATName, LogoPic, CGCTLOGO);
	Put(0, 0, LogoPic);
	FadeToPalette(GamePalette);
	Mouse_Wait();
	
	FadeToBlack();
	
	_ffree(LogoPic.Buffer);
}	


void MainMenu(void)
{
	int mdone = 0, i, ans, menuchoice;
	char TempStr[10];
	BUFPIC HScoreTable;
	
	RLEDAT_GetPic(RLEDATName, HScoreTable, HSCORETBL);
	
	Score = TIME_MASK;
	UpdateScore(0);
	Lives = 4;
	LivesStr[1] = '3';
	Level = 1; 
	LevelString[6] = Level + 48;
	NumAsteroids = 5;
	TimeLeft = TIME_MASK;
	WeapSpeed = 40;
	RLEDAT_GetPic(RLEDATName, Laser, LAS0);
	Retry = FALSE;
		
		
	Clear_Screen(BLACK);
	for(i=0;i<256;i++)
		Set_Palette(0, 0, 0, i);
		
	RLEDAT_GetPic(RLEDATName, StillPlanet, SPMENU);
	RLEDAT_GetPic(RLEDATName, TradeBack , CREDITS);
		
	Put(80, 50, StillPlanet);
	FadeToPalette(GamePalette);
	
	Mouse_Set_Boundaries(80,50, 230,135);
	Mouse_Show_Pointer(); 
	Use_Palette(GamePalette);
	
	while(!mdone)
	{
		for(i=0;i<NUM_STARS;i++)					// move the starfield
		{   
			ans = Read_Pixel(Stars[i].X, Stars[i].Y);
			if(ans > 0 && ans < 4)
				Set_Pixel(Stars[i].X, Stars[i].Y, BLACK);	
			Stars[i].Y += Stars[i].Speed;					
			if(Stars[i].Y > 199)
				Stars[i].Y -= 199;
			if(Read_Pixel(Stars[i].X, Stars[i].Y) == BLACK)
				Set_Pixel(Stars[i].X, Stars[i].Y, Stars[i].Speed);		
		}
		 
		for(i=0;i<4;i++)
		{
			Wait_Until_VRT();
		}
		
		Mouse_Status();
			
		if(MouseX >= 92 && MouseX <= 236)
		{
			menuchoice = 0;
			if(MouseX > 92 && MouseX < 227)
			{
				if(MouseY > 75 && MouseY < 90)
					menuchoice = 241;
				if(MouseY > 95 && MouseY < 110)
					menuchoice = 242;
				if(MouseY > 115 && MouseY < 130)
					menuchoice = 243;
			}
			else
				if(MouseY > 53 && MouseY < 62)
					menuchoice = 244;
		}
		else
			menuchoice = 0;
			
		if(menuchoice == 0)
			for(i=241;i<245;i++)
				Set_Palette(0, 204, 204, i);
		else
		{
			for(i=241;i<245;i++)
				Set_Palette(0, 204, 204, i);	
			Set_Palette(0, 255, 255, menuchoice); 
		}
						
		if(Mouse_Button_Status == 1 && menuchoice == 241)
			mdone = 1;
		
		if(Mouse_Button_Status == 1 && menuchoice == 242)
		{	
			Mouse_Hide_Pointer();
			FadeToBlack();
			Clear_Screen(BLACK);
			Put(0, 0, HScoreTable);
			ReadHScores("SPACERCK.DAT");  
			for(i=0;i<10;i++)
			{
				ltoa(HSScore[i], TempStr, 10);
				DispBigText(HSName[i] , 68 , 45 + (i * 9), 0, 186);
				DispBigText(TempStr   , 137, 45 + (i * 9), 0, 186);
				DispBigChar(HSLevel[i], 241, 45 + (i * 9), 0, 186);
			}
			
			FadeToPalette(GamePalette);			
			
			Mouse_Wait();
			FadeToBlack(); 
			Clear_Screen(BLACK);
			Put(80, 50, StillPlanet);
			FadeToPalette(GamePalette);
			Mouse_Show_Pointer();		
		}
	    
		if(Mouse_Button_Status == 1 && menuchoice == 243)
		{
			FadeToBlack();
			Exit_Program(VERSTRING);
		}
		
		if(Mouse_Button_Status == 1 && menuchoice == 244)
		{
			Mouse_Hide_Pointer();
			FadeToBlack();
			Put(0, 0, TradeBack);
			FadeToPalette(GamePalette);
			
			Mouse_Wait();
			FadeToBlack(); 
			Clear_Screen(BLACK);
			Put(80, 50, StillPlanet);
			Mouse_Show_Pointer();
			FadeToPalette(GamePalette);
		}
	} 
	  
	RLEDAT_GetPic(RLEDATName, TradeBack, STILLBCK);
		
	Mouse_Hide_Pointer(); 
	Mouse_Set_Boundaries(0,0, SCREEN_WIDTH,SCREEN_HEIGHT);
}


void NextLevel(void)
{
	int i;
	
	for(i=0;i<5;i++)
		Wait_Until_VRT();
	
	for(i=PlayerY;i>0;i-=4)						// Ship[0] speeds to planet
	{           
		Put(PlayerX, i, Ship[ShipFrame]);
		Wait_Until_VRT();
		Wait_Until_VRT();
		Put_Blank(PlayerX, i, Ship[ShipFrame], BLACK);
	} 
	
	FadeToBlack();
	
	Level++;
	LevelString[6] = Level + 48;
	
	if(LivesStr[1] <= '9')
	{
		Lives++;
		LivesStr[1]++;
	}
		
	if(Level == 9)
	{
		GetHScore(LASTLEVELCHAR);
			
		MsgBox(129, 93, "You win!!", 9);
		FadeToPalette(GamePalette);
		KeyB_Wait();
		FadeToBlack();
		Exit_Program("Congratulations!!!! Thanks for playing! Space Rocks, v1.01");
	}
	
	RLEDAT_GetPic(RLEDATName, StillPlanet, Level + STILLP1 - 2);
	
	Put(0, 0, TradeBack);
	Put(160 - (StillPlanet.Width >> 1), 76 - (StillPlanet.Height >> 1), StillPlanet);
	MsgBox(127, 0, PlanetNames[Level - 1], 8);
	
	FadeToPalette(GamePalette);
		
	Mouse_Wait();
		
	ShowNextBox();
}

void ShowNextBox(void)
{ 
	int i;
	
	FadeToBlack();
	Clear_Screen(BLACK);
	
	MsgBox(129,93, LevelString, 7);
	FadeToPalette(GamePalette);	 
	
	Mouse_Wait();
						
	Rect(129,93, 129 + (7 * ROMCHARW) + 6, 93 + ROMCHARH + 6, BLACK, RECTFILL);
	
	for(i=0;i<MAX_ASTEROIDS;i++)
		Asteroids[i].ExplFrame = -1;
	
	if(Retry == FALSE)
	{
		NumAsteroids++;
	
		RLEDAT_GetPic(RLEDATName, Laser, Level + LAS0 - 1);
		WeapSpeed -= 3;
		TimeLeft = LevelTimes[Level] + TIME_MASK; 
	}
	
	ShieldLeft = MAX_SHIELDS;
		
	Clear_Screen(BLACK);
	DispScore();
	DispShieldLvl();
	
	AstOut = 0;
				
	PlayerX   = 160 - (Ship[ShipFrame].Width >> 1);
	PlayerY   = 200 - Ship[ShipFrame].Height; 
	ShipFrame = 0; 
	Put(PlayerX, PlayerY, Ship[ShipFrame]);
	
	Retry = FALSE;
	Mouse_Status();
	Mouse_Movement();
}  

void GetHScore(char LevelChar)
{ 
	int i;
	
	ReadHScores("SPACERCK.DAT");
	if(HSScore[9] < ((Score - TIME_MASK) / 10))
	{
		char ScoreName[3];
			
		RLEDAT_GetPic(RLEDATName, StillPlanet, ENTERNAME);
		Put(80, 50, StillPlanet);
		FadeToPalette(GamePalette);                    
									
		for(i=0;i<3;i++)
		{
			while(!_kbhit());
			ScoreName[i] = _getch();
			if(ScoreName[i] == 8)			// BACKSPACE
			{
				if(i == 0)					// BEEP IF ON FIRST CHAR
				{
					i--;
					Sound(6818, 199940);		
				}     
				else
				{ 
					i -= 2;
					DispBigChar(' ', 184 + (8*(i+1)), 136, 252, 3);
				}
					
			}
			else
				DispBigChar(ScoreName[i], 184 + (8*i), 136, 252, 3);
		}
			
		AddHScore(ScoreName, (Score - TIME_MASK) / 10, LevelChar);
		WriteHScores("SPACERCK.DAT");					
	}
}



void FadeToBlack(void)
{
	int i, j;
	
	for(i=0;i<32;i++)
	{
		for(j=0;j<255;j++)
		{
			Read_Palette(j);
			
			if(Red > 5)
				Red -= 2;
			else
				Red = 0;
			
			if(Green > 5)
				Green -= 2;
			else
				Green = 0;
			
			if(Blue > 5)
				Blue -= 2;
			else
				Blue = 0;
			
			Set_Palette(Red, Green, Blue, j);
		}
		
		for(j=0;j<4;j++)
			Wait_Until_VRT();
	} 
}

void FadeToPalette(PALETTE Palette)
{
	int i, j;
	
	for(i=0;i<32;i++)
	{
		for(j=0;j<255;j++)
		{
			Read_Palette(j);
			
			if(Red < Palette.Red[j] - 5)
				Red += 2;
			else
				Red = Palette.Red[j];
			
			if(Green < Palette.Green[j] - 5)
				Green += 2;
			else
				Green = Palette.Green[j];
			
			if(Blue < Palette.Blue[j] - 5)
				Blue += 2;
			else
				Blue = Palette.Blue[j];
			
			Set_Palette(Red, Green, Blue, j);
		}
		
		for(j=0;j<4;j++)
			Wait_Until_VRT();
	}
}

void DispScore(void)
{
	DispBigText(StrScore, 242,5, 30, 4);		// print the score and the surrounding box
	Rect(240,3, 314,13, 20, NOFILL); 
}

void DispShieldLvl(void)
{
    Rect(240,15, 314             ,20, 20, NOFILL);		// display shield left outline
	Rect(241,16, 241 + ShieldLeft,19, 50, RECTFILL);	// display shield remaining
} 


void MsgBox(int X, int Y, char *ShowStr, int StrLen)
{
	Rect(X    , Y    , X + (StrLen * ROMCHARW) + 6, Y + ROMCHARH + 6, 20, NOFILL);
	Rect(X + 1, Y + 1, X + (StrLen * ROMCHARW) + 5, Y + ROMCHARH + 5, 25, NOFILL);
	Rect(X + 2, Y + 2, X + (StrLen * ROMCHARW) + 4, Y + ROMCHARH + 4, 20, NOFILL);
	
	DispBigText(ShowStr, X + 4, Y + 4, 30, 4);
}


void Rect(int StartX, int StartY, int EndX, int EndY, int Color, int Fill)
{
	if(Fill == RECTFILL)
	{
		int i;
		for(i=StartY;i<=EndY;i++)
			Horiz_Line(StartX, i, EndX, Color);
	}
	else
	{
		Horiz_Line(StartX, StartY, EndX, Color);
		Horiz_Line(StartX, EndY  , EndX, Color);
		Vert_Line (StartX, StartY, EndY, Color);
		Vert_Line (EndX  , StartY, EndY, Color);
	}	
}


void MoveStars(void)
{ 
	int i, ans;
		
	if(PointsOn)						// if points are shown, move them up
	{ 
		Horiz_Line(PointsX, PointsY + ROMSMALLH, PointsX + ROMSMALLW * PointsLen, BLACK);
		DispSmallText("500", PointsX, PointsY--, PointsColor--);
		if(PointsColor == 3)
		{
			DispSmallText("500", PointsX, ++PointsY, BLACK);
			PointsColor = 20;
			PointsOn = FALSE;
		}
			
	}
			
	for(i=0;i<NUM_STARS;i++)					// move the starfield
	{   
		ans = Read_Pixel(Stars[i].X, Stars[i].Y);
		if(ans > 0 && ans < 4)
			Set_Pixel(Stars[i].X, Stars[i].Y, BLACK);	
		Stars[i].Y += Stars[i].Speed;					
		if(Stars[i].Y > 199)
			Stars[i].Y -= 199;
		if(Read_Pixel(Stars[i].X, Stars[i].Y) == BLACK)
			Set_Pixel(Stars[i].X, Stars[i].Y, Stars[i].Speed);
	}
} 


void Initialize(void)
{
	int x;
	
	Set_Mode(VGA256);
	
	srand(1);										// select the star positions		
	for(x=0;x<NUM_STARS;x++)
	{
		Stars[x].X 	   = rand()%320 + 1;
		Stars[x].Y 	   = rand()%200 + 1;
		Stars[x].Speed = rand()%3   + 1;
	}
			
	for(x=0;x<MAX_ASTEROIDS;x++)					// select the asteroid positions
	{
		Rand_Asteroid(Asteroids[x], x);
		AstOut--;
		Asteroids[x].FrameCounter = 0; 
	}
		
	RLEDAT_GetPalette(RLEDATName, GamePalette);
		
	RLEDAT_GetPic(RLEDATName, Ship[0], SPWSHLD0);
	RLEDAT_GetPic(RLEDATName, Ship[1], SPWSHLD1);
	RLEDAT_GetPic(RLEDATName, Asteroid[0], ROCK1);
	RLEDAT_GetPic(RLEDATName, Asteroid[1], ROCK2);
	RLEDAT_GetPic(RLEDATName, Asteroid[2], ROCK3);
	RLEDAT_GetPic(RLEDATName, Asteroid[3], ROCK4);
	RLEDAT_GetPic(RLEDATName, Asteroid[4], ROCK5);
	RLEDAT_GetPic(RLEDATName, AstExpl[0], EXPL0);
	RLEDAT_GetPic(RLEDATName, AstExpl[1], EXPL1);
	RLEDAT_GetPic(RLEDATName, AstExpl[2], EXPL2);
	RLEDAT_GetPic(RLEDATName, AstExpl[3], EXPL3);
	RLEDAT_GetPic(RLEDATName, AstExpl[4], EXPL4);
	RLEDAT_GetPic(RLEDATName, TradeBack, STILLBCK);
	
	Mouse_Set_Sensitivity(20, 20, 64);
	
	MinX = 0;
	MaxX = 320 - Ship[0].Width;
	MinY = SCREEN_HEIGHT >> 1;
	MaxY = SCREEN_HEIGHT - Ship[0].Height;
	
	Clear_Screen(BLACK);
}


void MoveLaser(void)
{
	int x;
		
	Put_Blank(WeaponX, WeaponY, Laser, BLACK);
			
	WeaponY -= WEAPON_SPEED; 
		
	Put(WeaponX, WeaponY, Laser); 
		
	if(WeaponY <= 0 || (WeaponX >= 217 && WeaponY <= 30))	//if the lasers leave the screen...
	{
		Put_Blank(WeaponX, WeaponY, Laser, BLACK);
		WeaponOn = FALSE;            
	}
		
	for(x=0;x<NumAsteroids;x++)					// check for a laser hit
	{
		if((WeaponX + LASERW >= Asteroids[x].X) && (WeaponX <= Asteroids[x].X + ROCKW))
		{
		    if((WeaponY + LASERH >= Asteroids[x].Y) && (WeaponY <= Asteroids[x].Y + ROCKH))
			{
				if(Asteroids[x].ExplFrame == -1)
				{
					// clear the old asteroid and laser
					Put_Blank(WeaponX, WeaponY, Laser, BLACK);
					Put_Blank(Asteroids[x].X - Asteroids[x].Speed, Asteroids[x].Y, Asteroid[0], BLACK);
					WeaponOn = FALSE;
					
					// clear the old points											
					if(PointsOn)
						DispSmallText("   ", PointsX, PointsY, BLACK);
					
					// generate the new points position	
					PointsX  	= Asteroids[x].X;
					PointsY  	= Asteroids[x].Y;
					PointsColor = 20;
					PointsOn 	= TRUE;
						
					Asteroids[x].ExplFrame = 0;
						
					// increment the score
					UpdateScore(500);
				}	
			}
		}
	}
}


void Rand_Asteroid(ASTEROID &Ast, int AstNum)
{
	int j, NewCoor = 1; 
	
	Ast.Type  = rand()%ASTTYPES;
	Ast.Speed = rand()%3 - 1;				// generate AstSpeed either -1,0,1
	
	while(NewCoor)
	{
		NewCoor = 0;
		
		if(Ast.Speed == 0)
		{
			Ast.X = rand()%214;
			Ast.Y = 0; 
		}
		else
		{
			Ast.X = rand()%2;
			Ast.Y = rand()%179 + 21; 
			
			if(Ast.X == 0 && Ast.Speed < 0)			// if X is 0, the asteroid can't go left!
				Ast.Speed = -Ast.Speed;
	
			if(Ast.X == 1 && Ast.Speed > 0)			// if X is 1, the asteroid can't go right!
				Ast.Speed = -Ast.Speed;
	
			if(Ast.X == 1)							// the X is on the far left or right
				Ast.X = SCREEN_WIDTH - ROCKW;
		}
		
		for(j=0;j<NumAsteroids;j++)			// check for asteroid collision
		{
			if(Asteroids[j].ExplFrame == -1 && AstNum != j)
			{
				if((Ast.X + ROCKW >= Asteroids[j].X) && (Ast.X <= Asteroids[j].X + ROCKW))
				{
					if((Ast.Y + ROCKH >= Asteroids[j].Y) && (Ast.Y <= Asteroids[j].Y + ROCKH))
						NewCoor = 1;
				}
			}
		} 
	}
			
	
	if(TimeLeft == TIME_MASK)
	{
		Ast.ExplFrame = -999;
		if(++AstOut == NumAsteroids)
			NextLevel();
	}
	else
		Ast.ExplFrame = -1;	
}


void MoveAsteroids(void)
{ 
	int i, j, k;
		
	for(i=0;i<NumAsteroids;i++)			// cycles through the asteroids
	{
		if(Asteroids[i].ExplFrame == -1)	// if they are not exploding...
		{	
			if((Asteroids[i].X + ROCKW >= PlayerX) && (Asteroids[i].X <= PlayerX + Ship[ShipFrame].Width))
			{
				if((Asteroids[i].Y + ROCKH >= PlayerY) && (Asteroids[i].Y <= PlayerY + Ship[ShipFrame].Height))
					if(ShieldOn == FALSE)
					{
						Wait_Until_VRT();
						for(j=0;j<EXPLFRAMES;j++)
						{
							Put(PlayerX, PlayerY, AstExpl[j]);
							Put(PlayerX, PlayerY + ROCKH, AstExpl[j]);
							for(k=0;k<5;k++)
								Wait_Until_VRT();														
						} 
						
						FadeToBlack();
						Retry = TRUE; 
						
						if(NoDeath == FALSE)
						{
							--Lives;
							LivesStr[1]--;
						}
						
						if(Lives == 0)
						{
							GetHScore(LevelString[6]);
							MainMenu(); 
						}
												
						ShowNextBox(); 
						for(k=0;k<NumAsteroids;k++)
								Rand_Asteroid(Asteroids[k], k);
					}						
					else
					{
						Asteroids[i].ExplFrame = 0;
						Put(PlayerX, PlayerY, Ship[ShipFrame]); 
					}
			}
			
			Put(Asteroids[i].X, Asteroids[i].Y, Asteroid[Asteroids[i].Type]);
			
			if(Asteroids[i].Speed > 0)			// if they are going right...
			{
				Vert_Line (Asteroids[i].X, Asteroids[i].Y, Asteroids[i].Y + ROCKH, BLACK);
						
				if(Asteroids[i].X++ > SCREEN_WIDTH - ROCKW)
				{   
					Put_Blank(Asteroids[i].X - 1, Asteroids[i].Y, Asteroid[0], BLACK);
					Rand_Asteroid(Asteroids[i], i);
				}
			}
			
			if(Asteroids[i].Speed < 0)			// if they are going left...
			{
				Vert_Line (Asteroids[i].X + ROCKW, Asteroids[i].Y, Asteroids[i].Y + ROCKH, BLACK);
						
				if(Asteroids[i].X-- < 1)
				{   
					Put_Blank(Asteroids[i].X + 1, Asteroids[i].Y, Asteroid[0], BLACK);
					Rand_Asteroid(Asteroids[i], i);
				}                                    
			}
			
			Horiz_Line(Asteroids[i].X, Asteroids[i].Y, Asteroids[i].X + ROCKW, BLACK);
								
			if(Asteroids[i].Y++ > SCREEN_HEIGHT - ROCKH)	//if the Y exceeds boundaries
			{
				Put_Blank(Asteroids[i].X - Asteroids[i].Speed, Asteroids[i].Y, Asteroid[0], BLACK);
				Rand_Asteroid(Asteroids[i], i);
			}
			
			for(j=0;j<NumAsteroids;j++)			// check for asteroid collision
			{
			if(Asteroids[j].ExplFrame == -1 && i != j)
			{
			if((Asteroids[i].X + ROCKW >= Asteroids[j].X) && (Asteroids[i].X <= Asteroids[j].X + ROCKW))
			{
				if((Asteroids[i].Y + ROCKH >= Asteroids[j].Y) && (Asteroids[i].Y <= Asteroids[j].Y + ROCKH))
				{
					Asteroids[i].ExplFrame = 0;
					Asteroids[j].ExplFrame = 0;
				}
			}
			}
			}
		}
		
		if(Asteroids[i].ExplFrame > -1)				// show the explosion!!!
		{   
		    Asteroids[i].FrameCounter++;			// increment the frame speed
		    if(Asteroids[i].FrameCounter == 4)		// every 4 frame speed cycles...
		    {
		  	 	// display the next frame!
		  	 	Put(Asteroids[i].X, Asteroids[i].Y, AstExpl[Asteroids[i].ExplFrame]);
		    	Asteroids[i].ExplFrame++;
				
				// if it's the last frame...
				if(Asteroids[i].ExplFrame == EXPLFRAMES)
				{
					Put_Blank(Asteroids[i].X, Asteroids[i].Y, Asteroid[0], BLACK);
					Rand_Asteroid(Asteroids[i], i);
				} 
				
				Asteroids[i].FrameCounter = 0;		// reset the frame speed counter
			}
		}
	}
}


void Move_Player(void)
{
	Put_Blank(PlayerX, PlayerY, Ship[ShipFrame], BLACK);
	
	PlayerX += Mouse_MickeyX;				// change the players position
	PlayerY += Mouse_MickeyY;
		
	if(PlayerX < MinX)							// check screen boundaries...
		PlayerX = MinX;
	if(PlayerX > MaxX)
		PlayerX = MaxX;      
	if(PlayerY < MinY)
		PlayerY = MinY;
	if(PlayerY > MaxY)
		PlayerY = MaxY;	
		
	Put(PlayerX, PlayerY, Ship[ShipFrame]);
}


void Fire_Weapon(void)
{
	if(Mouse_Button_Status == 1)			// if the left mouse button...
	{
		if(!WeaponOn && LaserWait == 0)		// and the weapon is not already firing...
		{
			WeaponX   = PlayerX + (Ship[ShipFrame].Width >> 1) - (WEAPW >> 1);
			WeaponY   = PlayerY - WEAPH;
			WeaponOn  = TRUE;
			LaserWait = 1;
	
			SoundStatus = 1200;
		}
	}
	
	if(Mouse_Button_Status == 2 && ShieldLeft > 0)
	{
		ShieldOn  = TRUE;
		ShieldRep = 0;
		ShipFrame = 1;
		Put(PlayerX, PlayerY, Ship[ShipFrame]);
	}
}


void UpdateScore(long Change)
{
	Change = (Change << 3) + (Change << 1);			//Change *= 10;
	Score += Change;
					
	_ltoa(Score, StrScore, 10);						//convert to string
	StrScore[0] = '0';
	StrScore[9] = NULL;
	
	DispScore();
}


void Change_Time(unsigned int new_time)			// change the internal timer
{
	_asm
	{
		mov	AL,3Ch				// Set the control word
		out	43h,AL
		
		mov	AX,new_time
		shr	AX,4
		out	40h,AL				// Send the low byte
		shr	AX,4
		out 40h,AL				// Send the high byte
	}
}


// display a string of small characters
void DispSmallText(char String[], int X, int Y, int Color)
{
	int i;
	for(i=0;String[i]!=NULL;i++)
		DispSmallChar(String[i], X + (i << 2) + i, Y, Color);
}


// display a small character from ROM
void DispSmallChar(unsigned char Char, int X, int Y, int Color)
{
	int VidOff, i, j, pix = 0;
	unsigned char far *ROMOff;
	unsigned char bit_mask = 0x80;
	
	ROMOff = rom_chars + Char * ROMCHARH;		// figure out offsets of ROM and screen
	VidOff = (Y << 8) + (Y << 6) + X;
			
	for(j=0;j<ROMCHARH;j++)
	{
			bit_mask = 0x80;
			
		if(j != 1 && j != 5)							// omit some horizontal lines
		{
			for(i=0;i<ROMCHARW;i++)
			{
				if(i != 1 && i != 3 && i != 5)			// omit some vertical lines
				{
					if((*ROMOff & bit_mask))
						video_buffer[VidOff + pix] = Color;
					else
						video_buffer[VidOff + pix] = BLACK;
					pix++;
			    } 
			    bit_mask = (bit_mask >> 1);				// shift the bitmask
			}
			
			VidOff += SCREEN_WIDTH;
			pix = 0;
		}
		
		ROMOff++;
	}
}


// display a regular ROM character 
void DispBigChar(unsigned char Char, int X, int Y, int Color, int BackColor)
{
	int VidOff, i, j;
	BUFFER ROMOff;
	unsigned char bit_mask = 0x80;
	
	ROMOff = rom_chars + Char * ROMCHARH;		// compute ROM and screen offsets
	VidOff = (Y << 8) + (Y << 6) + X;
			
	for(j=0;j<ROMCHARH;j++)
	{
		bit_mask = 0x80;
			
		for(i=0;i<ROMCHARW;i++)
		{
			if((*ROMOff & bit_mask))				// if there is a 1 in the bit...
				video_buffer[VidOff + i] = Color;
			else
				video_buffer[VidOff + i] = BackColor;
			 
	    	bit_mask = (bit_mask >> 1);
		}
			
		VidOff += SCREEN_WIDTH;					// increase the ROM and screen offsets
		ROMOff++;
	}
}


// display a regular ROM character string
void DispBigText(char *String, int X, int Y, int Color, int BackColor)
{ 
	int i;
	for(i=0;String[i]!=NULL;i++)
		DispBigChar(String[i], X + (i << 3), Y, Color, BackColor);
}		

