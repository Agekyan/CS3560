#pragma once

#include <memory>
#include <string>

#include "Types.hpp"

namespace wss {

class Player;

class Trader {
public:
    virtual ~Trader() = default;

    virtual std::string name() const = 0;
    virtual void interact(Player& player) = 0;
};

class CalmTrader : public Trader {
public:
    std::string name() const override { return "Calm Trader"; }
    void interact(Player& player) override;
};

class HotheadedTrader : public Trader {
public:
    std::string name() const override { return "Hotheaded Trader"; }
    void interact(Player& player) override;
};

class RandomTrader : public Trader {
public:
    std::string name() const override { return "Random Trader"; }
    void interact(Player& player) override;
};

class SurvivalTrader : public Trader {
public:
    std::string name() const override { return "Survival Trader"; }
    void interact(Player& player) override;
};

} // namespace wss
