#include <algorithm>
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "../globals.hpp"
#include "../engine/mesh.hpp"
#include "../engine/physics/collision.hpp"
#include "scene.hpp"
#include "playerattack.hpp"
#include <iostream>

/*
Player
*/
Player::Player(unsigned int gridx, unsigned int gridy, PlayerAnimator animator) 
		: GameObject(gridx, gridy, *(animator.getCurrentFrame())) {
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
	prevAttack = false;
}

void Player::update(const std::vector<Mesh>& meshes, Scene& parentScene) {
	//Movement
	bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	bool jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	bool attack = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);

	//Horizontal movement
	if (!attacking) {
		if (moveLeft ^ moveRight) {
			if (moveRight) {
				hVelocity = std::clamp(hVelocity + hAcceleration, -2.f, 2.f);
			}
			else {
				hVelocity = std::clamp(hVelocity - hAcceleration, -2.f, 2.f);
			}
		}
		else {
			if (hVelocity > 0.f) {
				hVelocity = std::clamp(hVelocity - hAcceleration, 0.f, 2.f);
			}
			else
				if (hVelocity < 0.f) {
					hVelocity = std::clamp(hVelocity + hAcceleration, -2.f, 0.f);
				}
		}
	}
	else {
		if (onGround) {
			hVelocity = 0.f;
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
	}
	else {
		if (hVelocity > 0.f) {
			hVelocity = 0.f;

			//Lock position to the right
			float lockedx = std::ceilf(getPosition().x / 16) * 16;
			setPosition(lockedx, getPosition().y);
		}
		else if (hVelocity < 0.f) {
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
		}
		else {
			vVelocity = vAcceleration;
		}
	}
	else {
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
	}
	else {
		//If going upwards
		if (vVelocity < 0.f) {
			vVelocity = 0.f;

			//Lock position to ceiling
			float lockedy = std::floorf(getPosition().y / 16) * 16;
			setPosition(getPosition().x, lockedy);
		}
		else {
			//If going downwards
			vVelocity = 0.f;
			onGround = true;

			//Lock position to the ground
			float lockedy = std::ceilf(getPosition().y / 16) * 16;
			setPosition(getPosition().x, lockedy);
		}
	}

	prevJump = jump;

	//Attack
	if (!attacking) {
		if (attack && !prevAttack) {
			attacking = true;
			attackTimer = 0;

			PlayerAttack a = attacks["slash"];

			attackSpeed = a.getAnimator().getSpeed() * a.getAnimator().getFrameCount();

			bool rightFacing;
			if (animator.getState() == AnimationStates::right) {
				rightFacing = true;
			}
			else {
				rightFacing = false;
			}

			a.create(getPosition().x, getPosition().y, rightFacing);
			parentScene.addPlayerAttack(a);
		}
	} 
	else {
		attackTimer++;
		if (attackTimer >= attackSpeed) {
			attacking = false;
		}
	}

	prevAttack = attack;

	animator.update(hVelocity, sprite);
}

void Player::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}

void Player::addAttack(std::string name, PlayerAttack& attack) {
	attacks[name] = attack;
}