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

char terrainSymbol(const std::shared_ptr<wss::Terrain>& terrain) {
    if (!terrain) {
        return '?';
    }
    const std::string& name = terrain->name();
    if (name == "Plains") return 'P';
    if (name == "Forest") return 'F';
    if (name == "Desert") return 'D';
    if (name == "Swamp") return 'S';
    if (name == "Mountain") return 'M';
    if (name == "Tundra") return 'T';
    if (name == "Canyon") return 'C';
    return '?';
}

void renderMap(const wss::Map& map, const wss::Player& player, const std::vector<std::vector<bool>>& visited) {
    std::cout << "\x1b[2J\x1b[H"; // tiny UI: clear screen + move cursor to top
    for (int row = 0; row < map.height(); ++row) {
        for (int col = 0; col < map.width(); ++col) {
            wss::Position pos{row, col};
            if (pos.row == player.position().row && pos.col == player.position().col) {
                std::cout << '@';
                continue;
            }
            if (visited[row][col]) {
                std::cout << 'X';
                continue;
            }
            std::cout << terrainSymbol(map.at(pos).terrain());
        }
        std::cout << '\n';
    }
    std::cout << "Legend: @=player, X=trail, P=Plains, F=Forest, D=Desert, S=Swamp, M=Mountain, T=Tundra, C=Canyon\n";
}

void resolveWeather(wss::Player& player, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);
    int weatherRoll = roll(rng);
    if (weatherRoll <= 12) {
        player.applyEfficiency(1.2, 2);
        std::cout << "  Weather: harsh winds, costs spike for 2 turns\n";
    } else if (weatherRoll <= 24) {
        player.applyEfficiency(0.8, 3);
        std::cout << "  Weather: clear skies, costs drop for 3 turns\n";
    }
}

std::shared_ptr<wss::Terrain> chooseTerrain(Difficulty difficulty, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);
    int value = roll(rng);

    if (difficulty == Difficulty::Easy) {
        if (value <= 40) {
            return std::make_shared<wss::PlainsTerrain>();
        }
        if (value <= 58) {
            return std::make_shared<wss::ForestTerrain>();
        }
        if (value <= 72) {
            return std::make_shared<wss::DesertTerrain>();
        }
        if (value <= 82) {
            return std::make_shared<wss::SwampTerrain>();
        }
        if (value <= 90) {
            return std::make_shared<wss::MountainTerrain>();
        }
        if (value <= 95) {
            return std::make_shared<wss::TundraTerrain>();
        }
        return std::make_shared<wss::CanyonTerrain>();
    }

    if (difficulty == Difficulty::Normal) {
        if (value <= 26) {
            return std::make_shared<wss::PlainsTerrain>();
        }
        if (value <= 44) {
            return std::make_shared<wss::ForestTerrain>();
        }
        if (value <= 62) {
            return std::make_shared<wss::DesertTerrain>();
        }
        if (value <= 78) {
            return std::make_shared<wss::SwampTerrain>();
        }
        if (value <= 90) {
            return std::make_shared<wss::MountainTerrain>();
        }
        if (value <= 95) {
            return std::make_shared<wss::TundraTerrain>();
        }
        return std::make_shared<wss::CanyonTerrain>();
    }

    if (value <= 16) {
        return std::make_shared<wss::PlainsTerrain>();
    }
    if (value <= 30) {
        return std::make_shared<wss::ForestTerrain>();
    }
    if (value <= 55) {
        return std::make_shared<wss::DesertTerrain>();
    }
    if (value <= 74) {
        return std::make_shared<wss::SwampTerrain>();
    }
    if (value <= 88) {
        return std::make_shared<wss::MountainTerrain>();
    }
    if (value <= 95) {
        return std::make_shared<wss::TundraTerrain>();
    }
    return std::make_shared<wss::CanyonTerrain>();
}

