#pragma once

#include <optional>
#include <queue>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Direction.h"

struct NextMove {
    Direction direction;
    sf::Vector2f coord;
};

class SnakeBlock : public sf::RectangleShape {
private:
    Direction currDirection;

    std::optional<SnakeBlock*> nextBlock;
    std::queue<NextMove> nextMoves;
public:
    // Constructors

    // default pos + dir
    SnakeBlock();
    // default dir
    SnakeBlock(sf::Vector2f initPosition);
    // default pos
    SnakeBlock(Direction direction);
    // passed pos + dir
    SnakeBlock(sf::Vector2f initPosition, Direction direction);

    // methods
    void PopNextMoves();
    void PushNextMoves(NextMove in);

    // Getters
    Direction GetCurrDirection();
    std::optional<SnakeBlock*> GetNextBlock();
    std::queue<NextMove> GetNextMoves();


    // Setters
    void SetCurrDirection(Direction direction);
    void SetNextBlock(SnakeBlock* next);
    void SetNextMoves(std::queue<NextMove> in);
};