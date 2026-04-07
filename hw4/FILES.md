# HW4 File Map

## Core Runtime
- src/main.cpp
  - Program startup prompts
  - Difficulty selection
  - Map population
  - Vision/Brain selection
  - Turn loop and end conditions

## Mechanics and Behaviors
- src/Item.cpp
  - Item collection effects on player resources

- src/Trader.cpp
  - Trader offer behavior and resource exchange effects

- src/Vision.cpp
  - Vision procedures (closest/secondClosest/easiest path queries)

- src/Brain.cpp
  - Brain strategy logic for movement/rest decisions

## Header-Level Support Updates
- include/wss/Player.hpp
  - Added resource helpers for canSpend, spendX methods, and terrain-aware rest

- include/wss/Cell.hpp
  - Added mutable item vector access for one-time item removal after collection

## Build and Documentation
- CMakeLists.txt
  - Added new source files to target build list

- README.md
  - Updated with run behavior note

- HW4_README.md
  - Root-level HW4 summary

- hw4/README.md
  - Folder-level HW4 overview

- hw4/ITERATIONS.md
  - Categorized implementation iteration history and validation summary
