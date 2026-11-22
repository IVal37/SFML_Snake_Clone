#include "Apple.h"

#include "GameConfig.h"

using namespace sf;

// default pos constructor
Apple::Apple() {
    setSize(SQUARE_SIZE);
    setFillColor(sf::Color::Red);
    setPosition({ 200.f, 100.f });
    exists = true;
}

// passed pos constructor
Apple::Apple(Vector2f initPosition) {
    setSize(SQUARE_SIZE);
    setFillColor(Color::Red);
    setPosition(initPosition);
    exists = true;
}

// Getters
bool Apple::GetExists() { return this->exists; }

// Setters
void Apple::SetExists(bool in) { this->exists = in; }