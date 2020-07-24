#include "animator.hpp"

PlayerAnimator::PlayerAnimator(Animation left, Animation right) {
	leftAnimation = left;
	rightAnimation = right;
	swapAnimation(AnimationStates::right);
}

void PlayerAnimator::update() {

}

void PlayerAnimator::swapAnimation(AnimationStates state) {
	currentAnimation = state;

	if (state == AnimationStates::left) {
		animIterator = leftAnimation.frames.begin();
	} else {
		animIterator = rightAnimation.frames.begin();
	}
}