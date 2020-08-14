#include <algorithm>
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "../globals.hpp"
#include "../engine/mesh.hpp"
#include "../engine/physics/collision.hpp"
#include "scene.hpp"
#include "playerattack.hpp"
#include <iostream>

Player::Player(unsigned int gridx, unsigned int gridy, PlayerAnimator&& animator) 
		: GameObject(gridx, gridy, *(animator.getCurrentFrame())) {
	controlled = true;

	maxHp = 100;
	currentHp = 100;
	invincible = false;
	iframes = 20;
	iframeTimer = 0;

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
	prevInteract = false;
}

void Player::update(const std::vector<Mesh>& meshes, const std::vector<Enemy>& enemies, std::vector<FloatingText>& floatText, std::vector<Interactable>& interactables, Scene& parentScene) {
	//Movement
	bool moveLeft = false;
	bool moveRight = false;
	bool jump = false;
	bool attack = false;
	if (controlled) {
		moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		attack = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	}
	bool interact = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

	for (Interactable& i : interactables) {
		if (isColliding(boundingBox, i.getBoundingBox())) {
			if (interact && !prevInteract) {
				i.interact();
				if (i.dialogueInfo.hasDialogue) {
					if (i.dialogueInfo.visible && i.dialogueInfo.pauseGame) {
						controlled = false;
						hVelocity = 0.f;
					}
					if (!i.dialogueInfo.visible && i.dialogueInfo.pauseGame) {
						controlled = true;
					}
				}
			}
		}
	}

	prevInteract = interact;

	//Horizontal movement
	if (!attacking) {
		if (!invincible) {
			if ((moveLeft ^ moveRight)) {
				if (moveRight) {
					hVelocity = std::clamp(hVelocity + hAcceleration, -hMax, hMax);
				} else {
					hVelocity = std::clamp(hVelocity - hAcceleration, -hMax, hMax);
				}
			} else {
				if (hVelocity > 0.f) {
					hVelocity = std::clamp(hVelocity - hAcceleration, 0.f, hMax);
				} else
				if (hVelocity < 0.f) {
					hVelocity = std::clamp(hVelocity + hAcceleration, -hMax, 0.f);
				}
			}
		}
	} else {
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
			//Pushes the player in the air when hit
			if (!invincible) {
				vVelocity = vAcceleration;
			}
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

	//Enemy collision
	if (!invincible && controlled) {
		for (const Enemy& e : enemies) {
			if (isColliding(boundingBox, e.getBoundingBox())) {
				floatText.emplace_back(getPosition().x, getPosition().y - 8, std::to_string(e.getDamage()), *settings.font);
				currentHp -= e.getDamage();
				invincible = true;
				iframeTimer = 0;

				//Knockback
				if (animator.getState() == AnimationStates::right) {
					hVelocity = -hMax;
				} else {
					hVelocity = hMax;
				}
				vVelocity = -2.f;
			}
		}
	} else 
	if (invincible) {
		iframeTimer++;
		if (iframeTimer >= iframes) {
			invincible = false;
			hVelocity = 0.f;
		}
	}

	if (!invincible) {
		animator.update(hVelocity, sprite);
	}
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

void Player::reset() {
	currentHp = 100;
	invincible = false;
	iframes = 20;
	iframeTimer = 0;

	hVelocity = 0.f;

	vVelocity = 0.f;
	onGround = false;

	prevJump = false;
	prevAttack = false;
	prevInteract = false;
}