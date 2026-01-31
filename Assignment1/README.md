***

```markdown
# The Matrix — Interactive Pathfinding Tasks

This project contains two interactive AI pathfinding challenges inspired by *The Matrix* universe: **Task A (A\_star)** and **Task B (Backtracking\_algorithm)**.  
Both tasks involve Neo trying to reach the Keymaker while avoiding enemies, using commands exchanged through standard input/output.

---

## General Description

In both tasks, your program communicates interactively with a simulated map.  
The map reveals only what Neo can perceive based on his **perception zone** variant.

**Symbols on the map:**
- `P` — Enemy perception zone  
- `A` — Agent Smith  
- `S` — Sentinel  
- `K` — Keymaker  
- `B` — Backdoor key  
- `N` — Neo  

---

## Task A — A_star

**Limits:** time — 30 s, memory — 256 MB  

### Input  
- First line: integer `1` or `2` — Neo’s perception zone variant.  
- Second line: two integers `(x, y)` — coordinates of the Keymaker.  
- After that, interaction with the map begins.

### Commands  
- `m x y` — move Neo to position `(x, y)` and receive the list of visible items.  
- `e n` — end the game, where `n` is the length of the shortest path from Neo to the Keymaker (or `-1` if unsolvable).  

### Output format (map response)
After each `m x y`, the map responds with:
```
n
x1 y1 c1
x2 y2 c2
...
```
where `n` is the number of perceived cells, each line giving coordinates and the symbol of a visible item.

### Example Interaction  
Keymaker located at `(3, 1)`:
```
1
3 1
m 0 0
0
m 1 0
0
m 2 0
1
3 1 K
m 3 1
e 4
```

---

## Task B — Backtracking_algorithm

**Limits:** time — 1 s, memory — 256 MB  

The interaction format, input, and commands are identical to Task A.  
The only difference is the **algorithm** used to reach the Keymaker — backtracking instead of A*.

### Example Interaction  
Keymaker located at `(3, 1)`:
```
1
3 1
m 0 0
0
m 2 0
1
3 1 K
m 3 1
e 4
```

---

## Algorithm Notes

- **A\*** (Task A) — uses heuristics (like Manhattan distance) to find the optimal path efficiently.  
- **Backtracking** (Task B) — explores all possible paths recursively until a valid one is found (less efficient but exhaustive).  

Both algorithms must rely only on perceived information and interact dynamically with the simulation.

---

## Usage

1. Compile or run the program in an environment supporting stdin/stdout interaction.  
2. The system will provide:
   - Variant number (`1` or `2`)  
   - Keymaker coordinates `(x, y)`  
3. Use `m x y` to explore and `e n` when the shortest path is known.  

---

## Example Map (for understanding)

```
N...PAP..
...KPPP..
.........
.........
...P.....
B.PSP..PP
...P...PA
.......PP
.........
```

### Coordinate System
- The **x-axis** is horizontal.  
- The **y-axis** is vertical.  
- The origin `(0, 0)` is at the **top left** corner.

---

**Author:** Interactive AI Pathfinding Demo  
**Language:** Any language supporting standard input/output (C++, Python, etc.)  
**Goal:** Demonstrate algorithmic reasoning in an interactive environment.
```

***
