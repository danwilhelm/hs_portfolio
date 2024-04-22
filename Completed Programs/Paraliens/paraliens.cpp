/****************************************************************************
	Paraliens.cpp -- Authored by Daniel Wilhelm for the
	Computer Game Creation Team at Columbus North High School

	Last revision date: 12/04/00

	Notes to self: (None)
****************************************************************************/

#include "paraliens.h"
//#define  DEBUG

int Init(void)
{
	int i, j;

	srand(GetTickCount());
	
	MyResources();
	
	#ifndef DEBUG
	CreateFullScreen(WIN_TITLE);
	#else
	CreateWindowed(WIN_TITLE, 0, 0, 646, 504);
	#endif

/////////////////////////////////////////////////////////////////////////////
// INITIALIZE DIRECTX
/////////////////////////////////////////////////////////////////////////////

	if(xmedia.Create())
		xmError("DirectX 7.0 or higher is required.");

	#ifndef DEBUG
	if(xmedia.SetMode(&screen, XRES, YRES, BPP, BACKBUFFERS))
		xmError("Cannot set 640x480x16 video mode.");
	#else
	// Windowed mode only!
	primary.CreatePrimary(&xmedia, 0);
	screen.CreateSurface(&xmedia, XRES, YRES, primary.bpp);
	#endif

	//primary.Clip(cliplist, cliplistlen);
	screen.Clip(cliplist, cliplistlen);

/////////////////////////////////////////////////////////////////////////////
// LOAD AND INITIALIZE MUSIC DATA
/////////////////////////////////////////////////////////////////////////////
	
	for(i=0;i<musiclistlen;i++)
		if(musiclist[i].musicptr->Load(&xmedia, musiclist[i].filename))
			xmError("Failed to load music");

/////////////////////////////////////////////////////////////////////////////
// LOAD AND INITIALIZE SOUND DATA
/////////////////////////////////////////////////////////////////////////////
	
	for(i=0;i<soundlistlen;i++)
		if(soundlist[i].soundptr->Load(&xmedia, soundlist[i].filename))
			xmError("Failed to load sound");

/////////////////////////////////////////////////////////////////////////////
// LOAD AND INITIALIZE GRAPHICS DATA
/////////////////////////////////////////////////////////////////////////////
	
	for(i=0;i<graphicslistlen;i++)
		if(graphicslist[i].surfaceptr->Load(&xmedia, graphicslist[i].filename))
			xmError("Failed to load graphics");

/////////////////////////////////////////////////////////////////////////////
// LOAD AND INITIALIZE ANIMATION DATA
/////////////////////////////////////////////////////////////////////////////

	for(i=0;i<animationlistlen;i++)
	{
		CAnimation *aniptr = animationlist[i].animationptr;

		aniptr->attanimation = animationlist[i].attanimptr;
		aniptr->LogicFunc  = animationlist[i].Logic;
		aniptr->numframes  = animationlist[i].maxframes;

		for(j=0;j<aniptr->numframes;j++)
			aniptr->surface[j] = &animationlist[i].surfaceptr[j];

		aniptr->width  = aniptr->surface[0]->Width();
		aniptr->height = aniptr->surface[0]->Height();
	}

	scorecolor = RGB(100, 200, 0);

	UpdateScore(STARTSCORE);

	return 0;
}


