# CS3560

WSS C++ project ~~scaffold~~ for CS3560 with completed HW3 documentation and a runnable HW4 implementation.

## Build

```bash
cmake -S . -B build
cmake --build build
./build/wss
```

The program then prompts for map width, map height, difficulty, vision type, and brain type.
It runs turn-by-turn until the player reaches the east edge, runs out of resources, or reaches a turn cap.

## Layout

- `include/wss/` contains the public class headers.
- `src/main.cpp` plus `src/Brain.cpp`, `src/Vision.cpp`, `src/Item.cpp`, and `src/Trader.cpp` implement HW4 runtime behavior.
- `docs/HW3_Design.md` contains the text version of the HW3 design deliverable.

## Homework Documentation

- `HW3_README.md` main HW3 summary
- `HW4_README.md` main HW4 summary
- `hw3/` organized HW3 folder docs (`README.md`, `ITERATIONS.md`, `FILES.md`)
- `hw4/` organized HW4 folder docs (`README.md`, `ITERATIONS.md`, `FILES.md`)
