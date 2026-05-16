## 1. Team members and one-sentence contributions

- Sahith Kakumani — Led project planning and coordinated milestone deliveries to keep the team on schedule.
- Adam Khalil — Implemented the core game logic and resolved integration issues across subsystems.
- David Agegyan — Prepared documentation and structured the project report and grading materials.
- Octavio Munoz — Designed the textual UI layout and improved usability of the game display.
- Hank Cao — Performed systematic testing and verified feature behavior across scenarios.
- Mon Sarder — Established the repository workflow and guided code review and CI practices.

## 2. Project status (what is finished, what is not)

- Finished: core gameplay loop, map generation, resource mechanics (strength/water/food/gold), multiple terrain types, multiple trader, vision and brain strategies, and a basic textual renderer that displays the map and player state.
- Not yet finished: a polished GUI front-end and advanced visuals; automated test coverage for all subsystems; and richer procedural generation parameters for terrain and items.
- Notes on display: a curses-style or C++ GUI approach was experimented with; the professor noted (and we observed) that the simple terminal UI can behave like ASCII overprinting — it redraws characters in-place instead of emitting new lines, so some terminals show the display as characters overwritten in-place rather than as a scrolling log. This is expected with terminal cursor control sequences (we use clear + cursor-reset for the tiny UI). An alternative input-driven mode (step-by-step input-based gameplay) is available as an option for manual testing.

- Source locations: the main program and implementation files are in the `src/` directory and public headers are in `include/` (see `src/` and `include/` at the repository root).
- Sample output: a recorded example run is saved as `sources/output_example` and demonstrates map renders, per-turn movement, terrain encountered, events, trades, and the resulting player state for each turn.

## 3. How we applied Object-Oriented Design

- We used UML and design diagrams (see [docs/HW3_Design.md](docs/HW3_Design.md)) to plan class responsibilities and message flows — for example the `Player` responsibilities, `Map/Cell` composition, and the abstract `Vision`/`Brain`/`Trader` interfaces were modeled in sequence/activity diagrams.
- We applied the Strategy pattern by making `Brain` and `Vision` abstract interfaces and providing concrete strategies (`GreedyBrain`, `CautiousBrain`, `OpportunistBrain`, `WideVision`, `FocusedVision`, etc.) so behaviour can be swapped at runtime; see `include/wss/Brain.hpp` and `include/wss/Vision.hpp`.
- We used composition to build complex structures: `Map` owns a `std::vector<Cell>` and each `Cell` holds terrain/items/traders (see `include/wss/Map.hpp` and `include/wss/Cell.hpp`), which keeps object lifetimes clear and testable.
- We adhered to Single Responsibility and Dependency Inversion: `Player` encapsulates resource rules and exposes high-level operations (spend/add/recover), while it depends on abstract `Vision` and `Brain` contracts rather than concrete implementations, improving modularity and testability (`include/wss/Player.hpp`).

- Additional design notes (abstraction, encapsulation, multiplicities, hierarchy):
	- Abstraction: key concepts are deliberately exposed via minimal interfaces (e.g., `Vision`, `Brain`, `Trader`) so the game engine interacts with behaviour contracts rather than implementation details. This reduces coupling and makes it straightforward to add new strategies without changing core logic (`include/wss/Vision.hpp`, `include/wss/Brain.hpp`).
	- Encapsulation: classes hide internal state and expose controlled operations. For example `Player` enforces resource invariants through `spend` and `add*` methods, and `Map` exposes accessors but manages `Cell` storage internally (`include/wss/Player.hpp`, `include/wss/Map.hpp`).
	- Multiplicity and ownership: relationships are explicit — `Map` owns a collection of `Cell` objects (`std::vector<Cell>`), each `Cell` may reference zero-or-more `Item` or `Trader` objects via `shared_ptr` to express shared access (see `include/wss/Cell.hpp`). This clarifies lifetimes and avoids accidental dangling references.
	- Hierarchy decisions: we used class hierarchies primarily for polymorphic behavior (abstract base + concrete derived classes) rather than deep inheritance trees. Concrete leaf classes (e.g., `PlainsTerrain`, `MountainTerrain`) are `final` to signal they are not intended for further derivation and to simplify reasoning about behavior (`include/wss/Terrain.hpp`).
	- Design for testability: by depending on interfaces and injecting `Vision`/`Brain` strategies, we can write unit tests that replace complex pathfinding or AI with simple test doubles, which simplifies verification of `Player` and `Map` behaviors.

