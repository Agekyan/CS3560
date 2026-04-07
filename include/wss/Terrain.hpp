#pragma once

#include <string>
#include <utility>

#include "Types.hpp"

namespace wss {

class Terrain {
public:
    Terrain(std::string name, Cost enterCost)
        : name_(std::move(name)), enterCost_(enterCost) {}

    virtual ~Terrain() = default;

    const std::string& name() const {
        return name_;
    }

    const Cost& enterCost() const {
        return enterCost_;
    }

    bool canEnter(const Cost& available) const {
        return available.movement >= enterCost_.movement &&
               available.water >= enterCost_.water &&
               available.food >= enterCost_.food;
    }

private:
    std::string name_;
    Cost enterCost_;
};

class PlainsTerrain final : public Terrain {
public:
    PlainsTerrain() : Terrain("Plains", {1, 1, 1}) {}
};

class MountainTerrain final : public Terrain {
public:
    MountainTerrain() : Terrain("Mountain", {3, 2, 2}) {}
};

class DesertTerrain final : public Terrain {
public:
    DesertTerrain() : Terrain("Desert", {2, 3, 2}) {}
};

class SwampTerrain final : public Terrain {
public:
    SwampTerrain() : Terrain("Swamp", {2, 2, 3}) {}
};

class ForestTerrain final : public Terrain {
public:
    ForestTerrain() : Terrain("Forest", {2, 1, 2}) {}
};

} // namespace wss
