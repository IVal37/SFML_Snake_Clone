#pragma once

#include <iostream>

enum class Direction { Left, Right, Up, Down };
inline std::ostream& operator<<(std::ostream& os, Direction d) {
    switch (d) {
    case Direction::Up:   return os << "Up";
    case Direction::Down:   return os << "Down";
    case Direction::Left:   return os << "Left";
    case Direction::Right:   return os << "Right";
    }
    return os;
}