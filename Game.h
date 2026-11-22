#pragma once

#include <random>

#include <SFML/Graphics.hpp>

#include "Snake.h"
#include "Apple.h"

class Game {
	private:
		sf::RenderWindow window;
		sf::Vector2f windowSize;

		sf::Clock clock;
		std::mt19937 gen;

		Snake snake;
		Apple apple;

	public:
		// Constructor
		Game();

		// Methods
		void run();
};