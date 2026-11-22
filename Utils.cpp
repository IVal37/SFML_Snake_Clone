#include "utils.h"

#include "direction.h"

sf::Vector2f GetDirectionVec(Direction direction) {
    switch (direction) {
        // left
    case Direction::Left:
        return { -1.f, 0.f };
        // right
    case Direction::Right:
        return { 1.f, 0.f };
        // up
    case Direction::Up:
        return { 0.f, -1.f };
        // down
    case Direction::Down:
        return { 0.f, 1.f };
    }

    return { 0.f, 0.f };
}

sf::Vector2f GetOppositeDirectionVec(Direction direction) {
    switch (direction) {
        // left
    case Direction::Left:
        return { 1.f, 0.f };
        // right
    case Direction::Right:
        return { -1.f, 0.f };
        // up
    case Direction::Up:
        return { 0.f, 1.f };
        // down
    case Direction::Down:
        return { 0.f, -1.f };
    }

    return { 0.f, 0.f };
}

bool ValidTurnDirection(Direction current, Direction next) {
    return !(
        (current == Direction::Left && next == Direction::Right) ||
        (current == Direction::Right && next == Direction::Left) ||
        (current == Direction::Up && next == Direction::Down) ||
        (current == Direction::Down && next == Direction::Up) ||
        (current == next)
        );
}