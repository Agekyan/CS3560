#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Types.hpp"

namespace wss {

class Brain;
class Vision;

class Player {
public:
    Player(int maxStrength, int maxWater, int maxFood)
        : maxStrength_(maxStrength), maxWater_(maxWater), maxFood_(maxFood),
          currentStrength_(maxStrength), currentWater_(maxWater), currentFood_(maxFood),
          currentGold_(0) {}

    int maxStrength() const {
        return maxStrength_;
    }

    int maxWater() const {
        return maxWater_;
    }

    int maxFood() const {
        return maxFood_;
    }

    int currentStrength() const {
        return currentStrength_;
    }

    int currentWater() const {
        return currentWater_;
    }

    int currentFood() const {
        return currentFood_;
    }

    int currentGold() const {
        return currentGold_;
    }

    const Position& position() const {
        return position_;
    }

    void setPosition(Position position) {
        position_ = position;
    }

    void setVision(std::shared_ptr<Vision> vision) {
        vision_ = std::move(vision);
    }

    void setBrain(std::shared_ptr<Brain> brain) {
        brain_ = std::move(brain);
    }

    std::shared_ptr<Vision> vision() const {
        return vision_;
    }

    std::shared_ptr<Brain> brain() const {
        return brain_;
    }

    bool spend(const Cost& cost) {
        if (currentStrength_ < cost.movement || currentWater_ < cost.water || currentFood_ < cost.food) {
            return false;
        }
        currentStrength_ -= cost.movement;
        currentWater_ -= cost.water;
        currentFood_ -= cost.food;
        return true;
    }

    bool canSpend(const Cost& cost) const {
        return currentStrength_ >= cost.movement && currentWater_ >= cost.water && currentFood_ >= cost.food;
    }

    void recoverAtRest() {
        currentStrength_ = std::min(maxStrength_, currentStrength_ + 2);
    }

    bool restInTerrain(const Cost& terrainCost) {
        const int halfWater = (terrainCost.water + 1) / 2;
        const int halfFood = (terrainCost.food + 1) / 2;
        if (currentWater_ < halfWater || currentFood_ < halfFood) {
            return false;
        }
        currentWater_ -= halfWater;
        currentFood_ -= halfFood;
        recoverAtRest();
        return true;
    }

    bool spendGold(int amount) {
        if (amount < 0 || currentGold_ < amount) {
            return false;
        }
        currentGold_ -= amount;
        return true;
    }

    bool spendFood(int amount) {
        if (amount < 0 || currentFood_ < amount) {
            return false;
        }
        currentFood_ -= amount;
        return true;
    }

    bool spendWater(int amount) {
        if (amount < 0 || currentWater_ < amount) {
            return false;
        }
        currentWater_ -= amount;
        return true;
    }

    void addWater(int amount) {
        currentWater_ = std::min(maxWater_, currentWater_ + amount);
    }

    void addFood(int amount) {
        currentFood_ = std::min(maxFood_, currentFood_ + amount);
    }

    void addGold(int amount) {
        currentGold_ += amount;
    }

    std::string describe() const {
        std::ostringstream out;
        out << "Player(strength=" << currentStrength_ << '/' << maxStrength_
            << ", water=" << currentWater_ << '/' << maxWater_
            << ", food=" << currentFood_ << '/' << maxFood_
            << ", gold=" << currentGold_ << ")";
        return out.str();
    }

private:
    int maxStrength_;
    int maxWater_;
    int maxFood_;
    int currentStrength_;
    int currentWater_;
    int currentFood_;
    int currentGold_;
    Position position_;
    std::shared_ptr<Vision> vision_;
    std::shared_ptr<Brain> brain_;
};

} // namespace wss
