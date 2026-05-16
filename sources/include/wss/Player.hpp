#pragma once

#include <algorithm>
#include <cmath>
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
        int effMovement = static_cast<int>(std::ceil(cost.movement * efficiencyMultiplier_));
        int effWater = static_cast<int>(std::ceil(cost.water * efficiencyMultiplier_));
        int effFood = static_cast<int>(std::ceil(cost.food * efficiencyMultiplier_));
        if (currentStrength_ < effMovement || currentWater_ < effWater || currentFood_ < effFood) {
            return false;
        }
        currentStrength_ -= effMovement;
        currentWater_ -= effWater;
        currentFood_ -= effFood;
        return true;
    }

    bool canSpend(const Cost& cost) const {
        return currentStrength_ >= cost.movement && currentWater_ >= cost.water && currentFood_ >= cost.food;
    }

    void recoverAtRest() {
        currentStrength_ = std::min(maxStrength_, currentStrength_ + 2);
    }

    bool restInTerrain(const Cost& terrainCost) {
        int halfWater = static_cast<int>(std::ceil((terrainCost.water * 0.5) * efficiencyMultiplier_));
        int halfFood = static_cast<int>(std::ceil((terrainCost.food * 0.5) * efficiencyMultiplier_));
        if (currentWater_ < halfWater || currentFood_ < halfFood) {
            return false;
        }
        currentWater_ -= halfWater;
        currentFood_ -= halfFood;
        recoverAtRest();
        return true;
    }

    void applyEfficiency(double multiplier, int turns) {
        if (multiplier <= 0.0) return;
        efficiencyMultiplier_ = multiplier;
        efficiencyTurns_ = turns;
    }

    void tickTurn() {
        if (efficiencyTurns_ > 0) {
            --efficiencyTurns_;
            if (efficiencyTurns_ <= 0) {
                efficiencyMultiplier_ = 1.0;
                efficiencyTurns_ = 0;
            }
        }
    }

    void multiplyMaxStrength(int factor) {
        if (factor <= 0) return;
        maxStrength_ *= factor;
        currentStrength_ = std::min(currentStrength_ * factor, maxStrength_);
    }

    void multiplyMaxWater(int factor) {
        if (factor <= 0) return;
        maxWater_ *= factor;
        currentWater_ = std::min(currentWater_ * factor, maxWater_);
    }

    void multiplyMaxFood(int factor) {
        if (factor <= 0) return;
        maxFood_ *= factor;
        currentFood_ = std::min(currentFood_ * factor, maxFood_);
    }

    void increaseMaxStrength(int amount) {
        if (amount <= 0) return;
        maxStrength_ += amount;
        currentStrength_ = std::min(currentStrength_ + amount, maxStrength_);
    }

    void increaseMaxWater(int amount) {
        if (amount <= 0) return;
        maxWater_ += amount;
        currentWater_ = std::min(currentWater_ + amount, maxWater_);
    }

    void increaseMaxFood(int amount) {
        if (amount <= 0) return;
        maxFood_ += amount;
        currentFood_ = std::min(currentFood_ + amount, maxFood_);
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
    double efficiencyMultiplier_ = 1.0;
    int efficiencyTurns_ = 0;
};

} // namespace wss
