#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <unordered_map>
#include "../engine/gameobject.hpp"
#include "../engine/animator.hpp"
#include "playerattack.hpp"
#include "enemy.hpp"
#include "interactable.hpp"
#include "../engine/floatingtext.hpp"

class Scene;

class Player : public GameObject {
public:
	Player() {
		maxHp = 100; currentHp = 100; invincible = false; iframes = 20; iframeTimer = 0;
		hAcceleration = 0.f; hMax = 0.f; hVelocity = 0.f;
		onGround = true; prevJump = false; vAcceleration = 0.f; vJumpAcceleration = 0.f; vTerminalVelocity = 0.f; vVelocity = 0.f;
		attacking = false; attackSpeed = 0; attackTimer = 0; prevAttack = false;
		prevInteract = false; }
	Player(unsigned int gridx, unsigned int gridy, PlayerAnimator&& animator);
	void update() {} //Override the abstract functions
	void update(const std::vector<Mesh>& meshes, const std::vector<Enemy>& enemies, std::vector<FloatingText>& floatText, std::vector<Interactable>& interactables, Scene& parentScene);
	void draw(sf::RenderWindow& window);
	void addAttack(std::string name, PlayerAttack& attack);
	void reset(); //Reset when dead
	const int& getMaxHp() const { return maxHp; }
	const int& getCurrentHp() const { return currentHp; }
	bool isDead() const { return currentHp <= 0; }
private:
	int maxHp;
	int currentHp;
	bool invincible;
	int iframes;
	int iframeTimer;

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

	bool prevInteract;
};

#endif