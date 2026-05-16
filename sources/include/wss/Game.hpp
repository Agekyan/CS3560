#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Brain.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Vision.hpp"

namespace wss {

class Game {
public:
    Game(int width, int height)
        : map_(width, height), player_(10, 10, 10) {}

    Map& map() {
        return map_;
    }

    const Map& map() const {
        return map_;
    }

    Player& player() {
        return player_;
    }

    const Player& player() const {
        return player_;
    }

    void configurePlayer(std::shared_ptr<Vision> vision, std::shared_ptr<Brain> brain) {
        player_.setVision(std::move(vision));
        player_.setBrain(std::move(brain));
    }

    std::string describe() const {
        std::ostringstream out;
        out << "WSS Game\n";
        out << "  " << map_.describe() << "\n";
        out << "  " << player_.describe() << "\n";
        out << "  Vision: " << (player_.vision() ? player_.vision()->name() : "none") << "\n";
        out << "  Brain: " << (player_.brain() ? player_.brain()->name() : "none") << "\n";
        return out.str();
    }

private:
    Map map_;
    Player player_;
};

} // namespace wss
