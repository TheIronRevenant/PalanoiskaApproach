#ifndef PLAYERATTACK_H
#define PLAYERATTACK_H

#include "../engine/gameobject.hpp"
#include "../engine/animator.hpp"

namespace sf {
	class RenderWindow;
};

class PlayerAttack : public GameObject {
public:
	PlayerAttack() {}
	PlayerAttack(AttackAnimator animator);
	void create(float x, float y, bool rightFacing);
	void update();
	void draw(sf::RenderWindow& window);
private:
	AttackAnimator animator;
};

#endif