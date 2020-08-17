#ifndef GLOBALS_H
#define GLOBALS_H

/*
Used to define global constants and GameState enum
*/

namespace Globals {
	const unsigned int TileSize = 16;
	const unsigned int PlayerOffset = 0;
	const unsigned int TileOffset = 2;
	const unsigned int EnemyOffset = 19;
	const unsigned int EffectOffset = 23;
	extern bool Debugging;
};

enum class GameState { MainMenu, InGame, Paused };

#endif