#include "enemy.hpp"
#include "../globals.hpp"
#include "playerattack.hpp"
#include "../engine/physics/collision.hpp"
#include "../engine/mesh.hpp"
#include <iostream>
#include "../engine/settings.hpp"

Enemy::Enemy(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {
	moveRight = true;
	speed = 0.5f;
	currentHp = 10;
	damage = 5;
}

#pragma warning(disable : 26812 ) //Disable warning about sfml

void Enemy::update(const std::vector<Mesh>& meshes, std::vector<PlayerAttack>& attacks, std::vector<FloatingText>& floatText, bool takeDamage) {
	if (takeDamage) {
		for (PlayerAttack& a : attacks) {
			if (isColliding(boundingBox, a.getBoundingBox())) {
				int damage = a.getDamage();
				currentHp -= damage;
				floatText.emplace_back(getPosition().x, getPosition().y - 8, std::to_string(damage), *settings.font);
			}
		}
	}

	bool canMove = true;
	bool falling = true;
	sf::RectangleShape nextPos = boundingBox;
	sf::RectangleShape fallPos = boundingBox;
	if (moveRight) {
		nextPos.move(speed, 0.f);
		fallPos.move(boundingBox.getSize().x, 0.2f);
	} else {
		nextPos.move(-speed, 0.f);
		fallPos.move(-boundingBox.getSize().x, 0.2f);
	}

	for (const Mesh& m : meshes) {
		if (isColliding(nextPos, m.getBoundingBox())) {
			canMove = false;
		}
		if (isColliding(fallPos, m.getBoundingBox())) {
			falling = false;
		}
	}

	if (canMove && !falling) {
		setPosition(nextPos.getPosition());
	} else {
		if (moveRight) {
			//Lock position to the left
			float lockedx = std::floorf(getPosition().x / 16) * 16;
			setPosition(lockedx, getPosition().y);
		} else
		if (!moveRight) {
			//Lock position to the right
			float lockedx = std::ceilf(getPosition().x / 16) * 16;
			setPosition(lockedx, getPosition().y);
		}

		moveRight = !moveRight;
	}
}

#pragma warning(default : 26812 )

void Enemy::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}