int Main(void)
{
	BOOL keypressed = FALSE;
	int i;

	xmedia.GetInput();

	screen.Clear();

	switch(gamestate)
	{
		// inside the game
		case(STATE_PLAY):
		break;
			
		case(STATE_NEWGAME):
			RenderWorld();
			GameInput();
		break;

		case(STATE_EXIT):
		break;

		case(STATE_INTRO):
			/*intromusic.Play();
			screen.Blit(&cgctlogo, 0, 0);
			primary.Blit(&screen, win_windowx, win_windowy, BLIT_OPAQUE);
			
			while(keypressed == FALSE)
			{
				xmedia.GetInput();

				for(i=0;i<0x66;i++)
					if(ButtonDown(i))
						keypressed = TRUE;
			};

			screen.Blit(&mainmenu, 0, 0);
			primary.Blit(&screen, win_windowx, win_windowy, BLIT_OPAQUE);

			Sleep(2000);
			
			while(keypressed == FALSE)
			{
				xmedia.GetInput();

				for(i=0;i<0x66;i++)
					if(ButtonDown(i))
						keypressed = TRUE;
			};*/
		
			gamestate = STATE_NEWGAME;
		break;

		case(STATE_MAINMENU):
			RenderMenu();
		break;

		case(STATE_OPTIONS):
		break;
		
		case(STATE_HIGHSCORES):
		break;

		case(STATE_CREDITS):
		break;
	}
	
	#ifndef DEBUG
	xmedia.Flip();
	#else
	// double buffer to the primary
	primary.Blit(&screen, 10,10, BLIT_OPAQUE);//win_windowx, win_windowy, BLIT_OPAQUE);
	#endif

	keydelay++;

	return 0;
}


int Shutdown(void)
{    
 	int i;
	
	// release the music
	for(i=0;i<musiclistlen;i++)
		musiclist[i].musicptr->Release();

	// release the sound
	for(i=0;i<soundlistlen;i++)
		soundlist[i].soundptr->Release();

	// release the graphics
	for(i=0;i<graphicslistlen;i++)
		graphicslist[i].surfaceptr->Release();

	// release the double buffer
	screen.Release();

	// finally, release DirectX
	xmedia.Release();

	return 0;
}


void RenderMenu(void)
{
	int i;
	
	screen.Blit(&mainmenu, 0, 0, BLIT_OPAQUE);
	
	int btnstarty = (YRES / 2) - (mainmenubtn[0].Height() * MAX_MENUBTNS / 2);
	int btnstartx = (XRES / 2) - (mainmenubtn[0].Height() / 2);
	
	for(i=0;i<MAX_MENUBTNS;i++)
		screen.Blit(&mainmenubtn[i], btnstartx, mainmenubtn[0].Height() * i + btnstarty);
	
	if(ButtonDown(DIK_ESCAPE) || ButtonDown(MOUSE_RIGHT))
		gamerunning = FALSE;
}


void RenderWorld(void)
{
	ANIMLIST::iterator i, lasti = 0;
	
	if(++shipcounter > maxshipcounter)//rand()%shipprob == 0)
	{
		CAnimation tempship = shipani[rand()%MAX_SHIPS];
		
		tempship.x = -SHIPX;
		tempship.y = 0;

		tempship.x0 = 2;
		tempship.y0 = 0;
		
		renderlist.push_back(tempship);

		shipcounter = 0;
	}

	// display the background
	screen.Blit(&background[backgroundnum], 0, 0, BLIT_OPAQUE);
	screen.Blit(&crosshair[crosshairnum][0], crossx, crossy);

	// loop through each list item and render each to the screen + do logic
	for(i=renderlist.begin();i!=renderlist.end();++i)
	{
		if(lasti != 0)
			renderlist.erase(lasti);
		
		// if the return value is LIST_REMOVE, remove the item from the rendering list!
		if(i->Animate(&screen) == LIST_REMOVE)
			lasti = i;
		else
			lasti = 0;
	}

	if(lasti != 0)
		renderlist.erase(lasti);

	if(gamemusic[currmusic].IsPlaying())
	{
		currmusic = rand()%MAX_MUSIC;
		
		gamemusic[currmusic].Play();
	}

	screen.Text(SCOREX, SCOREY, scorestr, scorecolor);
}


void LoadWeapon(int weapon)
{
	if(level >= weapon)
	{
		weaponsnd[weapon][WEAPON_LOAD].Play();
		currweapon = weapon;
	}
	else
		weaponsnd[0][0].Play();
}


