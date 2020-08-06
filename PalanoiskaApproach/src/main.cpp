#include <SFML/Graphics.hpp>
#include "engine/game.hpp"
#include "globals.hpp"

//Set debugging value
bool Globals::Debugging = false;

int main() {
	Game game;
	int exitCode = game.init();

	return exitCode;
}