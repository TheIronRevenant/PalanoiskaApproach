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
	Player() {
		maxHp = 100; currentHp = 100;
		hAcceleration = 0.f; hMax = 0.f; hVelocity = 0.f;
		onGround = true; prevJump = false; vAcceleration = 0.f; vJumpAcceleration = 0.f; vTerminalVelocity = 0.f; vVelocity = 0.f;
		attacking = false; attackSpeed = 0; attackTimer = 0; prevAttack = false; }
	Player(unsigned int gridx, unsigned int gridy, PlayerAnimator&& animator);
	void update() {} //Override the abstract functions
	void update(const std::vector<Mesh>& meshes, Scene& parentScene);
	void draw(sf::RenderWindow& window);
	void addAttack(std::string name, PlayerAttack& attack);
	const int& getMaxHp() const { return maxHp; }
	const int& getCurrentHp() const { return currentHp; }
private:
	int maxHp;
	int currentHp;

	PlayerAnimator animator;
	std::unordered_map<std::string, PlayerAttack> attacks;
	bool attacking;
	bool prevAttack;
	int attackSpeed;
	int attackTimer;

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