## 4. Grading information (A–L)

- **A. 5 or more — 15 points**: We implement 7 terrain types (Plains, Mountain, Desert, Swamp, Forest, Tundra, Canyon). See [include/wss/Terrain.hpp](include/wss/Terrain.hpp).

```cpp
// example from include/wss/Terrain.hpp
class PlainsTerrain final : public Terrain { public: PlainsTerrain() : Terrain("Plains", {1,1,1}) {} };
class MountainTerrain final : public Terrain { public: MountainTerrain() : Terrain("Mountain", {3,2,2}) {} };
```

- **B. 2 or more — 20 points**: Four trader types: `CalmTrader`, `HotheadedTrader`, `RandomTrader`, and `SurvivalTrader`. Each has different acceptance logic (see `src/Trader.cpp`): `CalmTrader` offers low-cost exchanges and prefers small trades; `HotheadedTrader` demands more gold but offers larger rewards; `RandomTrader` returns randomized offers; `SurvivalTrader` prioritizes survival trades when the player is low on resources. See [include/wss/Trader.hpp](include/wss/Trader.hpp) and [src/Trader.cpp](src/Trader.cpp).

```cpp
// example from src/Trader.cpp
bool CalmTrader::makeOffer(Player& player) { if (player.currentGold()>=1) { player.spendGold(1); player.addFood(3); return true; } return false; }
```

- **C. 4 or more — 10 points**: Four Vision strategies (`CautiousVision`, `WideVision`, `ScoutVision`, `FocusedVision`) are implemented to compute paths to resources; see [include/wss/Vision.hpp](include/wss/Vision.hpp).

- **D. 3 or more — 15 points**: Three Brain strategies (`GreedyBrain`, `CautiousBrain`, `OpportunistBrain`) — each implements `makeMove` with different priorities (greedy prioritizes nearest resources, cautious prefers safer/easier paths, opportunist balances multiple goals). See [include/wss/Brain.hpp](include/wss/Brain.hpp) and [src/Brain.cpp](src/Brain.cpp).

- **E. Video / Text Output — 12 points**: The program produces clear textual output (map render, status lines, and event messages). We recommend including the short demonstration video showing the runtime output and behavior to capture dynamic interactions.

- **F. Hierarchy (8 points)**: Example of composition/hierarchy — `Map` contains a vector of `Cell` objects and manages their positions; see [include/wss/Map.hpp](include/wss/Map.hpp).

```cpp
// include/wss/Map.hpp
class Map { private: std::vector<Cell> cells_; /* ... */ };
```

- **G. Inheritance (8 points)**: Concrete strategy classes inherit from abstract bases (inheritance used for polymorphic substitution). Example: `GreedyBrain` inherits from `Brain`; see [include/wss/Brain.hpp](include/wss/Brain.hpp).

```cpp
// include/wss/Brain.hpp
class Brain { public: virtual void makeMove(Player&, const Map&) const = 0; };
class GreedyBrain final : public Brain { public: void makeMove(Player&, const Map&) const override; };
```

- **H. Polymorphism (8 points)**: Runtime dispatch is used when invoking a brain's move method via a `shared_ptr` to the `Brain` interface — the code calls `player.brain()->makeMove(player, map);` enabling different behaviors at runtime; see [src/main.cpp](src/main.cpp).

