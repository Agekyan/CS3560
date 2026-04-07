#include "wss/Item.hpp"

#include "wss/Player.hpp"

namespace wss {

void FoodBonus::collect(Player& player) const {
    player.addFood(amount_);
}

void WaterBonus::collect(Player& player) const {
    player.addWater(amount_);
}

void GoldBonus::collect(Player& player) const {
    player.addGold(amount_);
}

} // namespace wss
