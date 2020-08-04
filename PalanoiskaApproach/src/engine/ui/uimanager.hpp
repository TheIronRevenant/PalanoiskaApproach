#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "uielement.hpp"
#include "../../globals.hpp"
#include "../settings.hpp"
#include "../../game/player.hpp"

class UIManager {
public:
	UIManager() { gameState = nullptr; uiView.reset(sf::FloatRect(-1000.f, 0.f, 100.f, 100.f)); }
	UIManager(GameState* gameState);
	void recieveClick(float x, float y);
	void update(const Player& player);
	void draw(sf::RenderWindow& window);
	void addMainMenu(UIElement&& element);
	void addInGame(UIElement&& element);
	void addPaused(UIElement&& element);
private:
	std::vector<UIElement> mainMenu;
	std::vector<UIElement> inGame;
	std::vector<UIElement> paused;
	GameState* gameState;
	sf::View uiView;
};

#endif