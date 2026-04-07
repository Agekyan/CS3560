#pragma once

#include <memory>
#include <string>

#include "Map.hpp"
#include "Path.hpp"

namespace wss {

class Player;

class Vision {
public:
    virtual ~Vision() = default;

    virtual std::string name() const = 0;

    virtual Path closestFood(const Map& map, const Position& origin) const = 0;
    virtual Path closestWater(const Map& map, const Position& origin) const = 0;
    virtual Path closestGold(const Map& map, const Position& origin) const = 0;
    virtual Path closestTrader(const Map& map, const Position& origin) const = 0;

    virtual Path easiestPath(const Map& map, const Position& origin) const = 0;

    virtual Path secondClosestFood(const Map& map, const Position& origin) const = 0;
    virtual Path secondClosestWater(const Map& map, const Position& origin) const = 0;
    virtual Path secondClosestGold(const Map& map, const Position& origin) const = 0;
    virtual Path secondClosestTrader(const Map& map, const Position& origin) const = 0;
};

class CautiousVision final : public Vision {
public:
    std::string name() const override {
        return "Cautious Vision";
    }

    Path closestFood(const Map& map, const Position& origin) const override;
    Path closestWater(const Map& map, const Position& origin) const override;
    Path closestGold(const Map& map, const Position& origin) const override;
    Path closestTrader(const Map& map, const Position& origin) const override;
    Path easiestPath(const Map& map, const Position& origin) const override;
    Path secondClosestFood(const Map& map, const Position& origin) const override;
    Path secondClosestWater(const Map& map, const Position& origin) const override;
    Path secondClosestGold(const Map& map, const Position& origin) const override;
    Path secondClosestTrader(const Map& map, const Position& origin) const override;
};

class WideVision final : public Vision {
public:
    std::string name() const override {
        return "Wide Vision";
    }

    Path closestFood(const Map& map, const Position& origin) const override;
    Path closestWater(const Map& map, const Position& origin) const override;
    Path closestGold(const Map& map, const Position& origin) const override;
    Path closestTrader(const Map& map, const Position& origin) const override;
    Path easiestPath(const Map& map, const Position& origin) const override;
    Path secondClosestFood(const Map& map, const Position& origin) const override;
    Path secondClosestWater(const Map& map, const Position& origin) const override;
    Path secondClosestGold(const Map& map, const Position& origin) const override;
    Path secondClosestTrader(const Map& map, const Position& origin) const override;
};

} // namespace wss
