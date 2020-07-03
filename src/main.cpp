// ---------------------------------------------------------------------------
// main.cpp
// -----------------------------------------------------------------------
// Main
//
// Entry point
// -----------------------------------------------------------------------
// Copyright (c) 2007, Kronoman
// In loving memory of my father.
// Made in Argentina
// -----------------------------------------------------------------------
// Released under The MIT License.
// -----------------------------------------------------------------------
#include "hardwaremanager.h"

#include "maingloop.h"

// -----------------------------------------------------------------------
// game entry point from operating system...
// -----------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// init hardware
	HardwareManager::init(argc, argv);

	// ---------------------------
	// here starts the game...
	// ---------------------------
	MainGameLoop mgl;
	mgl.start();

	HardwareManager::shutdown();

	return 0;
}
END_OF_MAIN();

