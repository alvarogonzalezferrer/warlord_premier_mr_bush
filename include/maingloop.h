// ---------------------------------------------------------------------------
// maingloop.h
// -----------------------------------------------------------------------
// Main Game Loop
// Based on the generic game machine
// -----------------------------------------------------------------------
// Copyright (c) 2007, Kronoman
// In loving memory of my father.
// Made in Argentina
// -----------------------------------------------------------------------
// Released under The MIT License.
// -----------------------------------------------------------------------
#ifndef MAINGLOOP_H
#define MAINGLOOP_H

#include "gamemachine.h"
#include "map.h"
#include "weather.h"
#include "baby.h"
#include "particle.h"
#include "datafile.h"

class MainGameLoop : public GameMachine
{
	public:
		MainGameLoop();
		~MainGameLoop();
						
		void update(); ///< updates the game, will return states if needed... 
		void render(); ///< renders what is happening

		static const unsigned int STATE_NEXTLEVEL = 2;
		static const unsigned int STATE_INTRONEXTLEVEL = 3;
		static const unsigned int STATE_INTRO = 4; 
		static const unsigned int STATE_GAMEOVER = 5; 

		int wait; // delay for various screen pauses...
		int camerashake; // delay for camera shake effects...

		// this would be the 'world'
		Map *map; // map
		Weather weather; // weather of the map
		BabyList babyL; // baby list
		ParticleManager particleM; // particles

		// player shit, I should have done a proper class, but no time... no class...
		int w_x, w_y; // weapon x,y
		int w_t; // weapon time

		int grenades; // grenades left
		int g_t; // grenade reload time

		int airstrike; // airstrike left
		int a_t; // airstrike time
		int airstrike_t; // airstrike dialog stake... 0 (none) to 10 (do it)

		int life; // premier bush's life, recharges with baby blood after each level
		int blood; // blood harvested in this level, will add to life after each level
		
		int level; // level

		unsigned long int money; // money burnt on war, sorta score


		// data stuff
		Datafile data; // some data, like warlord pic, sounds, font, etc...
		BITMAP *warlord;

		BITMAP *condor; // secretary

		BITMAP *gameoverbmp;
		BITMAP *introbmp;
		BITMAP *nextbmp;

		SAMPLE *shootSND;
		SAMPLE *grenadeSND;
		SAMPLE *airstrikeSND;

};

#endif

