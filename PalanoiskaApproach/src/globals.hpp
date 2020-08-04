#ifndef GLOBALS_H
#define GLOBALS_H

/*
Used to define global constants and GameState enum
*/

#include "engine/settings.hpp"

namespace Globals {
	const unsigned int TileSize = 16;
	extern bool Debugging;
};

enum class GameState { MainMenu, InGame, Paused };

#endif