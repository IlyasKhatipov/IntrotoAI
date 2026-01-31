```markdown
# Sudoku Solver - Genetic Algorithm

## Algorithm Description

This C# implementation solves Sudoku puzzles using a **genetic algorithm** that evolves a population of candidate solutions through natural selection principles.

### Core Components

**1. Individual Representation**
- Each Sudoku grid (9×9) is an "individual" 
- Fixed cells (given numbers) remain unchanged
- Empty cells are filled with random numbers 1-9

**2. Fitness Function**
Counts **conflicts** across:
```
Rows + Columns + 3×3 boxes = Total fitness score
```
- Perfect solution: `fitness = 0` (no duplicates)
- Lower fitness = better individual

**3. Evolutionary Operators**

**Selection** (Tournament Selection):
```
For each parent:
  Pick k random individuals
  Choose the one with lowest fitness
```

**Crossover** (Uniform):
```
For each cell: randomly inherit from parent1 (50%) or parent2 (50%)
```

**Mutation** (1% rate):
```
For each empty cell: replace with random number (1-9) with probability 0.01
```

**4. Main Evolution Loop**
```
While best_fitness > 0:
  1. Calculate fitness for entire population (10,000 individuals)
  2. Keep best individual (elitism)
  3. Tournament selection → parents
  4. Crossover + Mutation → new children  
  5. Replace 2/3 worst individuals with children
  6. If no improvement after 100-250 generations → restart population
```

**5. Termination**
Stops when `fitness = 0` (valid Sudoku found).

### Population Flow
```
Initial Population (10K random grids)
     ↓ (evaluate fitness)
Sorted by fitness (best → worst)
     ↓
[Elite: best 1] + [Children: 2/3 population] + [Survivors: 1/3 best]
     ↓ repeat until fitness = 0
```

**Key Innovation**: Dynamic restart prevents local optima trapping.
```

***
