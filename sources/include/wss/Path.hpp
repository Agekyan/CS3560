#pragma once

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Types.hpp"

namespace wss {

class Path {
public:
    Path() = default;

    Path(std::vector<Direction> moves, Cost cost)
        : moves_(std::move(moves)), cost_(cost) {}

    const std::vector<Direction>& moves() const {
        return moves_;
    }

    const Cost& cost() const {
        return cost_;
    }

    bool empty() const {
        return moves_.empty();
    }

    std::string describe() const {
        std::ostringstream out;
        out << "Path{";
        for (std::size_t index = 0; index < moves_.size(); ++index) {
            out << to_string(moves_[index]);
            if (index + 1 < moves_.size()) {
                out << " -> ";
            }
        }
        out << " | movement=" << cost_.movement
            << ", water=" << cost_.water
            << ", food=" << cost_.food
            << "}";
        return out.str();
    }

private:
    std::vector<Direction> moves_;
    Cost cost_;
};

} // namespace wss
