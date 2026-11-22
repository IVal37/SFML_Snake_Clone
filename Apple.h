#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Apple : public sf::RectangleShape {
private:
    bool exists;

public:
    // Constructors

    // Default pos
    Apple();
    // Passed pos
    Apple(sf::Vector2f initPosition);
    // Getters
    bool GetExists();

    // Setters
    void SetExists(bool in);
};