int GameInput(void)
{
	if(keydelay >= maxkeydelay)
	{
		// exit to the main menu
		if(ButtonDown(DIK_ESCAPE) || ButtonDown(MOUSE_RIGHT))
			gamestate = STATE_MAINMENU;

		// Fire!
		if((ButtonDown(DIK_SPACE) || ButtonDown(MOUSE_LEFT)))
		{	
			weaponsnd[currweapon][WEAPON_FIRE].Play();
			
			// copy the current weapon type
			CAnimation tempweapon = weaponani[currweapon];
			
			// calculate the slope of the weapon (flip because this screen is non-Cartesean)
			tempweapon.x0 = -(WEAPONSPEED * cos(crossangle));
			tempweapon.y0 = -(WEAPONSPEED * sin(crossangle));

			// calculate the initial (x,y) of the weapon
			tempweapon.x  = TURRETX;
			tempweapon.y  = TURRETY - CROSSRADIUS;

			// ... and now push it onto the list of things to draw
			renderlist.push_back(tempweapon);

			keydelay = 0;

			UpdateScore(WEAPONCOST[level]);
		}

		if(ButtonDown(DIK_1) || ButtonDown(DIK_NUMPAD1)) LoadWeapon(0);
		if(ButtonDown(DIK_2) || ButtonDown(DIK_NUMPAD2)) LoadWeapon(1);
		if(ButtonDown(DIK_3) || ButtonDown(DIK_NUMPAD3)) LoadWeapon(2);
		if(ButtonDown(DIK_4) || ButtonDown(DIK_NUMPAD4)) LoadWeapon(3);
		if(ButtonDown(DIK_5) || ButtonDown(DIK_NUMPAD5)) LoadWeapon(4);
		if(ButtonDown(DIK_6) || ButtonDown(DIK_NUMPAD6)) LoadWeapon(5);
		if(ButtonDown(DIK_7) || ButtonDown(DIK_NUMPAD7)) LoadWeapon(6);

		//if(ButtonDown(DIK_8) || ButtonDown(DIK_NUMPAD8)) LoadWeapon(7);
		//if(ButtonDown(DIK_9) || ButtonDown(DIK_NUMPAD9)) LoadWeapon(8);
	}

	// Move the crosshair to the left
	if((ButtonDown(DIK_RIGHT) || GetXAxis > 0) && crossangle < MAX_ANGLE)
	{
		// increase the angle
		crossangle += CROSSANGINC;

		// calculate the new (x, y) position using trig
		crossx = TURRETX - CROSSRADIUS * cos(crossangle);// * WORLDSCALE);
		crossy = TURRETY - CROSSRADIUS - CROSSRADIUS * sin(crossangle);// * WORLDSCALE);
	}

	// Move the crosshair to the right
	if((ButtonDown(DIK_LEFT) || GetXAxis < 0) && crossangle > MIN_ANGLE)
	{
		crossangle -= CROSSANGINC;

		crossx = TURRETX - CROSSRADIUS * cos(crossangle);// * WORLDSCALE);
		crossy = TURRETY - CROSSRADIUS - CROSSRADIUS * sin(crossangle);// * WORLDSCALE);
	}

	return 0;
}


int AlienLogic(CAnimation *aniptr)
{
	// drop the alien (note: aliens should never leave the screen!)
	if(aniptr->state == ALIEN_DROPPING)
	{
		if(aniptr->y >= ALIEN_MAXY)
		{
			if(aniptr->y0 == alienfallspeed)
				return LIST_REMOVE;

			aniptr->y0 = 0;

			groundaliens++;
		}
	}

	return 0;
}


int ParachuteLogic(CAnimation *aniptr)
{
	// drop the parachute (note: parachutes should never leave the screen!)
	if(aniptr->state == ALIEN_DROPPING)
	{
		if(aniptr->y >= ALIEN_MAXY)
			return LIST_REMOVE;
	}

	return 0;

}


