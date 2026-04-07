#pragma once

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cell.hpp"
#include "Types.hpp"

namespace wss {

class Map {
public:
    Map(int width, int height)
        : width_(width), height_(height) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Map dimensions must be positive.");
        }
        cells_.resize(static_cast<std::size_t>(width_ * height_));
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    bool inBounds(Position position) const {
        return position.row >= 0 && position.row < height_ && position.col >= 0 && position.col < width_;
    }

    Cell& at(Position position) {
        return cells_.at(index(position));
    }

    const Cell& at(Position position) const {
        return cells_.at(index(position));
    }

    std::string describe() const {
        std::ostringstream out;
        out << "Map(" << width_ << "x" << height_ << ")";
        return out.str();
    }

private:
    std::size_t index(Position position) const {
        if (!inBounds(position)) {
            throw std::out_of_range("Position outside map bounds.");
        }
        return static_cast<std::size_t>(position.row * width_ + position.col);
    }

    int width_ = 0;
    int height_ = 0;
    std::vector<Cell> cells_;
};

} // namespace wss
