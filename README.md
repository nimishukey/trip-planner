# Trip Planner
A C program that finds the optimal walking and ferry route between city landmarks based on user preferences and real-time schedules.

This project is a real-time trip planner that takes into account walking paths, ferry schedules, and user departure times to suggest the most efficient route through a city.

---

## Features

- Parses input for city landmarks, walking paths, and ferry routes.
- Accepts user trip queries with time of departure.
- Calculates and prints the optimal path, considering walking durations and ferry schedules.
- Supports:
  - Stage 1: Direct route identification.
  - Stage 2: Walking-only paths.
  - Stage 3: Combined walking and ferry travel with time constraints.
  - Stage 4: Optimal route with shortest total travel time (including waiting).

---

## How to Use

1. **Compile the program**:

   ```bash
   gcc -o tripPlan tripPlan.c

2. **Run the program**:

   ```bash
   ./tripPlan

3. **Follow prompts to input**
  - Landmarks
  - Walking links
  - Ferry schedules
  - User queries

4. Enter **done** when you're finished to exit.

---

## Example

```` ``` ````text
Number of walking links: 2
TheRocks
CircularQuay
6
ManlyWharf
ManlyBeach
8

From: TheRocks
To: CircularQuay
Departure time: 0000

Walk 8 minute(s):
  0000 TheRocks
  0008 CircularQuay

From: done
Happy travels!

---

## Project Structure

  ```bash
  trip-planner/
  ├── tripPlan.c      # Main C source file
  └── README.md      


