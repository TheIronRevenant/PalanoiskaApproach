#include <SFML/Graphics.hpp>
#include "engine/game.hpp"

bool Globals::Debugging = false;

int main() {
	Game game;
	int exitCode = game.init();

	return exitCode;
}