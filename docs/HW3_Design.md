# CS3560 HW3 Design Notes for WSS

This file is a text version of the HW3 design deliverable. It is meant to be turned into UML diagrams, but it also stands on its own as a design summary for the C++ implementation.

## 1. System Overview

WSS (Wilderness Survival System) is a map-based survival game. A Player starts on the west edge of a Map and tries to reach the east edge while managing movement, water, food, and gold. The Map contains terrain, items, and traders.

## 2. Core Classes

### Game
Responsibilities:
- Own the Map and the Player.
- Configure the chosen Vision and Brain.
- Start the simulation.
- Print a summary of the current game state.

Collaborators:
- Map
- Player
- Vision
- Brain

### Map
Responsibilities:
- Store the rectangular grid of cells.
- Check whether a Position is in bounds.
- Provide access to a specific Cell.
- Support map generation by difficulty.

Collaborators:
- Cell
- Terrain
- Item
- Trader

### Cell
Responsibilities:
- Hold one terrain type.
- Hold zero or more items.
- Hold zero or more traders.

Collaborators:
- Terrain
- Item
- Trader

### Terrain
Responsibilities:
- Represent the movement, water, and food cost of entering a square.
- Decide whether the Player can enter the square.

Subclasses:
- PlainsTerrain
- MountainTerrain
- DesertTerrain
- SwampTerrain
- ForestTerrain

### Item
Responsibilities:
- Represent a collectible bonus in a map square.
- Apply its effect to the Player.
- Indicate whether it is repeating.

Subclasses:
- FoodBonus
- WaterBonus
- GoldBonus

### Trader
Responsibilities:
- Offer trades to the Player.
- React to counter-offers.
- Decide whether the negotiation continues, succeeds, or ends.

Subclasses:
- CalmTrader
- HotheadedTrader
- RandomTrader

### Player
Responsibilities:
- Track current and maximum strength, water, food, and gold.
- Track the current Position.
- Spend resources when moving.
- Recover movement when resting.
- Receive bonuses from items and trades.
- Hold a Vision and a Brain.

Collaborators:
- Vision
- Brain
- Terrain
- Item
- Trader
- Map

### Vision
Responsibilities:
- Inspect neighboring map squares.
- Return a Path to the nearest relevant square.
- Provide alternative paths when asked for second-best choices.

Subclasses:
- CautiousVision
- WideVision

### Brain
Responsibilities:
- Decide what the Player should do next.
- Query Vision.
- Move the Player or choose to rest.
- Trigger item collection and trader interaction after movement.

Subclasses:
- GreedyBrain
- CautiousBrain

### Path
Responsibilities:
- Store an ordered list of moves.
- Store the movement, water, and food cost of that route.
- Summarize the route in readable form.

### Supporting Types
- Position: row and column location.
- Cost: movement, water, and food cost bundle.
- Direction: compass direction of a move.

## 3. Class Relationships

- Game owns one Map and one Player.
- Map contains many Cells.
- Each Cell has exactly one Terrain when initialized.
- A Cell may contain many Items.
- A Cell may contain many Traders.
- Player uses one Vision and one Brain.
- Vision and Brain are designed for inheritance and polymorphism.
- Terrain, Item, and Trader are also designed as polymorphic hierarchies.

## 4. Difficulty Strategy for Map Generation

### Easy
- Mostly PlainsTerrain.
- Occasional ForestTerrain.
- Few MountainTerrain or SwampTerrain squares.
- More food and water bonuses.

### Normal
- Mixed terrain distribution.
- More MountainTerrain and SwampTerrain than Easy.
- Moderate item placement.

### Hard
- Frequent MountainTerrain, DesertTerrain, and SwampTerrain.
- Fewer bonuses.
- Traders appear less often.

The exact generation strategy should be documented in the final implementation and kept consistent with the selected difficulty.

## 5. Text Use Cases

### Use Case 1: Start Game
Actors:
- User

