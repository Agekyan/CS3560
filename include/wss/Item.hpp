#pragma once

#include <string>

namespace wss {

class Player;

class Item {
public:
    virtual ~Item() = default;

    virtual std::string name() const = 0;
    virtual bool repeating() const = 0;
    virtual void collect(Player& player) const = 0;
};

class FoodBonus final : public Item {
public:
    explicit FoodBonus(int amount, bool repeating = false)
        : amount_(amount), repeating_(repeating) {}

    std::string name() const override {
        return "Food Bonus";
    }

    bool repeating() const override {
        return repeating_;
    }

    void collect(Player& player) const override;

    int amount() const {
        return amount_;
    }

private:
    int amount_;
    bool repeating_;
};

class WaterBonus final : public Item {
public:
    explicit WaterBonus(int amount, bool repeating = false)
        : amount_(amount), repeating_(repeating) {}

    std::string name() const override {
        return "Water Bonus";
    }

    bool repeating() const override {
        return repeating_;
    }

    void collect(Player& player) const override;

    int amount() const {
        return amount_;
    }

private:
    int amount_;
    bool repeating_;
};

class GoldBonus final : public Item {
public:
    explicit GoldBonus(int amount, bool repeating = false)
        : amount_(amount), repeating_(repeating) {}

    std::string name() const override {
        return "Gold Bonus";
    }

    bool repeating() const override {
        return repeating_;
    }

    void collect(Player& player) const override;

    int amount() const {
        return amount_;
    }

private:
    int amount_;
    bool repeating_;
};

} // namespace wss
