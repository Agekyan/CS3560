#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "Item.hpp"
#include "Terrain.hpp"
#include "Trader.hpp"

namespace wss {

class Cell {
public:
    Cell() = default;

    explicit Cell(std::shared_ptr<Terrain> terrain)
        : terrain_(std::move(terrain)) {}

    void setTerrain(std::shared_ptr<Terrain> terrain) {
        terrain_ = std::move(terrain);
    }

    std::shared_ptr<Terrain> terrain() const {
        return terrain_;
    }

    void addItem(std::shared_ptr<Item> item) {
        items_.push_back(std::move(item));
    }

    void addTrader(std::shared_ptr<Trader> trader) {
        traders_.push_back(std::move(trader));
    }

    const std::vector<std::shared_ptr<Item>>& items() const {
        return items_;
    }

    std::vector<std::shared_ptr<Item>>& itemsMutable() {
        return items_;
    }

    const std::vector<std::shared_ptr<Trader>>& traders() const {
        return traders_;
    }

private:
    std::shared_ptr<Terrain> terrain_;
    std::vector<std::shared_ptr<Item>> items_;
    std::vector<std::shared_ptr<Trader>> traders_;
};

} // namespace wss