Main Flow:
- The user enters the map size and difficulty.
- The game creates the map.
- The game creates the player.
- The user selects a Vision and Brain.
- The game begins the simulation.

### Use Case 2: Move Player
Actors:
- Player
- Brain
- Vision

Main Flow:
- Brain examines the player status.
- Brain asks Vision for nearby options.
- Brain selects a move or chooses to rest.
- The Player moves or stays still.
- Terrain costs are applied.

### Use Case 3: Collect Items
Actors:
- Player
- Item

Main Flow:
- The Player enters a Cell.
- The game checks for items in that Cell.
- The Player collects each available item.
- One-time items are removed.
- Repeating items remain for future turns.

### Use Case 4: Trade With Trader
Actors:
- Player
- Trader

Main Flow:
- The Player proposes a trade.
- The Trader accepts, rejects, or counters.
- The Player may accept or counter.
- The negotiation continues until one side ends it.

## 6. Text Sequence Models

### Sequence: Player Move
- Brain calls Vision to inspect nearby squares.
- Brain chooses a Path.
- Brain tells Player to move one step.
- Map returns the destination Cell.
- Player spends the required resources.
- Game triggers item collection and trader interaction in the new Cell.

### Sequence: Trader Negotiation
- Player makes an offer.
- Trader evaluates the offer.
- Trader either accepts, rejects, or counteroffers.
- If counteroffered, Player responds.
- The loop ends when one side accepts or rejects.

## 7. Text Activity Diagrams

### Activity: Brain.makeMove()
- Read current player resources.
- Query Vision for nearby paths.
- Compare options.
- Decide whether to move or rest.
- If moving, tell Player to step into the chosen square.
- If resting, recover movement and reduce water/food at half rate.
- After movement, process any items and traders in the new Cell.

### Activity: Trader Negotiation
- Receive the Player offer.
- Evaluate whether the trade is profitable enough.
- Accept, reject, or generate a counteroffer.
- Repeat until a terminal decision is reached.

## 8. Text State Diagrams

### Player Turn State Diagram
States:
- Ready
- Evaluating
- Moving
- Resting
- ResolvingCell
- EndTurn

Transitions:
- Ready -> Evaluating when the turn begins.
- Evaluating -> Moving when Brain chooses a move.
- Evaluating -> Resting when Brain chooses to stay still.
- Moving -> ResolvingCell after the move succeeds.
- Resting -> ResolvingCell after recovery is applied.
- ResolvingCell -> EndTurn after items and trades are processed.

### Trader State Diagram
States:
- Idle
- ConsideringOffer
- CounterOffering
- Accepting
- Rejecting
- Exhausted

Transitions:
- Idle -> ConsideringOffer when the Player makes an offer.
- ConsideringOffer -> Accepting when the offer is good enough.
- ConsideringOffer -> Rejecting when the offer is unacceptable.
- ConsideringOffer -> CounterOffering when the Trader wants another deal.
- CounterOffering -> ConsideringOffer when the Player responds.
- Repeated counteroffers can lead to Exhausted for the more aggressive trader subtype.

### Brain State Diagram
States:
- Observe
- Decide
- Move
- Rest
- Trade
- EndTurn

Transitions:
- Observe -> Decide after querying the Player and Vision.
- Decide -> Move when movement is the best option.
- Decide -> Rest when resources are low.
- Move -> Trade when the new square contains a trader.
- Trade -> EndTurn after negotiation finishes.
- Rest -> EndTurn after recovery.

## 9. Suggested Subclasses

To satisfy the project requirement for inheritance and polymorphism, the implementation should include at least:
- 5 Terrain subclasses
- 3 Item subclasses
- 3 Trader subclasses
- 2 Vision subclasses
- 2 Brain subclasses

## 10. Notes for HW4

This scaffold is intentionally minimal. For the implementation phase, the next steps are:
- Add real map generation based on difficulty.
- Implement path search logic inside Vision subclasses.
- Implement decision logic inside Brain subclasses.
- Implement item collection and trader negotiation.
- Add a simple command-line interface for setup and output.
