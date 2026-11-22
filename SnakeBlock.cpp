#include "SnakeBlock.h"

#include "GameConfig.h"

using namespace std;
using namespace sf;

// Constructors
// default pos + dir
SnakeBlock::SnakeBlock() {
    setSize(SQUARE_SIZE);
    setFillColor(Color::Green);
    setPosition({ 100.f, 100.f });
    setOutlineColor(Color::White);
    setOutlineThickness(2.f);
    currDirection = Direction::Right;
}
// default dir
SnakeBlock::SnakeBlock(Vector2f initPosition) {
    setSize(SQUARE_SIZE);
    setFillColor(Color::Green);
    setPosition(initPosition);
    setOutlineColor(Color::White);
    setOutlineThickness(2.f);
    currDirection = Direction::Right;
}
// default pos
SnakeBlock::SnakeBlock(Direction direction) {
    setSize(SQUARE_SIZE);
    setFillColor(Color::Green);
    setPosition({ 100.f, 100.f });
    setOutlineColor(Color::White);
    setOutlineThickness(2.f);
    currDirection = direction;
}
// passed pos + dir
SnakeBlock::SnakeBlock(Vector2f initPosition, Direction direction) {
    setSize(SQUARE_SIZE);
    setFillColor(Color::Green);
    setPosition(initPosition);
    setOutlineColor(Color::White);
    setOutlineThickness(2.f);
    currDirection = direction;
}

// methods
void SnakeBlock::PopNextMoves() {
    nextMoves.pop();
}
void SnakeBlock::PushNextMoves(NextMove in) {
    nextMoves.push(in);
}

// Getters
Direction SnakeBlock::GetCurrDirection() { return currDirection; }
optional<SnakeBlock*> SnakeBlock::GetNextBlock() { return nextBlock; }
queue<NextMove> SnakeBlock::GetNextMoves() { return nextMoves; }


// Setters
void SnakeBlock::SetCurrDirection(Direction direction) { currDirection = direction; }
void SnakeBlock::SetNextBlock(SnakeBlock* next) { nextBlock = next; }
void SnakeBlock::SetNextMoves(queue<NextMove> in) { nextMoves = in; }
