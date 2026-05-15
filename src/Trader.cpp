#include "wss/Trader.hpp"

#include <random>

#include "wss/Player.hpp"

namespace wss {

bool CalmTrader::makeOffer(Player& player) {
    static std::mt19937 rng(3560);
    std::uniform_int_distribution<int> pick(0, 2);

    if (player.currentGold() >= 1) {
            if (!player.spendGold(1)) {
                return false;
            }
            switch (pick(rng)) {
            case 0:
                player.addFood(3);
                break;
            case 1:
                player.addWater(3);
                break;
            default:
                player.addGold(2);
                break;
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
    static std::mt19937 rng(3560 + 17);
    std::uniform_int_distribution<int> pick(0, 2);

    if (player.currentGold() >= 2) {
            if (!player.spendGold(2)) {
                return false;
            }
            switch (pick(rng)) {
            case 0:
                player.addWater(5);
                break;
            case 1:
                player.addFood(4);
                break;
            default:
                player.addGold(3);
                break;
            }
            return true;
        }
    return false;
}

bool RandomTrader::makeOffer(Player& player) {
    static std::mt19937 rng(3560);
    std::uniform_int_distribution<int> pick(0, 2);

    if (player.currentGold() < 1) {
        return false;
    }

    if (!player.spendGold(1)) {
        return false;
    }

        switch (pick(rng)) {
        case 0:
            player.addFood(3);
            break;
        case 1:
            player.addWater(3);
            break;
        default:
            player.addGold(2);
            break;
        }
    return true;
}

bool SurvivalTrader::makeOffer(Player& player) {
    if (player.currentGold() < 1) {
        return false;
    }

    if (player.currentFood() <= 2 || player.currentWater() <= 2) {
        if (!player.spendGold(1)) {
            return false;
        }
        player.addFood(2);
        player.addWater(2);
        return true;
    }

    if (player.currentGold() >= 2) {
        if (!player.spendGold(2)) {
            return false;
        }
        player.addFood(1);
        player.addWater(1);
        player.addGold(1);
        return true;
    }

    return false;
}

} // namespace wss
