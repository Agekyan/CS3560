#include "wss/Vision.hpp"

#include <algorithm>
#include <optional>
#include <vector>

#include "wss/Item.hpp"

namespace wss {
namespace {

using FilterFn = bool (*)(const Cell&);

bool hasFood(const Cell& cell) {
    for (const auto& item : cell.items()) {
        if (dynamic_cast<FoodBonus*>(item.get()) != nullptr) {
            return true;
        }
    }
    return false;
}

bool hasWater(const Cell& cell) {
    for (const auto& item : cell.items()) {
        if (dynamic_cast<WaterBonus*>(item.get()) != nullptr) {
            return true;
        }
    }
    return false;
}

bool hasGold(const Cell& cell) {
    for (const auto& item : cell.items()) {
        if (dynamic_cast<GoldBonus*>(item.get()) != nullptr) {
            return true;
        }
    }
    return false;
}

bool hasTrader(const Cell& cell) {
    return !cell.traders().empty();
}

struct Candidate {
    Position position;
    Direction direction;
    Cost cost;
    int distance;
};

std::optional<Candidate> chooseByRank(std::vector<Candidate> candidates, std::size_t index) {
    if (candidates.empty() || index >= candidates.size()) {
        return std::nullopt;
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate& left, const Candidate& right) {
        if (left.distance != right.distance) {
            return left.distance < right.distance;
        }
        if (left.cost.movement != right.cost.movement) {
            return left.cost.movement < right.cost.movement;
        }
        return left.position.col > right.position.col;
    });

    return candidates[index];
}

Path toPath(const std::optional<Candidate>& candidate) {
    if (!candidate.has_value()) {
        return {};
    }
    return Path({candidate->direction}, candidate->cost);
}

std::vector<Candidate> visibleCandidates(
    const Map& map,
    const Position& origin,
    const std::vector<std::pair<Direction, Position>>& offsets,
    FilterFn filter) {
    std::vector<Candidate> candidates;

    for (const auto& [direction, offset] : offsets) {
        Position next{origin.row + offset.row, origin.col + offset.col};
        if (!map.inBounds(next)) {
            continue;
        }
        const Cell& cell = map.at(next);
        if (!cell.terrain()) {
            continue;
        }
        if (!filter(cell)) {
            continue;
        }

        candidates.push_back(Candidate{
            next,
            direction,
            cell.terrain()->enterCost(),
            std::abs(next.row - origin.row) + std::abs(next.col - origin.col)});
    }

    return candidates;
}

std::vector<Candidate> allVisible(
    const Map& map,
    const Position& origin,
    const std::vector<std::pair<Direction, Position>>& offsets) {
    std::vector<Candidate> candidates;

    for (const auto& [direction, offset] : offsets) {
        Position next{origin.row + offset.row, origin.col + offset.col};
        if (!map.inBounds(next)) {
            continue;
        }
        const Cell& cell = map.at(next);
        if (!cell.terrain()) {
            continue;
        }

        candidates.push_back(Candidate{
            next,
            direction,
            cell.terrain()->enterCost(),
            std::abs(next.row - origin.row) + std::abs(next.col - origin.col)});
    }

    return candidates;
}

Path chooseClosest(
    const Map& map,
    const Position& origin,
    const std::vector<std::pair<Direction, Position>>& offsets,
    FilterFn filter,
    std::size_t rank) {
    return toPath(chooseByRank(visibleCandidates(map, origin, offsets, filter), rank));
}

Path chooseEasiest(
    const Map& map,
    const Position& origin,
    const std::vector<std::pair<Direction, Position>>& offsets) {
    auto candidates = allVisible(map, origin, offsets);
    if (candidates.empty()) {
        return {};
    }

    auto best = std::min_element(candidates.begin(), candidates.end(), [](const Candidate& left, const Candidate& right) {
        if (left.cost.movement != right.cost.movement) {
            return left.cost.movement < right.cost.movement;
        }
        return left.position.col > right.position.col;
    });

    return Path({best->direction}, best->cost);
}

std::vector<std::pair<Direction, Position>> cautiousOffsets() {
    return {
        {Direction::North, {-1, 0}},
        {Direction::South, {1, 0}},
        {Direction::East, {0, 1}},
    };
}

std::vector<std::pair<Direction, Position>> wideOffsets() {
    return {
        {Direction::North, {-1, 0}},
        {Direction::South, {1, 0}},
        {Direction::East, {0, 1}},
        {Direction::West, {0, -1}},
        {Direction::NorthEast, {-1, 1}},
        {Direction::NorthWest, {-1, -1}},
        {Direction::SouthEast, {1, 1}},
        {Direction::SouthWest, {1, -1}},
    };
}

std::vector<std::pair<Direction, Position>> scoutOffsets() {
    return {
        {Direction::North, {-1, 0}},
        {Direction::South, {1, 0}},
        {Direction::East, {0, 1}},
        {Direction::West, {0, -1}},
        {Direction::NorthEast, {-1, 1}},
        {Direction::NorthWest, {-1, -1}},
        {Direction::SouthEast, {1, 1}},
        {Direction::SouthWest, {1, -1}},
        {Direction::North, {-2, 0}},
        {Direction::South, {2, 0}},
        {Direction::East, {0, 2}},
        {Direction::West, {0, -2}},
    };
}

std::vector<std::pair<Direction, Position>> focusedOffsets() {
    // tunnel vision: forward and diagonals, a couple steps out
    return {
        {Direction::East, {0, 1}},
        {Direction::NorthEast, {-1, 1}},
        {Direction::SouthEast, {1, 1}},
        {Direction::East, {0, 2}},
        {Direction::NorthEast, {-2, 2}},
        {Direction::SouthEast, {2, 2}},
    };
}

} // namespace

