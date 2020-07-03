// -----------------------------------------------------------------------
// gamemachine.cpp
// -----------------------------------------------------------------------
// Game Machine
// -----------------------------------------------------------------------
// Copyright (c) 2007, Kronoman
// In loving memory of my father.
// Made in Argentina
// -----------------------------------------------------------------------
// Released under The MIT License.
// -----------------------------------------------------------------------
#include <allegro.h>
#include "gamemachine.h"
#include "error.h"

// -- timers
// timer system to keep frame rate FPS constant
volatile int speed_counter = 0;

void increment_speed_counter()
{
	speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);
// --- end of timers...


unsigned int GameMachine::machineCount=0;

GameMachine::GameMachine()
{
	previousState = currentState = 0;
	machineCount++;

	// set up timer
	if (machineCount == 1)
	{
		LOCK_VARIABLE(speed_counter);
		LOCK_FUNCTION(increment_speed_counter);

		if (install_int_ex(increment_speed_counter, BPS_TO_TIMER(GAMEMACHINE_BPS)) != 0)
			Error::error("Error at gamemachine.cpp\nFailed to install timer!");

		speed_counter = 0;
	}
}


GameMachine::~GameMachine()
{
	machineCount--;
	// remove timer
	if (machineCount == 0)
		remove_int(increment_speed_counter); // timer
}

void GameMachine::start()
{
	currentState = STATE_START;
	previousState = STATE_START;

	doubleBuffer = create_bitmap(SCREEN_W, SCREEN_H);
	if (doubleBuffer == NULL)
		Error::error("GameMachine::start()\nERROR: can't get a bitmap %d x %d px", SCREEN_W, SCREEN_H);

	// -- main loop
	while (currentState != STATE_QUIT)
	{
		while (speed_counter > 0)
		{
			update();
			speed_counter--;
		}

		// do the rendering and showing
		render();
	}
	// -- main loop end

	destroy_bitmap(doubleBuffer); // bitmap
	doubleBuffer = NULL;
}

void GameMachine::changeState(const unsigned int state)
{
	previousState = currentState;
	currentState = state;
}



// -----------------------------------------------------------------------
// NOTE update and render are just examples!!
// so you SHOULD OVERRIDE THEM, THAT IS THE "MEAT" OF YOUR PROJECT
// -----------------------------------------------------------------------

void GameMachine::update()
{
	// this would handle the states...
	switch (currentState)
	{
	case STATE_START:
		changeState(STATE_OK);
		break;

	case STATE_OK:
		// TODO do something here, according to your game
		break;

	default:
		// probably a error if we go here... a unhandled state... mmm
		break;
	}

	// panic key to exit...
	if (key[KEY_ESC])
		changeState(STATE_QUIT);
}

void GameMachine::render()
{
	clear(doubleBuffer); // this is optional

	// TODO RENDER CODE HERE

	blit(doubleBuffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H); // to show on screen
}

