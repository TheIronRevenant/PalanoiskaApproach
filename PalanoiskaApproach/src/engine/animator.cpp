#include "animator.hpp"
#include <iostream>

/*
Player Animator
*/

PlayerAnimator::PlayerAnimator(Animation walk) {
	walkAnimation = walk;
	currentAnimation = AnimationStates::right;
}

void PlayerAnimator::update(const float hVelocity, sf::Sprite& sprite) {
	//Checks current direction and changes direction
	if (hVelocity > 0.f) {
		if (currentAnimation != AnimationStates::right) {
			swapAnimation(AnimationStates::right, sprite);
		}
	} else
	if (hVelocity < 0.f) {
		if (currentAnimation != AnimationStates::left) {
			swapAnimation(AnimationStates::left, sprite);
		}
	}

	//Dont animate if the player is still
	if (hVelocity != 0.f) {
		//Add to and reset the timer
		walkAnimation.timer++;
		if (walkAnimation.timer >= walkAnimation.speed) {
			walkAnimation.timer = 0;
			//Update frame
			walkAnimation.frame++;
			if (walkAnimation.frame >= walkAnimation.frames.size()) {
				walkAnimation.frame = 0;
			}
			//Set texture
			sprite.setTexture(*(walkAnimation.frames[walkAnimation.frame]));
		}
	} else {
		//Set to resting frame
		walkAnimation.timer = 0;
		walkAnimation.frame = 0;
		sprite.setTexture(*(walkAnimation.frames[walkAnimation.frame]));
	}
}

//Returns current animation frame, used for constructing GameObject base class
sf::Texture* PlayerAnimator::getCurrentFrame() {
	return walkAnimation.frames[walkAnimation.frame];
}

void PlayerAnimator::swapAnimation(AnimationStates state, sf::Sprite& sprite) {
	currentAnimation = state;

	walkAnimation.timer = 0;
	walkAnimation.frame = 0;

	if (currentAnimation == AnimationStates::left) {
		sf::Vector2u size = sprite.getTexture()->getSize();
		sprite.setTextureRect(sf::IntRect((int)size.x, 0, -(int)size.x, (int)size.y));
	} else {
		sf::Vector2u size = sprite.getTexture()->getSize();
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}
}

/*
Attack Animator
*/

AttackAnimator::AttackAnimator(Animation animation) {
	this->animation = animation;
}

bool AttackAnimator::update(sf::Sprite& sprite, bool& terminated) {
	bool updated = false;

	animation.timer++;
	if (animation.timer >= animation.speed) {
		animation.timer = 0;
		animation.frame++;
		updated = true;
		if (animation.frame >= animation.frames.size()) {
			terminated = true;
			updated = false;
		} else {
			sprite.setTexture(*(getCurrentFrame()));
		}
	}

	return updated;
}

sf::Texture* AttackAnimator::getCurrentFrame() {
	return animation.frames[animation.frame];
}

/*
Enemy Animator
*/

EnemyAnimator::EnemyAnimator(Animation animation) {
	this->animation = animation;
	currentAnimation = AnimationStates::right;
}

void EnemyAnimator::update(bool moveRight, sf::Sprite& sprite) {
	if ((moveRight && currentAnimation == AnimationStates::right) || (!moveRight && currentAnimation == AnimationStates::left)) {
		animation.timer++;
		if (animation.timer >= animation.speed) {
			animation.timer = 0;
			animation.frame++;
			if (animation.frame >= animation.frames.size()) {
				animation.frame = 0;
			}
			sprite.setTexture(*(getCurrentFrame()));
		}
	} else {
		if (moveRight) {
			swapAnimation(AnimationStates::right, sprite);
		} else {
			swapAnimation(AnimationStates::left, sprite);
		}
	}
}

sf::Texture* EnemyAnimator::getCurrentFrame() {
	return animation.frames[animation.frame];
}

void EnemyAnimator::swapAnimation(AnimationStates state, sf::Sprite& sprite) {
	currentAnimation = state;

	animation.timer = 0;
	animation.frame = 0;

	if (currentAnimation == AnimationStates::left) {
		sf::Vector2u size = sprite.getTexture()->getSize();
		sprite.setTextureRect(sf::IntRect((int)size.x, 0, -(int)size.x, (int)size.y));
	} else {
		sf::Vector2u size = sprite.getTexture()->getSize();
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}
}