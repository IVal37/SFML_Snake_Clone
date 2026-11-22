#include "Snake.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp >

#include "GameConfig.h"
#include "Utils.h"

using namespace sf;

// default pos + speed
Snake::Snake() {
    alive = true;
    tailLength = 0;
    speed = BASE_SPEED;

    SnakeBlock* tempBlockPtr = new SnakeBlock();
    head = tempBlockPtr;
    tail = tempBlockPtr;
}
// passed pos + speed
Snake::Snake(Vector2f position, float speed) {
    alive = true;
    tailLength = 0;
    this->speed = speed;

    SnakeBlock* tempBlockPtr = new SnakeBlock(position);
    head = tempBlockPtr;
    tail = tempBlockPtr;
}

// Methods
void Snake::extend() {
    tailLength++;

    Vector2f newTailPosition = tail->getPosition() + GetOppositeDirectionVec(tail->GetCurrDirection()) * UNIT_SIZE;
    SnakeBlock* tempBlockPtr = new SnakeBlock(newTailPosition, tail->GetCurrDirection());
    if (!tail->GetNextMoves().empty()) {
        tempBlockPtr->SetNextMoves(tail->GetNextMoves());
    }
    tail->SetNextBlock(tempBlockPtr);
    tail = tempBlockPtr;

}

void Snake::move(float dt) {
    // move head
    head->setPosition(head->getPosition() + GetDirectionVec(head->GetCurrDirection()) * speed * dt);
    if (tailLength <= 0) { return; }

    // move body
    SnakeBlock* iterBlock = head->GetNextBlock().value();
    while (true) {
        Direction currDirection = iterBlock->GetCurrDirection();
        Vector2f currPos = iterBlock->getPosition();

        //update next
        if (!iterBlock->GetNextMoves().empty()) {
            auto [nextDirection, turnPos] = iterBlock->GetNextMoves().front();

            // set up if logic - determine working with x or y and when curr coords "pass" the turn coords
            bool isVertical = (currDirection == Direction::Up || currDirection == Direction::Down);
            bool isPositive = (currDirection == Direction::Down || currDirection == Direction::Right);

            // extract x or y vals for logic
            float currCoord = isVertical ? currPos.y : currPos.x;
            float turnCoord = isVertical ? turnPos.y : turnPos.x;

            float distanceToTurn = isPositive ? turnCoord - currCoord : currCoord - turnCoord;
            float remainingDistance = speed * dt - distanceToTurn;

            if (distanceToTurn <= speed * dt) {
                iterBlock->setPosition(
                    Vector2f{
                        iterBlock->getPosition() +
                        GetDirectionVec(currDirection) * distanceToTurn +
                        GetDirectionVec(nextDirection) * remainingDistance
                    }
                );
                iterBlock->SetCurrDirection(nextDirection);
                // pop
                iterBlock->PopNextMoves();
            }
            else {
                iterBlock->setPosition(iterBlock->getPosition() + GetDirectionVec(iterBlock->GetCurrDirection()) * speed * dt);
            }
        }
        else {
            iterBlock->setPosition(iterBlock->getPosition() + GetDirectionVec(iterBlock->GetCurrDirection()) * speed * dt);
        }
        // loop to next block or break if tail
        if (iterBlock == tail) { break; }
        iterBlock = iterBlock->GetNextBlock().value();
    }
}

void Snake::turn(Direction turnDirection) {
    Vector2f currHeadCoords = head->getPosition();

    bool isVertical = (turnDirection == Direction::Up || turnDirection == Direction::Down);
    bool isPositive = (turnDirection == Direction::Down || turnDirection == Direction::Right);
    Vector2f roundedCoords = isVertical ? Vector2f{ (isPositive ? ceil(currHeadCoords.x) : floor(currHeadCoords.x)), currHeadCoords.y } : Vector2f{ currHeadCoords.x, (isPositive ? ceil(currHeadCoords.y) : floor(currHeadCoords.y)) };

    head->SetCurrDirection(turnDirection);
    head->setPosition(roundedCoords);

    if (tailLength <= 0) { return; }

    // add next moves to tail blocks
    NextMove nextMove = { turnDirection, roundedCoords };
    SnakeBlock* iterBlock = head->GetNextBlock().value();

    while (true) {
        iterBlock->PushNextMoves(nextMove);
        if (iterBlock == tail) { break; }
        iterBlock = iterBlock->GetNextBlock().value();
    }
}

void Snake::kill() {
    alive = false;

    SnakeBlock* tempBlockPtr = head;
    while (true) {
        tempBlockPtr->setFillColor(Color::Blue);
        if (!tempBlockPtr->GetNextBlock().has_value() || tempBlockPtr->GetNextBlock().value() == nullptr) { return; }
        tempBlockPtr = tempBlockPtr->GetNextBlock().value();
    }
}

void Snake::increaseSpeed() {
    speed += SPEED_MOD;
}
void Snake::decreaseSpeed() {
    speed -= SPEED_MOD;
}

// Getters
bool Snake::GetAlive() { return alive; }
int Snake::GetTailLength() { return tailLength; }
float Snake::GetSpeed() { return speed; }

SnakeBlock* Snake::GetHead() { return head; }
SnakeBlock* Snake::GetTail() { return tail; }

// Setters
void Snake::SetAlive(bool in) { alive = in; }
void Snake::SettailLength(bool in) { tailLength = in; }
void Snake::SetSpeed(float in) { speed = in; }