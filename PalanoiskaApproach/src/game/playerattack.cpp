#include <SFML/Graphics.hpp>
#include "../globals.hpp"
#include "playerattack.hpp"

PlayerAttack::PlayerAttack(AttackAnimator animator) {
	this->animator = animator;
}

void PlayerAttack::create(float x, float y, bool rightFacing) {
	boundingBox = sf::RectangleShape(sf::Vector2f(animator.getCurrentFrame()->getSize()));

	float posx;
	if (rightFacing) {
		posx = x;
	} else {
		posx = x - Globals::TileSize;
	}

	boundingBox.setPosition(posx, y);
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Green);
	boundingBox.setFillColor(sf::Color::Transparent);

	sprite = sf::Sprite();
	sprite.setPosition(getPosition());
	sprite.setTexture(*animator.getCurrentFrame());
}

void PlayerAttack::update() {
	animator.update(sprite);
}

void PlayerAttack::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}