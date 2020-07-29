#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "uielement.hpp"
#include "../../globals.hpp"

class UIManager {
public:
	UIManager() { gameState = nullptr; }
	UIManager(GameState* gameState);
	void recieveClick(float x, float y);
	void draw(sf::RenderWindow& window);
	void addMainMenu(UIElement&& element);
	void addInGame(UIElement&& element);
private:
	std::vector<UIElement> mainMenu;
	std::vector<UIElement> inGame;
	GameState* gameState;
};

#endif