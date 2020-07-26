#include "animator.hpp"

/*
Player Animator
*/

PlayerAnimator::PlayerAnimator(Animation left, Animation right) {
	leftAnimation = left;
	rightAnimation = right;
	swapAnimation(AnimationStates::right); //Defaults to the right
}

void PlayerAnimator::update(const float hVelocity, sf::Sprite& sprite) {
	//Checks current direction and changes direction
	if (hVelocity > 0.f) {
		if (currentAnimation != AnimationStates::right) {
			swapAnimation(AnimationStates::right);
		}
	} else
	if (hVelocity < 0.f) {
		if (currentAnimation != AnimationStates::left) {
			swapAnimation(AnimationStates::left);
		}
	}

	//Dont animate if the player is still
	if (hVelocity != 0.f) {
		if (currentAnimation == AnimationStates::left) {
			//Add to and reset the timer
			leftAnimation.timer++;
			if (leftAnimation.timer >= leftAnimation.speed) {
				leftAnimation.timer = 0;
				//Update frame
				leftAnimation.frame++;
				if (leftAnimation.frame >= leftAnimation.frames.size()) {
					leftAnimation.frame = 0;
				}
				//Set texture
				sprite.setTexture(*(leftAnimation.frames[leftAnimation.frame]));
			}
		}
		else {
			//Same as above but for rightAnimation
			rightAnimation.timer++;
			if (rightAnimation.timer >= rightAnimation.speed) {
				rightAnimation.timer = 0;
				rightAnimation.frame++;
				if (rightAnimation.frame >= rightAnimation.frames.size()) {
					rightAnimation.frame = 0;
				}
				sprite.setTexture(*(rightAnimation.frames[rightAnimation.frame]));
			}
		}
	}
}

//Returns current animation frame, used for constructing GameObject base class
sf::Texture* PlayerAnimator::getCurrentFrame() {
	if (currentAnimation == AnimationStates::left) {
		return leftAnimation.frames[leftAnimation.frame];
	} else {
		return rightAnimation.frames[rightAnimation.frame];
	}
}

void PlayerAnimator::swapAnimation(AnimationStates state) {
	currentAnimation = state;

	if (state == AnimationStates::left) {
		leftAnimation.frame = 0;
		leftAnimation.timer = 0;
	} else {
		rightAnimation.frame = 0;
		rightAnimation.timer = 0;
	}
}

/*
Attack Animator
*/

AttackAnimator::AttackAnimator(Animation animation) {
	this->animation = animation;
}

void AttackAnimator::update(sf::Sprite& sprite) {

}

sf::Texture* AttackAnimator::getCurrentFrame() {
	return animation.frames[animation.frame];
}