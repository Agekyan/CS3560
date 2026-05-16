#pragma once

#include <memory>
#include <string>

#include "Path.hpp"
#include "Types.hpp"

namespace wss {

class Vision {
public:
    virtual ~Vision() = default;

    virtual std::string name() const = 0;

    virtual Position closestFood(const Map& map, const Position& from) const = 0;
    virtual Position closestWater(const Map& map, const Position& from) const = 0;
    virtual Position closestGold(const Map& map, const Position& from) const = 0;
    virtual Position closestTrader(const Map& map, const Position& from) const = 0;
    virtual Path easiestPath(const Map& map, const Position& from, const Position& to) const = 0;
};

} // namespace wss