```cpp
// from src/main.cpp
// call the currently-configured brain strategy
player.brain()->makeMove(player, map);
```

- **I. Encapsulation (8 points)**: `Player` protects its internal resources and provides controlled operations like `spend`, `addFood`, and `applyEfficiency` to alter state; see [include/wss/Player.hpp](include/wss/Player.hpp).

```cpp
// include/wss/Player.hpp
bool spend(const Cost& cost); // checks and deducts resources atomically
void addFood(int amount);     // safely increase food up to max
```

- **J. Abstraction (8 points)**: Abstract interfaces for `Vision`, `Brain`, and `Trader` expose behavior contracts without revealing implementations, enabling the rest of the system to rely on the contract only; see [include/wss/Vision.hpp](include/wss/Vision.hpp) and [include/wss/Trader.hpp](include/wss/Trader.hpp).

- **K. Comments and documentation (8 points)**: Key design documentation and diagrams are in [docs/HW3_Design.md](docs/HW3_Design.md). The `renderMap` helper in [src/main.cpp](src/main.cpp) includes a short explanatory comment about the terminal control sequences used for the tiny UI:

```cpp
// tiny UI: clear screen + move cursor to top
std::cout << "\x1b[2J\x1b[H";
```

- **L. Extra / impressive features (bonus)**: The project includes a simple environmental/weather mechanic (`resolveWeather` in [src/main.cpp](src/main.cpp)) and nuanced trader logic in [src/Trader.cpp](src/Trader.cpp) that together create emergent survival decisions, which is our primary bonus highlight.

## Output summary

- The sample output logs each turn: it reports when the player moves to a new square (row/col), the terrain type, any collected items or trader interactions, environmental events, and the player's updated stats (strength, water, food, gold). The map is re-rendered with a legend between turns, and the run concludes with a short summary line (for example, "Run ended: turn limit reached") explaining why execution stopped.

## Example turn breakdown 
- (available in `output_example`)

Below is a representative turn and map snapshot from a recorded run. The block is followed by a short, labeled explanation of each line.

```text
Turn 496 rested to (row=4, col=0) terrain=Plains
	Collected: Water Bonus
	Wilderness event: found a stream and rested briefly (+1 water, +1 food)
	Player(strength=2/48, water=23/23, food=32/32, gold=354)
FPDMSPDSPT
TFTMFMFPDP
SFCPDPSSFP
FPSPPSFPPF
X@XMPPPTPD
XMTFFFPDFS
DFFMTDFDMD
FFMMTMPCPP
DDSPMFPSPD
PSPDFFSFFF
Legend: @=player, X=trail, P=Plains, F=Forest, D=Desert, S=Swamp, M=Mountain, T=Tundra, C=Canyon
```

- `Turn 496 rested to (row=4, col=0) terrain=Plains` — Turn number (496); action taken this turn (`rested`); player's map coordinates `(row=4, col=0)` using zero-based row/col indexing; and the terrain type of that cell (`Plains`).
- `Collected: Water Bonus` — Items collected automatically at the location this turn (a Water Bonus in this case).
- `Wilderness event: ... (+1 water, +1 food)` — A random/environmental event and the immediate resource changes it caused; here the player gained +1 water and +1 food from a stream.
- `Player(strength=2/48, water=23/23, food=32/32, gold=354)` — Current player stats after the turn: `strength=current/max`, `water=current/max`, `food=current/max`, and `gold` (unbounded integer value).
- The following 10 lines are the ASCII map: each character represents the terrain at that grid cell (rows printed top-to-bottom, columns left-to-right). Special characters:
	- `@` = player position (appears at the row/col given above)
	- `X` = trail / previously visited tile
	- `P, F, D, S, M, T, C` = terrain symbols per the legend
- `Legend: ...` — A mapping from map characters to their meanings so a reader can decode the ASCII map.

This compact per-turn output makes it straightforward to follow exactly where the player moved, what they found, what decisions/events occurred, and the resulting resource state.
