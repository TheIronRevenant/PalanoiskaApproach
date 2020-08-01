#include <SFML/Graphics.hpp>
#include "../globals.hpp"
#include "playerattack.hpp"

PlayerAttack::PlayerAttack(AttackAnimator&& animator) {
	this->animator = animator;
	terminated = false;
}

#pragma warning(disable : 26812 ) //Disable warning about sfml

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
	if (!rightFacing) {
		sf::Vector2f size = boundingBox.getSize();
		sprite.setTextureRect(sf::IntRect((int)size.x, 0, (int)-size.x, (int)size.y));
	}
}

#pragma warning(default : 26812 )

void PlayerAttack::update() {
	if (!terminated) {
		animator.update(sprite, terminated);
	}
}

void PlayerAttack::draw(sf::RenderWindow& window) {
	if (!terminated) {
		window.draw(sprite);

		if (Globals::Debugging) {
			window.draw(boundingBox);
		}
	}
}