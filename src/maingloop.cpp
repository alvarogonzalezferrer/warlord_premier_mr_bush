// -----------------------------------------------------------------------
// maingloop.cpp
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
#include <allegro.h>
#include "maingloop.h"
#include "error.h"


MainGameLoop::MainGameLoop()
{
	map = NULL;
	wait = 0;
	data.load("warlord.dat");
	camerashake = 0;
}

MainGameLoop::~MainGameLoop()
{
	if (map != NULL)
		delete(map);
	
	data.unload();
}


void MainGameLoop::update()
{
	int c = level+3; // how many babies to add to level?
	// this would handle the states...
	switch (currentState)
	{
	case STATE_START:
		camerashake = 0;
		// I re create the bitmap as a 640x480 only, to support any resolution
		if (doubleBuffer)
			destroy_bitmap(doubleBuffer); // bitmap
		doubleBuffer = create_bitmap(640, 480);
		if (doubleBuffer == NULL)
			Error::error("MainGameLoop -> STATE_START\nERROR: can't get a bitmap %d x %d px", SCREEN_W, SCREEN_H);
		
		// load shit
		warlord=(BITMAP *)data.getObject("WARLORD")->dat;
		condor=(BITMAP *)data.getObject("CONDOR")->dat;
	
		gameoverbmp=(BITMAP *)data.getObject("GAMEOVER")->dat;
		introbmp=(BITMAP *)data.getObject("INTRO")->dat;
		nextbmp=(BITMAP *)data.getObject("NEXTLEVEL")->dat;

		shootSND=(SAMPLE *)data.getObject("SHOOTSND")->dat;
		grenadeSND=(SAMPLE *)data.getObject("GRENADESND")->dat;
		airstrikeSND=(SAMPLE *)data.getObject("AIRSTRIKESND")->dat;


		// set up some start vars
		airstrike = 5; // airstrike left (for the WHOLE GAME)
		blood = 0;
		life = 2000; //  live/30 = seconds
		level = 0;
		wait=300;
		money = rand()%90+10;
		changeState(STATE_INTRO);
	break;

	case STATE_INTRO:
		camerashake = 0;

		if (key[KEY_SPACE])
			wait = 0;

		if (--wait < 0)
			changeState(STATE_NEXTLEVEL);
	break;

	case STATE_GAMEOVER:
		camerashake = 0;

		if (key[KEY_SPACE])
		{
			if (wait > 64)
				wait = 64;
		}

		if (--wait < 0)
			changeState(STATE_START);
	break;


	case STATE_NEXTLEVEL:
		camerashake = 0;
		level++;
		// map
		if (map == NULL)
			map = new Map();
		map->generate();
		// end map
		weather.generate(map->tw); // set weather

		if (c>BabyList::maxBL-1)
			c=BabyList::maxBL-1;
		babyL.generate(c); // generate baby

		grenades = 3;
		particleM.finish(); //empty particles
		w_x = w_y = w_t = g_t = a_t = airstrike_t = 0;

		wait = 150;
		
		life += blood;
		blood = 0;

		changeState(STATE_INTRONEXTLEVEL);

	break;

	case STATE_INTRONEXTLEVEL:		
		if (key[KEY_SPACE])
		{
			clear_keybuf();
			if (wait > 32)
				wait = 32;
			else
				wait = 0;
		}

		if (--wait < 0)
			changeState(STATE_OK);
	break;

	case STATE_OK:
		// main state of game		
		if (key[KEY_K] && key[KEY_R] && key[KEY_O]) // KRO = cheat code to skip level
		{
			changeState(STATE_NEXTLEVEL);
			while (key[KEY_K] && key[KEY_R] && key[KEY_O]);
		}

		weather.update();


		if (babyL.update(map) < 1)
			changeState(STATE_NEXTLEVEL); // GREAT! next level!

		// weapon
		w_x = mouse_x*doubleBuffer->w/SCREEN_W; // translate mouse to screen coordinates (to support multiple video)
		w_y = mouse_y*doubleBuffer->h/SCREEN_H;
		if (w_t > 0)
			w_t--; // reload weapon

		if (g_t > 0)
			g_t--; // reload grenade

		if (a_t > 0)
			a_t--; // reload airstrike

		if (mouse_b & 1 && w_t < 1) // shoot weapon
		{
			play_sample(shootSND, rand()%55+200, w_x*255/doubleBuffer->w, rand()%400+950, 0);
			w_t = 15 + rand()%5;
			blood += babyL.shoot(w_x, w_y, &particleM);

			// sparks
			for (int i =0;i < rand()%15+25; i++)
				particleM.add(new Spark(w_x, w_y, (float)(rand()%200-100)/100.0f, (float)(rand()%200-100)/100.0f, rand()%10+15, makecol(rand()%55+200,255,0)));
		}


		if (mouse_b & 2 && g_t < 1 && grenades > 0) // shoot grenade
		{
			play_sample(grenadeSND, 255, w_x*255/doubleBuffer->w, rand()%500+700, 0);
			camerashake  = rand()%15+5;

			grenades--;
			g_t = 60 + rand()%25;
			blood += babyL.grenade(w_x, w_y, &particleM);

			// sparks
			for (int i =0;i < rand()%115+125; i++)
			{
				particleM.add(new Spark(w_x + rand()%50-25, w_y + rand()%50-25, (float)(rand()%600-300)/100.0f, (float)(rand()%600-300)/100.0f, rand()%50+25, makecol(rand()%55+200,255,0)));
				particleM.add(new Circle(w_x , w_y , (float)(rand()%400-200)/100.0f, (float)(rand()%400-200)/100.0f, rand()%35+15, makecol(rand()%55+200,128,0), rand()%4+1));
			}

			for (int i =0;i < rand()%15+25; i++)
				particleM.add(new Smoke(w_x , w_y , (float)(rand()%400-200)/100.0f, (float)(rand()%400-200)/100.0f, rand()%25+15, makecol(rand()%55+128,128,0), rand()%3+1, rand()%2+1));

			for (int i =0;i < rand()%15+25; i++)
				particleM.add(new Smoke(w_x+rand()%12-6 , w_y+rand()%12-6 , (float)(rand()%400-200)/100.0f, (float)(rand()%400-200)/100.0f, rand()%25+15, makecol(rand()%55+128,0,0), rand()%2+1, rand()%2+1));
		}

		// airstrike
		if ((mouse_b & 4 || key[KEY_ENTER]) && a_t < 1 && airstrike_t == 0 && airstrike > 0)
		{
			airstrike_t = 1; // carry on with dialog	
		}

		// airstrike dialog
		if (airstrike_t > 0)
		{
			if (key[KEY_N])
				airstrike_t = 0; // cancel the airstrike

			if (key[KEY_Y])
			{
				airstrike_t +=rand()%2+1; // go fordward, we may skip dialog, to make it more random...
				clear_keybuf();
				while (key[KEY_Y]);
			}

			if (airstrike_t > 10)
			{
				airstrike_t = 0;
				a_t = 200+rand()%60;
				airstrike--;
				money *= rand()%3+2; // costs $$$

				camerashake  = rand()%45+50;

				play_sample(airstrikeSND, 255, 128, 1000, 0);

				// do the strike
				unsigned int xx=rand()%256+32;
				int yy;
				while (xx < Map::mapW)
				{
					yy = map->getHeight(xx) + rand()%50-25;
					blood += (int)(babyL.grenade(xx, yy, &particleM)*1.15); // 15 % bonus for airstrike
					
					// explosion effect
					play_sample(grenadeSND, rand()%64+128, xx*255/doubleBuffer->w, rand()%500+900, 0);

					for (int i =0;i < rand()%100+80; i++)
					{
						particleM.add(new Spark(xx + rand()%50-25, yy + rand()%50-25, (float)(rand()%400)/-100.0f, (float)(rand()%600-300)/100.0f, rand()%50+95, makecol(rand()%55+200,255,0)));
						particleM.add(new Circle(xx , yy , (float)(rand()%400-200)/100.0f, (float)(rand()%400)/-100.0f, rand()%35+95, makecol(rand()%55+200,128,0), rand()%4+1));
					}
		
					for (int i =0;i < rand()%15+25; i++)
						particleM.add(new Smoke(xx , yy , (float)(rand()%400-200)/100.0f, (float)(rand()%400-200)/100.0f, rand()%25+25, makecol(rand()%55+128,128,0), rand()%3+1, rand()%2+1));
		
					for (int i =0;i < rand()%15+25; i++)
						particleM.add(new Smoke(xx+rand()%12-6 , yy+rand()%12-6 , (float)(rand()%400-200)/100.0f, (float)(rand()%400+200)/-100.0f, rand()%25+20, makecol(rand()%55+128,0,0), rand()%2+1, rand()%2+1));

					xx += rand()%256+128;
				}
			}

		}

		// end weapon
		

		particleM.update();

		// life
		life--;
		if (life < 0) // game over!
		{
			wait=600;
			changeState(STATE_GAMEOVER);
		}
		
		money += rand()%10+10; // " score "
	break;

	default:
		// probably a error if we go here... a unhandled state... mmm
	break;
	}

	
	if (key[KEY_F10]) // panic key to exit...
		changeState(STATE_QUIT);

	
	if (key[KEY_F11])  // change video mode, will reload graphics, because of bpp change
	{
		int card = GFX_AUTODETECT;
		int w = SCREEN_W;
		int h = SCREEN_H;
		int cd = get_color_depth();
 		show_mouse(screen);
 		unscare_mouse();
		if (gfx_mode_select_ex(&card, &w, &h, &cd) != 0 && cd > 8)
		{
 			set_color_depth(cd);
      		if (set_gfx_mode(card, w,h, 0, 0) != 0)
         		Error::error("Error!\nCannot set video mode %d x %d @ %dbpp", w, h, cd);

			data.load("warlord.dat"); // RELOAD THIS DAMN DATA

			changeState(STATE_START); // RESTART GAME... sorry but the bitmaps get fkc up otherwise...
		}
		show_mouse(NULL);
	}

	if (camerashake > 0)
		camerashake --;

}

