#include "uimanager.hpp"

UIManager::UIManager(GameState* gameState) {
	this->gameState = gameState;
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

void UIManager::draw(sf::RenderWindow& window) {
	switch (*gameState) {
	case GameState::MainMenu:
		for (UIElement& e : mainMenu) {
			e.draw(window);
		}
		break;
	case GameState::InGame:
		for (UIElement& e : inGame) {
			e.draw(window);
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

void UIManager::addPaused(UIElement&& element) {
	paused.emplace_back(element);
}