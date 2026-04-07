#pragma once

#include <string>

namespace wss {

class Player;

class Trader {
public:
    virtual ~Trader() = default;

    virtual std::string name() const = 0;
    virtual bool repeating() const {
        return true;
    }

    virtual bool makeOffer(Player& player) = 0;
};

class CalmTrader final : public Trader {
public:
    std::string name() const override {
        return "Calm Trader";
    }

    bool makeOffer(Player& player) override;
};

class HotheadedTrader final : public Trader {
public:
    std::string name() const override {
        return "Hotheaded Trader";
    }

    bool makeOffer(Player& player) override;
};

class RandomTrader final : public Trader {
public:
    std::string name() const override {
        return "Random Trader";
    }

    bool makeOffer(Player& player) override;
};

} // namespace wss
