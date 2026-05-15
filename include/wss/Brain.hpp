#pragma once

#include <memory>
#include <string>

#include "Player.hpp"
#include "Vision.hpp"

namespace wss {

class Map;

class Brain {
public:
    virtual ~Brain() = default;

    virtual std::string name() const = 0;
    virtual void makeMove(Player& player, const Map& map) const = 0;
};

class GreedyBrain final : public Brain {
public:
    std::string name() const override {
        return "Greedy Brain";
    }

    void makeMove(Player& player, const Map& map) const override;
};

class CautiousBrain final : public Brain {
public:
    std::string name() const override {
        return "Cautious Brain";
    }

    void makeMove(Player& player, const Map& map) const override;
};

class OpportunistBrain final : public Brain {
public:
    std::string name() const override {
        return "Opportunist Brain";
    }

    void makeMove(Player& player, const Map& map) const override;
};

} // namespace wss
