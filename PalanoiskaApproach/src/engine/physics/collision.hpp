#ifndef COLLISION_H
#define COLLISION_H

namespace sf {
	class RectangleShape;
};

bool isColliding(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2);

#endif