int WeaponLogic(CAnimation *aniptr)
{
	ANIMLIST::iterator i;
	
	// test to see if the weapon has left the horizontal screen
	if(aniptr->x < -aniptr->width || aniptr->x > XRES)
		return LIST_REMOVE;

	// test to see if the weapon has left the vertical screen
	if(aniptr->y < -aniptr->height || aniptr->y > YRES)
		return LIST_REMOVE;

	// now check with every alien to see if any were hit:
	for(i=renderlist.begin();i!=renderlist.end();++i)
	{
		if(aniptr->x != i->x && aniptr->y != i->y)
		if(aniptr->x < i->x + i->width && aniptr->x + aniptr->width > i->x)
		{
			if(aniptr->y < i->y + i->height && aniptr->y + aniptr->height > i->y)
			{
				weaponsnd[currweapon][WEAPON_IMPACT].Play();

				if(i->LogicFunc == ParachuteLogic)
				{
					i--;

					if(i->LogicFunc == AlienLogic && i->y0 > 0)
						i->y0 = alienfallspeed;

					i++;
				}

				// now assign points for hitting things!
				if(i->LogicFunc == AlienLogic)     UpdateScore(SCORE_ALIEN); 
				if(i->LogicFunc == WeaponLogic)    UpdateScore(SCORE_WEAPON);
				if(i->LogicFunc == ShipLogic)	   UpdateScore(SCORE_SHIP);	
				if(i->LogicFunc == ParachuteLogic) UpdateScore(SCORE_PARACHUTE);

				renderlist.erase(i);

				return LIST_REMOVE;
			}
		}
	}

	return 0;
}


int ShipLogic(CAnimation *aniptr)
{
	// test to see if the ship has left the horizontal screen
	if(aniptr->x < -aniptr->width || aniptr->x > XRES)
		return LIST_REMOVE;

	// test to see if the ship has left the vertical screen
	if(aniptr->y < -aniptr->height || aniptr->y > YRES)	
		return LIST_REMOVE;

	// random code to drop aliens here:
	if(aniptr->x >= 0 && aniptr->x < XRES - ALIENX && rand()%dropalienprob == 0)
	{
		// copy an alien image
		CAnimation tempalien = alienani[rand()%MAX_ALIENS];

		tempalien.x = aniptr->x;
		tempalien.y = aniptr->y;

		tempalien.x0 = 0;
		tempalien.y0 = rand()%maxalienspeed+1;
				
		renderlist.push_back(tempalien);

		// copy a parachute image
		CAnimation temppara = paraani[0];

		temppara.x = aniptr->x ;
		temppara.y = aniptr->y - ALIENX + 15;

		temppara.x0 = 0;
		temppara.y0 = tempalien.y0;
	
		renderlist.push_back(temppara);
	}

	return 0;
}


int CrossHairLogic(CAnimation *aniptr)
{
	// Move the crosshair to the left
	if((ButtonDown(DIK_RIGHT) || GetXAxis > 0) && crossangle < MAX_ANGLE)
	{
		// increase the angle
		crossangle += CROSSANGINC;

		// calculate the new (x, y) position using trig
		aniptr->x = TURRETX - CROSSRADIUS * cos(crossangle);// * WORLDSCALE);
		aniptr->y = TURRETY - CROSSRADIUS - CROSSRADIUS * sin(crossangle);// * WORLDSCALE);
	}

	// Move the crosshair to the right
	if((ButtonDown(DIK_LEFT) || GetXAxis < 0) && crossangle > MIN_ANGLE)
	{
		crossangle -= CROSSANGINC;

		aniptr->x = TURRETX - CROSSRADIUS * cos(crossangle);// * WORLDSCALE);
		aniptr->y = TURRETY - CROSSRADIUS - CROSSRADIUS * sin(crossangle);// * WORLDSCALE);
	}

	return 0;
}


void UpdateScore(int points)
{
	int i;
	
	score += points;
	itoa(score, scorestr, 10);	

	// has player advanced or decreased a level?
	for(i=0;i<MAX_LEVELS;i++)
	{
		if(i > level && score > LEVELSCORE[i])
		{
			level++;
			LoadWeapon(level);
		}
		
		if(i < level && score < LEVELSCORE[i])
		{
			level--;			
			LoadWeapon(level);
		}
	}

}



