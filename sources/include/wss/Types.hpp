#pragma once

#include <string>

namespace wss {

struct Position {
    int row = 0;
    int col = 0;
};

enum class Direction {
    North,
    East,
    South,
    West
};

struct Cost {
    int movement = 0;
    int water = 0;
    int food = 0;
};

inline std::string to_string(Direction dir) {
    switch (dir) {
    case Direction::North: return "N";
    case Direction::East: return "E";
    case Direction::South: return "S";
    case Direction::West: return "W";
    default: return "?";
    }
}

} // namespace wss
