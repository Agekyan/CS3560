# CS3560 Homework 4 (WSS C++ Implementation)

## Course Information
- Course: CS3560 OOP
- Instructor: David Johannsen
- Team: Team 12

## Team 12 Members
- Sahith Kakumani
- Adam Khalil
- David Agekyan
- Octavio Munoz
- Hank Cao
- Mon Sarder

## Homework 4 Scope
HW4 is the C++ implementation phase of WSS:
- Text-mode runtime setup
- Difficulty-driven map generation
- Terrain/resource mechanics
- Items and traders
- Vision and Brain behavior
- End-to-end turn loop and simulation outcome

## Deliverables Included
- Homework folder overview: hw4/README.md
- Iteration history and progress logs: hw4/ITERATIONS.md
- File-level implementation map: hw4/FILES.md

## Build and Run
1. cmake -S . -B build
2. cmake --build build
3. ./build/wss

The executable prompts for:
- map width
- map height
- difficulty
- vision type
- brain type

Then it runs the simulation turn-by-turn.

## Validation Summary
- Clean compile with CMake
- Runtime smoke test completed with piped input
- No diagnostics in edited implementation files
