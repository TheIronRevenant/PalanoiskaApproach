#ifndef PLAYERATTACK_H
#define PLAYERATTACK_H

#include "../engine/gameobject.hpp"
#include "../engine/animator.hpp"

namespace sf {
	class RenderWindow;
};

class PlayerAttack : public GameObject {
public:
	PlayerAttack() { terminated = false; }
	PlayerAttack(AttackAnimator animator);
	void create(float x, float y, bool rightFacing);
	void update();
	void draw(sf::RenderWindow& window);
	bool isTerminated() const { return terminated; }
private:
	bool terminated;
	AttackAnimator animator;
};

#endif