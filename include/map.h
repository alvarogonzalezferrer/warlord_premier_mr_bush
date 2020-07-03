// -----------------------------------------------------------------------
// map.h
// -----------------------------------------------------------------------
// Map of the game
// -----------------------------------------------------------------------
// Copyright (c) 2007, Kronoman
// In loving memory of my father.
// Made in Argentina
// -----------------------------------------------------------------------
// Released under The MIT License.
// -----------------------------------------------------------------------
#ifndef MAP_H
#define MAP_H



#include <allegro.h>

// map itself
class Map
{
	public:
		Map();
		~Map();
	
		void generate(); // generate a random map, and cache the representation
	
		void render(BITMAP *bmp); // render on bitmap
	
		int getHeight(int x); // get height at position x
	
		void cache(); // cache bitmap with map

		static const unsigned int mapW = 640;
		static const unsigned int mapH = 480;
	
		int map[mapW]; // height of the map

		unsigned int tm; // type of map, grass, snow, rocks

		unsigned int tw; // type of weather, 0 = snow, 1 = rain, 2 = none


		static const unsigned int SNOW = 0; // snow
		static const unsigned int GRASS = 1; // grass
		static const unsigned int ROCK = 2; // rocks
	
		BITMAP *mapBMP; // cached representation of the map.
};

#endif
