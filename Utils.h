#pragma once

#include <SFML/System/Vector2.hpp>

#include "Direction.h"

// Get the direction vector based on the set current direction
sf::Vector2f GetDirectionVec(Direction direction);
// Get the direction vector based on the opposite of the set current direction
sf::Vector2f GetOppositeDirectionVec(Direction direction);


// Check next direction validity, return true if next direction is opposite of current direction
bool ValidTurnDirection(Direction current, Direction next);