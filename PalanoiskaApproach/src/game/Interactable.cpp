#include "interactable.hpp"
#include "../engine/game.hpp"
#include "../globals.hpp"
#include <iostream>

Interactable::Interactable(unsigned int gridx, unsigned int gridy, sf::Texture& texture, DialogueInfo diagInfo) : GameObject(gridx, gridy, texture) {
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Green);
	boundingBox.setFillColor(sf::Color::Transparent);
	dialogueInfo = diagInfo;
}

void Interactable::update() {
	if (dialogueInfo.hasDialogue && dialogueInfo.visible && !dialogueInfo.pauseGame) {
		dialogueInfo.showCount++;
		if (dialogueInfo.showCount >= dialogueInfo.showTime) {
			dialogueInfo.visible = false;
		}
	}
}

void Interactable::interact() {
	if (!dialogueInfo.visible) {
		interactfunc(dialogueInfo);
	}
	else {
		if (dialogueInfo.pauseGame) {
			dialogueInfo.diagIndex++;
			if (dialogueInfo.diagIndex >= dialogueInfo.diagCount) {
				dialogueInfo.diagIndex = 0;
				dialogueInfo.visible = false;
			}
		}
	}
}

void Interactable::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (dialogueInfo.hasDialogue && dialogueInfo.visible) {
		dialogueInfo.dialogue[dialogueInfo.diagIndex].draw(window);
	}

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}