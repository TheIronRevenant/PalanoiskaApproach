#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SFML/Graphics.hpp>
#include <vector>

//Player animation states
enum class AnimationStates { left, right };

struct Animation {
	std::vector<sf::Texture*> frames; //Frames
	int speed = 0; //How many updates till next frame
	int timer = 0; //Counts updates till next frame
	unsigned int frame = 0; //Current Frame
};

class PlayerAnimator {
public:
	PlayerAnimator() { currentAnimation = AnimationStates::right; }
	PlayerAnimator(Animation walk);
	void update(const float hVelocity, sf::Sprite& sprite);
	sf::Texture* getCurrentFrame();
	AnimationStates getState() { return currentAnimation; }
private:
	void swapAnimation(AnimationStates state, sf::Sprite& sprite);
	Animation walkAnimation;
	AnimationStates currentAnimation;
};

class AttackAnimator {
public:
	AttackAnimator() {}
	AttackAnimator(Animation animation);
	void update(sf::Sprite& sprite, bool& terminated);
	sf::Texture* getCurrentFrame();
	int getSpeed() const { return animation.speed;  }
	int getFrameCount() const { return animation.frames.size(); }
private:
	Animation animation;
};

class EnemyAnimator {
public:
	EnemyAnimator() { currentAnimation = AnimationStates::right; }
	EnemyAnimator(Animation animation);
	void update(bool moveRight, sf::Sprite& sprite);
	sf::Texture* getCurrentFrame();
	int getSpeed() const { return animation.speed; }
	int getFrameCount() const { return animation.frames.size(); }
	AnimationStates getState() { return currentAnimation; }
private:
	void swapAnimation(AnimationStates state, sf::Sprite& sprite);
	Animation animation;
	AnimationStates currentAnimation;
};

#endif