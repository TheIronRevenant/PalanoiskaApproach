#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../game/scene.hpp"

class Game {
public:
	Game() {}
	~Game() { window.~RenderWindow(); }
	int init();
	void update();
	void draw();
private:
	sf::RenderWindow window;
	sf::Event event;
	std::vector<sf::Texture> textures;
	sf::View view;
	Scene currentScene;
};

#endif