Path CautiousVision::closestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasFood, 0);
}

Path CautiousVision::closestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasWater, 0);
}

Path CautiousVision::closestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasGold, 0);
}

Path CautiousVision::closestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasTrader, 0);
}

Path CautiousVision::easiestPath(const Map& map, const Position& origin) const {
    return chooseEasiest(map, origin, cautiousOffsets());
}

Path CautiousVision::secondClosestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasFood, 1);
}

Path CautiousVision::secondClosestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasWater, 1);
}

Path CautiousVision::secondClosestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasGold, 1);
}

Path CautiousVision::secondClosestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, cautiousOffsets(), hasTrader, 1);
}

Path WideVision::closestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasFood, 0);
}

Path WideVision::closestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasWater, 0);
}

Path WideVision::closestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasGold, 0);
}

Path WideVision::closestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasTrader, 0);
}

Path WideVision::easiestPath(const Map& map, const Position& origin) const {
    return chooseEasiest(map, origin, wideOffsets());
}

Path WideVision::secondClosestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasFood, 1);
}

Path WideVision::secondClosestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasWater, 1);
}

Path WideVision::secondClosestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasGold, 1);
}

Path WideVision::secondClosestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, wideOffsets(), hasTrader, 1);
}

Path ScoutVision::closestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasFood, 0);
}

Path ScoutVision::closestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasWater, 0);
}

Path ScoutVision::closestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasGold, 0);
}

Path ScoutVision::closestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasTrader, 0);
}

Path ScoutVision::easiestPath(const Map& map, const Position& origin) const {
    return chooseEasiest(map, origin, scoutOffsets());
}

Path ScoutVision::secondClosestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasFood, 1);
}

Path ScoutVision::secondClosestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasWater, 1);
}

Path ScoutVision::secondClosestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasGold, 1);
}

Path ScoutVision::secondClosestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, scoutOffsets(), hasTrader, 1);
}

Path FocusedVision::closestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasFood, 0);
}

Path FocusedVision::closestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasWater, 0);
}

Path FocusedVision::closestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasGold, 0);
}

Path FocusedVision::closestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasTrader, 0);
}

Path FocusedVision::easiestPath(const Map& map, const Position& origin) const {
    return chooseEasiest(map, origin, focusedOffsets());
}

Path FocusedVision::secondClosestFood(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasFood, 1);
}

Path FocusedVision::secondClosestWater(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasWater, 1);
}

Path FocusedVision::secondClosestGold(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasGold, 1);
}

Path FocusedVision::secondClosestTrader(const Map& map, const Position& origin) const {
    return chooseClosest(map, origin, focusedOffsets(), hasTrader, 1);
}

} // namespace wss
