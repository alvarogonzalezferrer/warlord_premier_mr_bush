// -----------------------------------------------------------------------
// map.cpp
// -----------------------------------------------------------------------
// Map of the game
// -----------------------------------------------------------------------
// Copyright (c) 2007, Kronoman
// In loving memory of my father.
// Made in Argentina
// -----------------------------------------------------------------------
// Released under The MIT License.
// -----------------------------------------------------------------------

#include "error.h"
#include "map.h"
#include "logger.h"

Map::Map()
{
	Logger::log("Map::Map()");
	mapBMP = NULL;
	tw = 0;
}

Map::~Map()
{
	Logger::log("Map::~Map()");
	destroy_bitmap(mapBMP);
}

void Map::generate()
{
	Logger::log("Map::generate()");

	float y = mapH - mapH / 3;
	int r = 0; // length
	float s = 0; // slope
	tm = rand()%3; // map type


	// do terrain
	for (unsigned int x = 0; x < mapW; x++)
	{
		if (--r < 1) // next segment
		{
			r = rand() % mapW/10 + 4;
			s = (float)(rand() % 120 - 60) / 100.0f; 
		}

		y += s;
		if (y < mapH *0.4 || y > mapH*0.9) 
			r = 0; // bounce

		map[x] = (int)y;

	}

	cache();
}



void Map::cache()
{
	if (mapBMP == NULL)
	{
		mapBMP = create_bitmap(mapW, mapH);
		if (mapBMP == NULL)
			Error::error("Map::cache()\nERROR: can't get a bitmap %d x %d px", mapW, mapH);
	}

	// select weather
	tw = 2; // no weather default
	if (tm == SNOW)
	{
		if (rand()%100 < 90)
			tw = 0; // snow	
	}
	else
	{
		if (rand()%100 < 45)
			tw = 1; // rain
	}

	// gradient sky
	float c1[3];
	float c2[3];
	float s[3];

	c1[0]=0;
	c1[1]=148;
	c1[2]=rand()%64+190;

	c2[0]=0;
	c2[1]=0;
	c2[2]=128;

	if (tw == 0) // snow
	{
		c1[0]=48;
		c1[1]=48;
		c1[2]=96;
	
		c2[0]=16;
		c2[1]=16;
		c2[2]=16;
	}

	if (tw == 1) // rain
	{
		c1[0]=128;
		c1[1]=96;
		c1[2]=96;
	
		c2[0]=0;
		c2[1]=0;
		c2[2]=128;
	}

	s[0] = (c2[0] - c1[0]) / mapBMP->h;
	s[1] = (c2[1] - c1[1]) / mapBMP->h;
	s[2] = (c2[2] - c1[2]) / mapBMP->h;
	for (int y = 0 ;y < mapBMP->h;y++)
	{		
		hline(mapBMP, 0, y, mapBMP->w, makecol((int)c1[0],(int)c1[1],(int)c1[2]));
		c1[0] += s[0];
		c1[1] += s[1];
		c1[2] += s[2];
	}
	// end sky

	
	// floor
	for (unsigned int x = 0; x < mapW; x++)
	{
		// draw terrain - TODO improve
		for (int i = mapH; i > map[x]; i--)
		{
			int c;
			if ( i > map[x]*1.02 + rand()%15)
			{
				switch (tm)
				{
					case SNOW:
						putpixel(mapBMP, x, i, makecol(0,0,rand()%48+64));
					break;

					case ROCK:
						c = rand()%32+16;
						putpixel(mapBMP, x, i, makecol(c,c,c));
					break;
					case GRASS:
					default:
						c = rand()%48+32;
						putpixel(mapBMP, x, i, makecol(128,c,0));
					break;
				}
			}
			else
			{
				switch (tm)
				{
					case SNOW:
						c = rand()%48+200;
						putpixel(mapBMP, x, i, makecol(c,c,c));
					break;

					case ROCK:
						c = rand()%64+32;
						putpixel(mapBMP, x, i, makecol(c,c,c));
					break;

					case GRASS:
					default:
						putpixel(mapBMP, x, i, makecol(0,rand()%64+96,0));
					break;
				}
			}
		}
	}
	
}

void Map::render(BITMAP *bmp)
{
	blit(mapBMP, bmp, 0, 0, 0, 0, mapBMP->w, mapBMP->h);
}

int Map::getHeight(int x)
{
	if (x < 0)
		x = 0;

	if (x > mapW-1)
		x = mapW-1;

	return map[x];
}


