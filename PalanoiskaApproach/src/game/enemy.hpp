#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../engine/gameobject.hpp"
#include "../engine/floatingtext.hpp"
#include "../engine/animator.hpp"

class PlayerAttack;

enum class EnemyType { OrangeTest, PinkTest };

class Enemy : public GameObject {
public:
	Enemy() { moveRight = true; speed = 0.5f; currentHp = 10; damage = 5; type = EnemyType::OrangeTest; }
	Enemy(unsigned int gridx, unsigned int gridy, EnemyAnimator&& animator, EnemyType type);
	void update() {} //Override abstract
	void update(const std::vector<Mesh>& meshes, std::vector<PlayerAttack>& attacks, std::vector<FloatingText>& floatText, bool takeDamage);
	void draw(sf::RenderWindow& window);
	bool isDead() const { return currentHp <= 0; }
	const int& getDamage() const { return damage; }
private:
	EnemyAnimator animator;
	EnemyType type;
	bool moveRight;
	float speed;
	int currentHp;
	int damage;
};

#endif