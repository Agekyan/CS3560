#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "wss/Brain.hpp"
#include "wss/Item.hpp"
#include "wss/Map.hpp"
#include "wss/Player.hpp"
#include "wss/Terrain.hpp"
#include "wss/Trader.hpp"
#include "wss/Vision.hpp"

namespace {

enum class Difficulty {
    Easy,
    Normal,
    Hard
};

wss::Position applyMove(const wss::Position& position, wss::Direction direction) {
    wss::Position next = position;
    switch (direction) {
    case wss::Direction::North:
        next.row -= 1;
        break;
    case wss::Direction::South:
        next.row += 1;
        break;
    case wss::Direction::East:
        next.col += 1;
        break;
    case wss::Direction::West:
        next.col -= 1;
        break;
    case wss::Direction::NorthEast:
        next.row -= 1;
        next.col += 1;
        break;
    case wss::Direction::NorthWest:
        next.row -= 1;
        next.col -= 1;
        break;
    case wss::Direction::SouthEast:
        next.row += 1;
        next.col += 1;
        break;
    case wss::Direction::SouthWest:
        next.row += 1;
        next.col -= 1;
        break;
    }
    return next;
}

std::shared_ptr<wss::Terrain> chooseTerrain(Difficulty difficulty, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);
    int value = roll(rng);

    if (difficulty == Difficulty::Easy) {
        if (value <= 50) {
            return std::make_shared<wss::PlainsTerrain>();
        }
        if (value <= 70) {
            return std::make_shared<wss::ForestTerrain>();
        }
        if (value <= 85) {
            return std::make_shared<wss::DesertTerrain>();
        }
        if (value <= 93) {
            return std::make_shared<wss::SwampTerrain>();
        }
        return std::make_shared<wss::MountainTerrain>();
    }

    if (difficulty == Difficulty::Normal) {
        if (value <= 30) {
            return std::make_shared<wss::PlainsTerrain>();
        }
        if (value <= 50) {
            return std::make_shared<wss::ForestTerrain>();
        }
        if (value <= 70) {
            return std::make_shared<wss::DesertTerrain>();
        }
        if (value <= 85) {
            return std::make_shared<wss::SwampTerrain>();
        }
        return std::make_shared<wss::MountainTerrain>();
    }

    if (value <= 18) {
        return std::make_shared<wss::PlainsTerrain>();
    }
    if (value <= 35) {
        return std::make_shared<wss::ForestTerrain>();
    }
    if (value <= 62) {
        return std::make_shared<wss::DesertTerrain>();
    }
    if (value <= 82) {
        return std::make_shared<wss::SwampTerrain>();
    }
    return std::make_shared<wss::MountainTerrain>();
}

void populateMap(wss::Map& map, Difficulty difficulty, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);

    int itemChance = 0;
    int traderChance = 0;
    if (difficulty == Difficulty::Easy) {
        itemChance = 28;
        traderChance = 10;
    } else if (difficulty == Difficulty::Normal) {
        itemChance = 20;
        traderChance = 8;
    } else {
        itemChance = 12;
        traderChance = 5;
    }

    for (int row = 0; row < map.height(); ++row) {
        for (int col = 0; col < map.width(); ++col) {
            wss::Cell& cell = map.at({row, col});
            cell.setTerrain(chooseTerrain(difficulty, rng));

            int itemRoll = roll(rng);
            if (itemRoll <= itemChance) {
                int kind = roll(rng) % 3;
                bool repeating = (roll(rng) <= 18);
                if (kind == 0) {
                    cell.addItem(std::make_shared<wss::FoodBonus>(2 + (roll(rng) % 3), repeating));
                } else if (kind == 1) {
                    cell.addItem(std::make_shared<wss::WaterBonus>(2 + (roll(rng) % 3), repeating));
                } else {
                    cell.addItem(std::make_shared<wss::GoldBonus>(1 + (roll(rng) % 4), false));
                }
            }

            int traderRoll = roll(rng);
            if (traderRoll <= traderChance) {
                int kind = roll(rng) % 3;
                if (kind == 0) {
                    cell.addTrader(std::make_shared<wss::CalmTrader>());
                } else if (kind == 1) {
                    cell.addTrader(std::make_shared<wss::HotheadedTrader>());
                } else {
                    cell.addTrader(std::make_shared<wss::RandomTrader>());
                }
            }
        }
    }
}

