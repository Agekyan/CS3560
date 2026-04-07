# HW4 Iterations and Progress Notes

## Iteration 1: Item and Trader Behavior
### Progress Log
- Item/trader behavior is now implemented.

### Implementation Category
- Core gameplay interactions

### Key Result
- Item collection and trader actions now modify player resources.

## Iteration 2: Vision and Brain Wiring
### Progress Log
- Next I’m wiring Vision and Brain logic so turns can actually choose movement based on nearby terrain/items/traders.

### Implementation Category
- Decision and strategy system

### Key Result
- Brain uses Vision outputs to select movement or rest behavior.

## Iteration 3: Full Runtime Integration
### Progress Log
- I’m implementing the full runtime now: map/difficulty prompts, terrain and item/trader population, vision+brain selection, and an automated turn loop from west to east.

### Implementation Category
- End-to-end runtime flow

### Key Result
- Executable now runs full setup and simulation loop.

## Iteration 4: Build and Error-Fix Pass
### Progress Log
- I’ve wired the core HW4 code; now I’m compiling to catch any integration errors and will immediately fix them.

### Implementation Category
- Build integration and verification

### Key Result
- Integrated implementation compiles and links successfully.

## Iteration 5: Completed HW4 Scope
### Progress Log
- Finished HW4 implementation in C++ as a runnable text game, while keeping scope on the assignment requirements.

### Implementation Category
- Final deliverable completion

### Key Result
- Homework 4 is runnable, validated, and documented.

## What Is Now Implemented

### Runtime Game Flow
- Prompts for map width and height
- Prompts for difficulty
- Prompts for Vision and Brain type
- Runs turn-by-turn simulation until success or failure
- Main loop is in src/main.cpp

### Map Generation by Difficulty
- Terrain is generated per-cell using difficulty-based distributions
- Items and traders are populated with difficulty-scaled frequency
- Implemented in src/main.cpp

### Terrain Costs and Rest Behavior
- Entering a square deducts movement, water, and food
- Resting restores strength and consumes half-rate food/water for current terrain
- Player resource operations extended in include/wss/Player.hpp

### Items
- Food, water, and gold bonuses now apply real effects to the player
- One-time items are removed after collection; repeating bonuses stay
- Item behavior in src/Item.cpp
- Item removal support in include/wss/Cell.hpp

### Traders
- Trader types now execute concrete trade logic against player resources
- Implemented in src/Trader.cpp

### Vision Procedures
- Implemented closestFood, closestWater, closestGold, closestTrader
- Implemented easiestPath and secondClosest variants
- Includes cautious and wide visibility scope behavior
- Implemented in src/Vision.cpp

### Brain Behavior
- Implemented CautiousBrain and GreedyBrain makeMove strategies
- Brains use Vision output, move if feasible, otherwise rest
- Implemented in src/Brain.cpp

### Build Wiring and Docs
- Added new sources to build in CMakeLists.txt
- Updated run notes in README.md

## Validation Completed
- Clean compile with CMake
- Smoke-tested executable run with piped input
- No diagnostics in edited files via problems check

## Optional Next Tuning (Not Required)
- Tune balance only (terrain probabilities, bonus sizes, trader generosity) so it plays less punishingly while still matching the no-unnecessary-extras expectation.
