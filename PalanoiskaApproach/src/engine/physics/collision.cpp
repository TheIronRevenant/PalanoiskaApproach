#include <SFML/Graphics.hpp>
#include "collision.hpp"

/*
Very inefficiently checks collisions between two rects
*/

bool isColliding(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2) {
	sf::Rect<float> r1(rect1.getPosition(), rect1.getSize());
	sf::Rect<float> r2(rect2.getPosition(), rect2.getSize());

	return r1.intersects(r2);
}