#ifndef PLAYER_H
#define PLAYER_H

#include "../engine/gameobject.hpp"
#include "../engine/animator.hpp"

class Scene;

class Player : public GameObject {
public:
	Player() {}
	Player(unsigned int gridx, unsigned int gridy, PlayerAnimator animator);
	void update() {} //Override the abstract functions
	void update(const std::vector<Mesh>& meshes);
	void draw(sf::RenderWindow& window);
private:
	PlayerAnimator animator;

	float hVelocity;
	float hAcceleration;
	float hMax;

	bool onGround;
	bool prevJump;
	float vVelocity;
	float vJumpAcceleration;
	float vTerminalVelocity;
	float vAcceleration;
};

class PlayerAttack : public GameObject {
public:
	PlayerAttack() {}
	PlayerAttack(float x, float y, sf::Texture& texture);
	void update();
	void draw(sf::RenderWindow& window);
private:
	int timer;
	int speed;
};

#endif