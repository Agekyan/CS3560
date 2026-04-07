#include "wss/Trader.hpp"

#include <random>

#include "wss/Player.hpp"

namespace wss {

bool CalmTrader::makeOffer(Player& player) {
    if (player.currentGold() >= 1) {
        if (!player.spendGold(1)) {
            return false;
        }
        if (player.currentWater() < player.currentFood()) {
            player.addWater(2);
        } else {
            player.addFood(2);
        }
        return true;
    }

    if (player.currentFood() >= 2 && player.currentWater() <= 2) {
        if (!player.spendFood(2)) {
            return false;
        }
        player.addWater(2);
        return true;
    }

    return false;
}

bool HotheadedTrader::makeOffer(Player& player) {
    if (player.currentGold() >= 2) {
        if (!player.spendGold(2)) {
            return false;
        }
        player.addWater(3);
        return true;
    }
    return false;
}

bool RandomTrader::makeOffer(Player& player) {
    static std::mt19937 rng(3560);
    std::uniform_int_distribution<int> pick(0, 1);

    if (player.currentGold() < 1) {
        return false;
    }

    if (!player.spendGold(1)) {
        return false;
    }

    if (pick(rng) == 0) {
        player.addFood(2);
    } else {
        player.addWater(2);
    }
    return true;
}

} // namespace wss
