#include "uimanager.hpp"
#include "../settings.hpp"
#include <string>

UIManager::UIManager(GameState* gameState) {
	this->gameState = gameState;
	uiView.reset(sf::FloatRect(-1000.f, 0.f, (float)windowSettings.resw * 36.f, (float)windowSettings.resh * 36.f));
}

void UIManager::recieveClick(float x, float y) {
	switch (*gameState) {
	case GameState::MainMenu:
		for (UIElement& e : mainMenu) {
			if (e.getRect().contains(x, y)) {
				e.onClick();
			}
		}
		break;
	case GameState::InGame:
		for (UIElement& e : inGame) {
			if (e.getRect().contains(x, y)) {
				e.onClick();
			}
		}
		break;
	case GameState::Paused:
		for (UIElement& e : paused) {
			if (e.getRect().contains(x, y)) {
				e.onClick();
			}
		}
		break;
	}
}

void UIManager::update(const Player& player) {
	//Hp bar is 2nd element
	int hp = player.getCurrentHp();
	int max = player.getMaxHp();
	float percentHp = (float)hp / (float)max;
	float width = inGame[1].getRect().width * percentHp;
	inGame[1].setWidth(width);
	gameText[0].setString(std::to_string(hp) + " / " + std::to_string(max));
}

void UIManager::draw(sf::RenderWindow& window) {
	switch (*gameState) {
	case GameState::MainMenu:
		for (UIElement& e : mainMenu) {
			e.draw(window);
		}
		break;
	case GameState::InGame:
		window.setView(uiView);
		for (UIElement& e : inGame) {
			e.draw(window);
		}

		for (sf::Text& t : gameText) {
			window.draw(t);
		}
		break;
	case GameState::Paused:
		for (UIElement& e : paused) {
			e.draw(window);
		}
		break;
	}
}

void UIManager::addMainMenu(UIElement&& element) {
	mainMenu.emplace_back(element);
}

void UIManager::addInGame(UIElement&& element) {
	inGame.emplace_back(element);
}

void UIManager::addInGame(sf::Text& text) {
	gameText.emplace_back(text);
}

void UIManager::addPaused(UIElement&& element) {
	paused.emplace_back(element);
}