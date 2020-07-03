// -----------------------------------------------------------------------
// baby.h
// -----------------------------------------------------------------------
// Baby
// -----------------------------------------------------------------------
// Copyright (c) 2007, Kronoman
// In loving memory of my father.
// Made in Argentina
// -----------------------------------------------------------------------
// Released under The MIT License.
// -----------------------------------------------------------------------
#ifndef BABY_H
#define BABY_H

#include <allegro.h>
#include "map.h"
#include "datafile.h"
#include "particle.h"

class Baby
{
	public:
		Baby();
		~Baby();
	
		void update(Map *m);
		void render(BITMAP *bmp);
	
		float x,y;
		float sx,sy;

		int life;
		int state;
		bool jibbed; // dead by grenade?
		
		int anm; // animation frame
		BITMAP *sprite; // current sprite
	
		// for all
	
		static const int LEFT=0;
		static const int RIGHT=1;
		// TODO - i think this 3 states aren't used... no time to finish
		static const int JUMP=2;
		static const int DYING=3;
		static const int SUCESS=4; // managed to reach other side
	
		static BITMAP *walk[3];
		static BITMAP *jib[3];
		static BITMAP *dead;

		static SAMPLE *babySND;

		static Datafile data;
		static int count;
};

class BabyList
{
	public:
		BabyList();
		~BabyList();

		void generate(int i);
		int update(Map *m); // returns how many baby are alive yet...
		void render(BITMAP *bmp);
		
		int shoot(int x, int y, ParticleManager *pm); // returns how many life we leech
		int grenade(int x, int y, ParticleManager *pm); // returns how many life we leech

		static const int maxBL = 100;

		Baby *baby[maxBL];
		int n; // how much we have now
};


#endif