void populateMap(wss::Map& map, Difficulty difficulty, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);

    int itemChance = 0;
    int traderChance = 0;
    if (difficulty == Difficulty::Easy) {
        itemChance = 36;
        traderChance = 12;
    } else if (difficulty == Difficulty::Normal) {
        itemChance = 28;
        traderChance = 10;
    } else {
        itemChance = 18;
        traderChance = 7;
    }

    for (int row = 0; row < map.height(); ++row) {
        for (int col = 0; col < map.width(); ++col) {
            wss::Cell& cell = map.at({row, col});
            cell.setTerrain(chooseTerrain(difficulty, rng));

            int itemRoll = roll(rng);
            int itemsToSpawn = 0;
            if (itemRoll <= itemChance) {
                itemsToSpawn = 1;
            }
            if (roll(rng) <= itemChance / 2) {
                ++itemsToSpawn;
            }
            if (difficulty == Difficulty::Easy && roll(rng) <= 12) {
                ++itemsToSpawn;
            }
            itemsToSpawn = std::min(itemsToSpawn, 3);

            for (int index = 0; index < itemsToSpawn; ++index) {
                int kind = roll(rng) % 5;
                bool repeating = (roll(rng) <= 22);
                if (kind <= 1) {
                    cell.addItem(std::make_shared<wss::FoodBonus>(2 + (roll(rng) % 4), repeating));
                } else if (kind <= 3) {
                    cell.addItem(std::make_shared<wss::WaterBonus>(2 + (roll(rng) % 4), repeating));
                } else {
                    cell.addItem(std::make_shared<wss::GoldBonus>(2 + (roll(rng) % 5), false));
                }
            }

            int traderRoll = roll(rng);
            int tradersToSpawn = 0;
            if (traderRoll <= traderChance) {
                tradersToSpawn = 1;
            }
            if (roll(rng) <= traderChance / 2) {
                ++tradersToSpawn;
            }
            if (difficulty == Difficulty::Easy && roll(rng) <= 8) {
                ++tradersToSpawn;
            }
            tradersToSpawn = std::min(tradersToSpawn, 2);

            for (int index = 0; index < tradersToSpawn; ++index) {
                int kind = roll(rng) % 4;
                if (kind == 0) {
                    cell.addTrader(std::make_shared<wss::CalmTrader>());
                } else if (kind == 1) {
                    cell.addTrader(std::make_shared<wss::HotheadedTrader>());
                } else if (kind == 2) {
                    cell.addTrader(std::make_shared<wss::RandomTrader>());
                } else {
                    cell.addTrader(std::make_shared<wss::SurvivalTrader>());
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
        int beforeGold = player.currentGold();
        int beforeFood = player.currentFood();
        int beforeWater = player.currentWater();
        bool accepted = trader->makeOffer(player);
        std::cout << "  Trader (" << trader->name() << ") ";
        if (!accepted) {
            std::cout << "declined trade\n";
            continue;
        }

        std::cout << "trade resolved";
        std::cout << " [food " << beforeFood << "->" << player.currentFood()
                  << ", water " << beforeWater << "->" << player.currentWater()
                  << ", gold " << beforeGold << "->" << player.currentGold() << "]\n";
    }
}

void resolveWildernessEvent(wss::Player& player, std::mt19937& rng) {
    std::uniform_int_distribution<int> roll(1, 100);
    int eventRoll = roll(rng);

    std::cout << "  Wilderness event: ";
    if (eventRoll <= 14) {
        int f = 2 + (roll(rng) % 4); // 2-5
        int w = 2 + (roll(rng) % 4);
        int g = 1 + (roll(rng) % 3);
        player.addFood(f);
        player.addWater(w);
        player.addGold(g);
        std::cout << "found an abandoned camp and recovered supplies (+" << f << " food, +" << w << " water, +" << g << " gold)";
    } else if (eventRoll <= 28) {
        int g = 1 + (roll(rng) % 4);
        player.addGold(g);
        std::cout << "dug up a small gold cache (+" << g << " gold)";
    } else if (eventRoll <= 46) {
        int w = 2 + (roll(rng) % 4);
        player.addWater(w);
        std::cout << "a rain shower topped off water (+" << w << ")";
    } else if (eventRoll <= 64) {
        int f = 2 + (roll(rng) % 4);
        player.addFood(f);
        std::cout << "foraging paid off with berries (+" << f << " food)";
    } else if (eventRoll <= 76) {
        player.addWater(1);
        player.addFood(1);
        std::cout << "found a stream and rested briefly (+1 water, +1 food)";
    } else if (eventRoll <= 86) {
        player.applyEfficiency(0.6, 6); // costs reduced to ~60% for 6 turns
        std::cout << "found lightweight gear: movement and resource costs reduced for 6 turns";
    } else if (eventRoll <= 96) {
        if (player.spendFood(1) || player.spendWater(1)) {
            std::cout << "lost a few supplies crossing rough ground";
        } else {
            std::cout << "nothing happened";
        }
    } else {
        int which = roll(rng) % 3;
        if (which == 0) {
            int add = 2 + (roll(rng) % 3); // +2..+4 slots
            player.increaseMaxFood(add);
            player.addFood(add);
            std::cout << "found a fertile valley: max food increased by " << add << " and recovered " << add << " food";
        } else if (which == 1) {
            int add = 2 + (roll(rng) % 3);
            player.increaseMaxWater(add);
            player.addWater(add);
            std::cout << "found a life-giving spring: max water increased by " << add << " and recovered " << add << " water";
        } else {
            int add = 3 + (roll(rng) % 4); // give a bit more strength
            player.increaseMaxStrength(add);
            player.addFood(1);
            player.addWater(1);
            std::cout << "found an inspiring shrine: max strength increased by " << add << " and recovered small supplies";
        }
    }
    std::cout << "\n";
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
    std::cout << "Choose vision: 1=Cautious, 2=Wide, 3=Scout, 4=Focused: ";
    int selection = 1;
    std::cin >> selection;
    if (selection == 2) {
        return std::make_shared<wss::WideVision>();
    }
    if (selection == 3) {
        return std::make_shared<wss::ScoutVision>();
    }
    if (selection == 4) {
        return std::make_shared<wss::FocusedVision>();
    }
    return std::make_shared<wss::CautiousVision>();
}

std::shared_ptr<wss::Brain> chooseBrain() {
    std::cout << "Choose brain: 1=Cautious, 2=Greedy, 3=Opportunist: ";
    int selection = 1;
    std::cin >> selection;
    if (selection == 2) {
        return std::make_shared<wss::GreedyBrain>();
    }
    if (selection == 3) {
        return std::make_shared<wss::OpportunistBrain>();
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

    wss::Player player(16, 14, 14);
    player.setVision(chooseVision());
    player.setBrain(chooseBrain());
    player.setPosition({height / 2, 0});

    std::cout << "\nStarting run with " << player.vision()->name() << " and " << player.brain()->name() << "\n";

    const int maxTurns = width * height * 5;
    bool reachedEastEdge = false;
    std::vector<std::vector<bool>> visited(static_cast<std::size_t>(height), std::vector<bool>(static_cast<std::size_t>(width), false));
    for (int turn = 1; turn <= maxTurns; ++turn) {
        wss::Position before = player.position();
        visited[before.row][before.col] = true;
        player.brain()->makeMove(player, map);
        wss::Position after = player.position();
        visited[after.row][after.col] = true;

        bool moved = !(before.row == after.row && before.col == after.col);
        wss::Cell& current = map.at(after);

        // quick UI pass, not fancy but it shows the trail and terrain at a glance
        renderMap(map, player, visited);

        std::cout << "\nTurn " << turn << (moved ? " moved" : " rested")
                  << " to (row=" << after.row << ", col=" << after.col << ")";
        if (current.terrain()) {
            std::cout << " terrain=" << current.terrain()->name();
        }
        std::cout << "\n";

        collectItems(player, current);
        interactTraders(player, current);
        resolveWeather(player, rng);
        resolveWildernessEvent(player, rng);
        player.tickTurn();

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

    if (reachedEastEdge) {
        std::cout << "\nRun ended: checkpoint reached and turn limit expired after extended exploration.\n";
        return 0;
    }

    std::cout << "\nRun ended: turn limit reached.\n";
    return 0;
}
