#pragma once

#include <string>

namespace wss {

class Terrain {
public:
    Terrain() = default;
    virtual ~Terrain() = default;

    virtual std::string name() const = 0;

    virtual int movementCost() const = 0;
    virtual int waterCost() const = 0;
    virtual int foodCost() const = 0;
};

class PlainTerrain : public Terrain {
public:
    std::string name() const override { return "Plain"; }
    int movementCost() const override { return 1; }
    int waterCost() const override { return 1; }
    int foodCost() const override { return 1; }
};

class SwampTerrain : public Terrain {
public:
    std::string name() const override { return "Swamp"; }
    int movementCost() const override { return 3; }
    int waterCost() const override { return 2; }
    int foodCost() const override { return 2; }
};

} // namespace wss
