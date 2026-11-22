#pragma once

#include <SFML/System/Vector2.hpp>

#include "SnakeBlock.h"
#include "Direction.h"

class Snake {
    private:
        bool alive;
        int tailLength;
        float speed;

        SnakeBlock* head;
        SnakeBlock* tail;

    public:
        // Constructors

        // default pos + speed
        Snake();
        // passed pos + speed
        Snake(sf::Vector2f position, float speed);
        // Methods
        void extend();
        void move(float dt);
        void turn(Direction turnDirection);
        void kill();

        void increaseSpeed();
        void decreaseSpeed();

        // Getters
        bool GetAlive();
        int GetTailLength();
        float GetSpeed();

        SnakeBlock* GetHead();
        SnakeBlock* GetTail();

        // Setters
        void SetAlive(bool in);
        void SettailLength(bool in);
        void SetSpeed(float in);
};