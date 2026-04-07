#pragma once

#include <string>

namespace wss {

enum class Direction {
    North,
    South,
    East,
    West,
    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest
};

struct Position {
    int row = 0;
    int col = 0;
};

struct Cost {
    int movement = 0;
    int water = 0;
    int food = 0;
};

inline std::string to_string(Direction direction) {
    switch (direction) {
    case Direction::North:
        return "North";
    case Direction::South:
        return "South";
    case Direction::East:
        return "East";
    case Direction::West:
        return "West";
    case Direction::NorthEast:
        return "NorthEast";
    case Direction::NorthWest:
        return "NorthWest";
    case Direction::SouthEast:
        return "SouthEast";
    case Direction::SouthWest:
        return "SouthWest";
    }
    return "Unknown";
}

} // namespace wss
