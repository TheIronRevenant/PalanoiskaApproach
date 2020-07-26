#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <unordered_map>
#include "../engine/gameobject.hpp"
#include "../engine/animator.hpp"
#include "playerattack.hpp"

class Scene;

class Player : public GameObject {
public:
	Player() {}
	Player(unsigned int gridx, unsigned int gridy, PlayerAnimator animator);
	void update() {} //Override the abstract functions
	void update(const std::vector<Mesh>& meshes, Scene& parentScene);
	void draw(sf::RenderWindow& window);
	void addAttack(std::string name, PlayerAttack& attack);
private:
	PlayerAnimator animator;
	std::unordered_map<std::string, PlayerAttack> attacks;

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

#endif