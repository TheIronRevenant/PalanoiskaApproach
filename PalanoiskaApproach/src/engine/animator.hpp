#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SFML/Graphics.hpp>
#include <vector>

enum AnimationStates { left, right };

struct Animation {
	std::vector<sf::Texture*> frames;
	int speed;
	int timer = 0;
	int frame = 0;
};

class PlayerAnimator {
public:
	PlayerAnimator() {}
	PlayerAnimator(Animation left, Animation right);
	void update();
	sf::Texture* getCurrentFrame() { return *animIterator; }
private:
	void swapAnimation(AnimationStates state);
	Animation leftAnimation;
	Animation rightAnimation;
	AnimationStates currentAnimation;
	std::vector<sf::Texture*>::const_iterator animIterator;
};

#endif