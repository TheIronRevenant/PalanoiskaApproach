#include <SFML/Graphics.hpp>
#include "../globals.hpp"
#include "playerattack.hpp"

PlayerAttack::PlayerAttack(AttackAnimator&& animator, int damage, AttackType type) {
	this->animator = animator;
	terminated = false;
	this->damage = damage;
	this->type = type;
	rightFacing = false;
}

void PlayerAttack::create(float x, float y, bool rightFacing) {
	this->rightFacing = rightFacing;
	boundingBox = sf::RectangleShape(sf::Vector2f(animator.getCurrentFrame()->getSize()));

	float posx = x;
	if (type == AttackType::Slash) {
		if (!rightFacing) {
			posx = x - Globals::TileSize;
		}
	}
	
	if (type == AttackType::Slash) {
		boundingBox.setPosition(posx, y);
	} else
	if (type == AttackType::Overhead) {
		boundingBox.setPosition(posx, y - (Globals::TileSize * 2));
	}
	boundingBox.setOutlineThickness(1.f);
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

void PlayerAttack::update() {
	if (!terminated) {
		if (animator.update(sprite, terminated)) {
			//Attack updated
			if (type == AttackType::Overhead) {
				//Overhead logic
				boundingBox.setSize(sf::Vector2f(animator.getCurrentFrame()->getSize()));
				sf::Vector2f size = boundingBox.getSize();
				if (rightFacing) {
					boundingBox.move((float)Globals::TileSize, 0);
					sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
				} else {
					boundingBox.move(-((float)Globals::TileSize * 2), 0);
					sprite.setTextureRect(sf::IntRect((int)size.x, 0, (int)-size.x, (int)size.y));
				}
				sprite.setPosition(getPosition());
			}
		}
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