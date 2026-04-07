#include "wss/Brain.hpp"

#include <vector>

namespace wss {
namespace {

Position moved(const Position& position, Direction direction) {
    Position next = position;
    switch (direction) {
    case Direction::North:
        next.row -= 1;
        break;
    case Direction::South:
        next.row += 1;
        break;
    case Direction::East:
        next.col += 1;
        break;
    case Direction::West:
        next.col -= 1;
        break;
    case Direction::NorthEast:
        next.row -= 1;
        next.col += 1;
        break;
    case Direction::NorthWest:
        next.row -= 1;
        next.col -= 1;
        break;
    case Direction::SouthEast:
        next.row += 1;
        next.col += 1;
        break;
    case Direction::SouthWest:
        next.row += 1;
        next.col -= 1;
        break;
    }
    return next;
}

bool tryMove(Player& player, const Map& map, const Path& path) {
    if (path.empty()) {
        return false;
    }

    Position current = player.position();
    Position next = moved(current, path.moves().front());
    if (!map.inBounds(next)) {
        return false;
    }

    const Cell& destination = map.at(next);
    if (!destination.terrain()) {
        return false;
    }

    const Cost& cost = destination.terrain()->enterCost();
    if (!player.spend(cost)) {
        return false;
    }

    player.setPosition(next);
    return true;
}

} // namespace

void GreedyBrain::makeMove(Player& player, const Map& map) const {
    if (!player.vision()) {
        return;
    }

    std::vector<Path> options;
    options.push_back(player.vision()->closestGold(map, player.position()));
    options.push_back(player.vision()->closestFood(map, player.position()));
    options.push_back(player.vision()->closestWater(map, player.position()));
    options.push_back(player.vision()->easiestPath(map, player.position()));

    for (const auto& path : options) {
        if (tryMove(player, map, path)) {
            return;
        }
    }

    const Cell& current = map.at(player.position());
    if (current.terrain()) {
        player.restInTerrain(current.terrain()->enterCost());
    }
}

void CautiousBrain::makeMove(Player& player, const Map& map) const {
    if (!player.vision()) {
        return;
    }

    if (player.currentWater() <= 2 || player.currentFood() <= 2) {
        const Cell& current = map.at(player.position());
        if (current.terrain()) {
            player.restInTerrain(current.terrain()->enterCost());
            return;
        }
    }

    std::vector<Path> options;
    options.push_back(player.vision()->closestWater(map, player.position()));
    options.push_back(player.vision()->closestFood(map, player.position()));
    options.push_back(player.vision()->closestTrader(map, player.position()));
    options.push_back(player.vision()->easiestPath(map, player.position()));

    for (const auto& path : options) {
        if (tryMove(player, map, path)) {
            return;
        }
    }

    const Cell& current = map.at(player.position());
    if (current.terrain()) {
        player.restInTerrain(current.terrain()->enterCost());
    }
}

} // namespace wss
