#ifndef PLAYERATTACK_H
#define PLAYERATTACK_H

#include "../engine/gameobject.hpp"
#include "../engine/animator.hpp"

namespace sf {
	class RenderWindow;
};

class PlayerAttack : public GameObject {
public:
	PlayerAttack() { terminated = false; damage = 0; }
	PlayerAttack(AttackAnimator&& animator, int damage);
	void create(float x, float y, bool rightFacing);
	void update();
	void draw(sf::RenderWindow& window);
	bool isTerminated() const { return terminated; }
	const AttackAnimator& getAnimator() const { return animator; }
	const int& getDamage() const { return damage; }
private:
	bool terminated;
	AttackAnimator animator;
	int damage;
};

#endif