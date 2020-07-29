#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../game/scene.hpp"
#include "settings.hpp"

enum class GameState { MainMenu, InGame };

class Game {
public:
	Game() { event = sf::Event(); gameState = GameState::MainMenu; }
	~Game() { window.~RenderWindow(); }
	int init();
	void update();
	void draw();
private:
	sf::RenderWindow window;
	sf::Event event;
	std::vector<sf::Texture> textures;
	std::vector<sf::Texture> uiTextures;
	sf::View view;
	Scene currentScene;
	GameSettings::WindowSettings windowSettings;
	GameState gameState;
};

#endif