void MainGameLoop::render()
{
	switch (currentState)
	{
	case STATE_START:
	break;

	case STATE_INTRO:
		stretch_blit(warlord, doubleBuffer, 0, 0, warlord->w, warlord->h, 0, 0, doubleBuffer->w, doubleBuffer->h);
		masked_stretch_blit(introbmp, doubleBuffer, 0, 0, introbmp->w, introbmp->h, introbmp->w/5, introbmp->h/2, introbmp->w*2, introbmp->h*2);
		textprintf_ex(doubleBuffer, font,0,doubleBuffer->h-text_height(font)*2,makecol(128,128,128), -1, "Kronoman 2007 - Speed Hack");

		// TODO sound...
	break;

	case STATE_GAMEOVER:
		stretch_blit(warlord, doubleBuffer, 0, 0, warlord->w, warlord->h, 0, 0, doubleBuffer->w, doubleBuffer->h);

		drawing_mode(DRAW_MODE_TRANS, NULL,0,0); 
		rectfill(doubleBuffer, 0, 0, doubleBuffer->w, doubleBuffer->h, makecol(128,0,0));
		drawing_mode(DRAW_MODE_SOLID, NULL,0,0);

		masked_stretch_blit(gameoverbmp, doubleBuffer, 0, 0, gameoverbmp->w, gameoverbmp->h, gameoverbmp->w/3, 120, gameoverbmp->w*2, gameoverbmp->h*2);

		if (wait < 63)  // fade out with blood
			circlefill(doubleBuffer, doubleBuffer->w/2, doubleBuffer->h/2, (64-wait)*10, makecol(255,0,0));
		// TODO sound...
	break;

	case STATE_INTRONEXTLEVEL:
		stretch_blit(warlord, doubleBuffer, 0, 0, warlord->w, warlord->h, 0, 0, doubleBuffer->w, doubleBuffer->h);		
		masked_stretch_blit(nextbmp, doubleBuffer, 0, 0, nextbmp->w, nextbmp->h, nextbmp->w/3, nextbmp->h/2, nextbmp->w*6, nextbmp->h*6);
		// TODO sound

		textprintf_ex(doubleBuffer, font,45,nextbmp->h*8,makecol(255,0,0), -1, "Hunting number %d", level);

		textprintf_ex(doubleBuffer, font,0,doubleBuffer->h-text_height(font)*2,makecol(128,64 ,64 ), -1, "SPACE skip screen | F10 quit         | F11 video mode");
		textprintf_ex(doubleBuffer, font,0,doubleBuffer->h-text_height(font)*3,makecol(128,64 ,64 ), -1, "Left Mouse Shoot  | Right Mouse WOMD | ENTER airstrike");

		if (wait < 31)  // fade out
		{
			set_trans_blender(255-(wait*255/32),255-(wait*255/32),255-(wait*255/32),255-(wait*255/32));
			drawing_mode(DRAW_MODE_TRANS, NULL,0,0); 
			rectfill(doubleBuffer, 0, 0, doubleBuffer->w, doubleBuffer->h, makecol(0,0,0));
			drawing_mode(DRAW_MODE_SOLID, NULL,0,0);
			set_trans_blender(128,128,128,128);
		}
	break;


	case STATE_OK:
		if (map != NULL)
			map->render(doubleBuffer);

		babyL.render(doubleBuffer);
	
		particleM.render(doubleBuffer,0,0);

		weather.render(doubleBuffer);

		// aim
		circle(doubleBuffer, w_x, w_y, 4, makecol(255,0,0));
		circle(doubleBuffer, w_x, w_y, 8, makecol(255,0,0));
		hline(doubleBuffer, w_x-9, w_y, w_x+9, makecol(255,0,0));
		vline(doubleBuffer, w_x, w_y-9, w_y+9, makecol(255,0,0));

		// airstrike dialog
		if (airstrike_t > 0)
		{
			int yy = doubleBuffer->h-condor->h;
			int xx = condor->w + (int)(condor->w*0.1);
			
			char *txt = "Yarr! Sure captain?";
			if (airstrike_t > 2)
				txt = "Really sure yadda yadda WOMD?";
			if (airstrike_t > 3)
				txt = "Totally sure with this, my bro'?";
			if (airstrike_t > 4)
				txt = "Kids are going to die?";
			if (airstrike_t > 5)
				txt = "We better find oil! Sure? to do this?";
			if (airstrike_t > 6)
				txt = "The UN don't support us, sure mothefucka?";
			if (airstrike_t > 7)
				txt = "We are going to screw up! Really sure?";
			if (airstrike_t > 8)
				txt = "Is going to cost a fortune to tax payers, is OK?";
			if (airstrike_t > 9)
				txt = "Last warning, really OK? Judgment day is a sign ahead...";

			
			drawing_mode(DRAW_MODE_TRANS, NULL,0,0); 
			rectfill(doubleBuffer, 0, yy-5, doubleBuffer->w, doubleBuffer->h, makecol(64,32,0));
			drawing_mode(DRAW_MODE_SOLID, NULL,0,0);


			draw_sprite(doubleBuffer, condor, 0, yy);

			yy += (int)(condor->w * 0.2);
 			textout_ex(doubleBuffer, font, "Preemptive airstrike", xx, yy, makecol(255,128,0), -1);
			
			yy += text_height(font)*2;
 			textout_ex(doubleBuffer, font, txt, xx, yy, makecol(255,255,0), -1);

			yy += text_height(font)*3;
 			textout_ex(doubleBuffer, font, "(Y)es / (N)o", xx, yy, makecol(255,0,0), -1);

		}


		// draw score, life, etc...
		textprintf_ex(doubleBuffer, font,0,0,makecol(255,255,0), -1, "Premier Bush's Life %d", life);
		textprintf_ex(doubleBuffer, font,0,text_height(font),makecol(255,0,0), -1, "Blood Harvested %d", blood);
		textprintf_ex(doubleBuffer, font,0,text_height(font)*2,makecol(0,255,0), -1, "W.O.M.D Left %d", grenades);
		textprintf_ex(doubleBuffer, font,0,text_height(font)*3,makecol(0,255,0), -1, "Airstrikes Left %d", airstrike);
		textprintf_ex(doubleBuffer, font,0,text_height(font)*4,makecol(0,200,0), -1, "Money Used %lu,%d,%d,%d,%d,%d,%d,%d.%d U$S", money, rand()%800+100, rand()%800+100, rand()%800+100, rand()%800+100,rand()%800+100,rand()%800+100,rand()%800+100,rand()%80+10);
	break;

	default:
	break;
	}

	// fill screen -- lame way to do widescreen :P
	if (camerashake > 0) // do camera shake effect, for explosions
		stretch_blit(doubleBuffer, screen, 0, 0, doubleBuffer->w, doubleBuffer->h, rand()%8-4, rand()%8-4, SCREEN_W+rand()%5, SCREEN_H+rand()%5);
	else
		stretch_blit(doubleBuffer, screen, 0, 0, doubleBuffer->w, doubleBuffer->h, 0, 0, SCREEN_W, SCREEN_H);
}

