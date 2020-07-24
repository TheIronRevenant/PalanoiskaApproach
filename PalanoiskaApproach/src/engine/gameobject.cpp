#include <algorithm>
#include "../globals.hpp"
#include "physics/collision.hpp"
#include "mesh.hpp"
#include "gameobject.hpp"
#include <iostream>

/*
Game Object Abstract Class
*/
GameObject::GameObject(unsigned int gridx, unsigned int gridy, sf::Texture& texture) {
	boundingBox = sf::RectangleShape(sf::Vector2f(Globals::TileSize, Globals::TileSize));
	boundingBox.setPosition((float)gridx * Globals::TileSize, (float)gridy * Globals::TileSize);
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Red);
	boundingBox.setFillColor(sf::Color::Transparent);

	sprite = sf::Sprite();
	sprite.setPosition(getPosition());
	sprite.setTexture(texture);
}

void GameObject::draw(sf::RenderWindow& window) const {
	window.draw(sprite);
}

void GameObject::draw(sf::RenderTexture& texture) const {
	texture.draw(sprite);
}

void GameObject::move(sf::Vector2f vector) {
	sf::Vector2f pos = vector + getPosition();
	boundingBox.setPosition(pos);
	sprite.setPosition(pos);
}

void GameObject::move(float x, float y) {
	sf::Vector2f pos = getPosition();
	pos.x += x;
	pos.y += y;
	boundingBox.setPosition(pos);
	sprite.setPosition(pos);
}

void GameObject::setPosition(sf::Vector2f vector) {
	boundingBox.setPosition(vector);
	sprite.setPosition(getPosition());
}

void GameObject::setPosition(float x, float y) {
	boundingBox.setPosition(x, y);
	sprite.setPosition(getPosition());
}

/*
Static Object Class
*/
StaticObject::StaticObject(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {}

/*
Player Object Class
*/
Player::Player(unsigned int gridx, unsigned int gridy, PlayerAnimator animator) : GameObject(gridx, gridy, *(animator.getCurrentFrame())) {
	this->animator = animator;

	hVelocity = 0.f;
	hAcceleration = 0.2f;
	hMax = 2.f;

	vVelocity = 0.f;
	vJumpAcceleration = -6.f;
	vTerminalVelocity = 6.f;
	vAcceleration = 0.2f;
	onGround = false;
	prevJump = false;
}

void Player::update(const std::vector<Mesh>& meshes) {
	//Movement
	bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	bool jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	//Horizontal movement
	if (moveLeft ^ moveRight) {
		if (moveRight) {
			hVelocity = std::clamp(hVelocity + hAcceleration, -2.f, 2.f);
		} else {
			hVelocity = std::clamp(hVelocity - hAcceleration, -2.f, 2.f);
		}
	} else {
		if (hVelocity > 0.f) {
			hVelocity = std::clamp(hVelocity - hAcceleration, 0.f, 2.f);
		} else
		if (hVelocity < 0.f) {
			hVelocity = std::clamp(hVelocity + hAcceleration, -2.f, 0.f);
		}
	}

	//Next horizontal position
	sf::RectangleShape nextPos = boundingBox;
	nextPos.move(hVelocity, 0);
	bool canMove = true;

	//Check collisions
	for (std::vector<Mesh>::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
		if (isColliding(nextPos, it->getBoundingBox())) {
			canMove = false;
		}
	}

	if (canMove) {
		setPosition(nextPos.getPosition());
	} else {
		if (hVelocity > 0.f) {
			hVelocity = 0.f;

			//Lock position to the right
			float lockedx = std::ceilf(getPosition().x / 16) * 16;
			setPosition(lockedx, getPosition().y);
		} else
		if (hVelocity < 0.f) {
			hVelocity = 0.f;

			//Lock position to the left
			float lockedx = std::floorf(getPosition().x / 16) * 16;
			setPosition(lockedx, getPosition().y);
		}
	}

	//Vertical movement
	if (onGround) {
		//If the player jumps
		if (jump && !prevJump) {
			vVelocity = vJumpAcceleration;
		} else {
			vVelocity = vAcceleration;
		}
	} else {
		vVelocity = std::clamp(vVelocity + vAcceleration, vVelocity - vTerminalVelocity, vTerminalVelocity);
	}

	//Next vertical position
	sf::RectangleShape fallPos = boundingBox;
	fallPos.move(0, vVelocity);
	bool canFall = true;

	for (std::vector<Mesh>::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
		if (isColliding(fallPos, it->getBoundingBox())) {
			canFall = false;
		}
	}

	if (canFall) {
		setPosition(fallPos.getPosition());
		//Object can fall so is no longer on ground
		onGround = false;
	} else {
		//If going upwards
		if (vVelocity < 0.f) {
			vVelocity = 0.f;

			//Lock position to ceiling
			float lockedy = std::floorf(getPosition().y / 16) * 16;
			setPosition(getPosition().x, lockedy);
		} else {
			//If going downwards
			vVelocity = 0.f;
			onGround = true;

			//Lock position to the ground
			float lockedy = std::ceilf(getPosition().y / 16) * 16;
			setPosition(getPosition().x, lockedy);
		}
	}

	prevJump = jump;
}

void Player::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}