void collectItems(wss::Player& player, wss::Cell& cell) {
    auto& items = cell.itemsMutable();
    std::vector<std::size_t> removeIndexes;

    for (std::size_t i = 0; i < items.size(); ++i) {
        items[i]->collect(player);
        std::cout << "  Collected: " << items[i]->name() << "\n";
        if (!items[i]->repeating()) {
            removeIndexes.push_back(i);
        }
    }

    std::sort(removeIndexes.rbegin(), removeIndexes.rend());
    for (std::size_t index : removeIndexes) {
        items.erase(items.begin() + static_cast<long>(index));
    }
}

void interactTraders(wss::Player& player, const wss::Cell& cell) {
    for (const auto& trader : cell.traders()) {
        bool accepted = trader->makeOffer(player);
        std::cout << "  Trader (" << trader->name() << ") "
                  << (accepted ? "completed a trade" : "declined trade") << "\n";
    }
}

Difficulty chooseDifficulty() {
    std::cout << "Choose difficulty: 1=Easy, 2=Normal, 3=Hard: ";
    int selection = 2;
    std::cin >> selection;
    if (selection == 1) {
        return Difficulty::Easy;
    }
    if (selection == 3) {
        return Difficulty::Hard;
    }
    return Difficulty::Normal;
}

std::shared_ptr<wss::Vision> chooseVision() {
    std::cout << "Choose vision: 1=CautiousVision, 2=WideVision: ";
    int selection = 1;
    std::cin >> selection;
    if (selection == 2) {
        return std::make_shared<wss::WideVision>();
    }
    return std::make_shared<wss::CautiousVision>();
}

std::shared_ptr<wss::Brain> chooseBrain() {
    std::cout << "Choose brain: 1=CautiousBrain, 2=GreedyBrain: ";
    int selection = 1;
    std::cin >> selection;
    if (selection == 2) {
        return std::make_shared<wss::GreedyBrain>();
    }
    return std::make_shared<wss::CautiousBrain>();
}

} // namespace

int main() {
    int width = 20;
    int height = 10;

    std::cout << "Wilderness Survival System (WSS)\n";
    std::cout << "Map width: ";
    std::cin >> width;
    std::cout << "Map height: ";
    std::cin >> height;

    if (width <= 0 || height <= 0) {
        std::cout << "Invalid map size.\n";
        return 1;
    }

    Difficulty difficulty = chooseDifficulty();

    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());

    wss::Map map(width, height);
    populateMap(map, difficulty, rng);

    wss::Player player(14, 12, 12);
    player.setVision(chooseVision());
    player.setBrain(chooseBrain());
    player.setPosition({height / 2, 0});

    std::cout << "\nStarting run with " << player.vision()->name() << " and " << player.brain()->name() << "\n";

    const int maxTurns = width * height * 3;
    for (int turn = 1; turn <= maxTurns; ++turn) {
        wss::Position before = player.position();
        player.brain()->makeMove(player, map);
        wss::Position after = player.position();

        bool moved = !(before.row == after.row && before.col == after.col);
        wss::Cell& current = map.at(after);

        std::cout << "\nTurn " << turn << (moved ? " moved" : " rested")
                  << " to (row=" << after.row << ", col=" << after.col << ")";
        if (current.terrain()) {
            std::cout << " terrain=" << current.terrain()->name();
        }
        std::cout << "\n";

        collectItems(player, current);
        interactTraders(player, current);

        std::cout << "  " << player.describe() << "\n";

        if (after.col == width - 1) {
            std::cout << "\nSuccess: player reached east edge.\n";
            return 0;
        }

        if (player.currentWater() <= 0 || player.currentFood() <= 0) {
            std::cout << "\nFailure: player ran out of water or food.\n";
            return 0;
        }
    }

    std::cout << "\nRun ended: turn limit reached.\n";
    return 0;
}
