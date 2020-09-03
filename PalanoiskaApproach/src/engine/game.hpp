#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include "../game/scene.hpp"
#include "ui/uimanager.hpp"
#include "settings.hpp"
#include "../globals.hpp"

class Game {
public:
	Game() { event = sf::Event(); gameState = GameState::MainMenu; }
	~Game() { window.~RenderWindow(); }
	int init();
	void update();
	void draw();
	void changeScene(std::string name);
private:
	sf::RenderWindow window;
	sf::Event event;
	std::unordered_map<std::string, std::string> dialogueText;
	std::vector<sf::Texture> textures;
	std::vector<sf::Texture> uiTextures;
	std::vector<sf::Texture> backgroundTextures;
	sf::Font font;
	sf::View menuView;
	Scene currentScene;
	GameState gameState;
	UIManager uiManager;
	Player player;
};

#endif