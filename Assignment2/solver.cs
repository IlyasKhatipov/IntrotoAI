using System;
using System.Linq;

class MyScript
{
    private const string BlueColor = "\u001b[34m";
    
    private const string YellowColor = "\u001b[33m";
    private const string ResetColor = "\u001b[0m";

    public static (int[,], bool[,]) HandleInput(string[] data)
    {
        int[,] sudoku = new int[9, 9];
        bool[,] isFixed = new bool[9, 9];

        for (int i = 0; i < 9; i++)
        {
            var numbers = data[i].Replace("-", "0").Split(' ', StringSplitOptions.RemoveEmptyEntries);
            for (int j = 0; j < 9; j++)
            {
                sudoku[i, j] = int.Parse(numbers[j]);
                isFixed[i, j] = sudoku[i, j] != 0;
            }
        }

        return (sudoku, isFixed);
    }

    public static int[,] GetIndividual(int[,] initialSudoku, bool[,] isFixed)
    {
        int[,] individual = (int[,])initialSudoku.Clone();
        Random random = new Random();

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (!isFixed[i, j])
                    individual[i, j] = random.Next(1, 10);
            }
        }

        return individual;
    }

    public static int CalculateFitness(int[,] individual, bool[,] isFixed)
    {
        int fitness = 0;

        for (int i = 0; i < 9; i++)
        {
            bool[] seen = new bool[10];
            for (int j = 0; j < 9; j++)
            {
                int num = individual[i, j];
                if (num != 0 && seen[num]) fitness++;
                seen[num] = true;
            }
        }

        for (int j = 0; j < 9; j++)
        {
            bool[] seen = new bool[10];
            for (int i = 0; i < 9; i++)
            {
                int num = individual[i, j];
                if (num != 0 && seen[num]) fitness++;
                seen[num] = true;
            }
        }

        for (int row = 0; row < 9; row += 3)
        {
            for (int col = 0; col < 9; col += 3)
            {
                bool[] seen = new bool[10];
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        int num = individual[row + i, col + j];
                        if (num != 0 && seen[num]) fitness++;
                        seen[num] = true;
                    }
                }
            }
        }

        return fitness;
    }

    public static int[,] Crossover(int[,] parent1, int[,] parent2, Random random)
    {
        int[,] child = new int[9, 9];

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                child[i, j] = random.Next(2) == 0 ? parent1[i, j] : parent2[i, j];
            }
        }

        return child;
    }

    public static int[,] Mutate(int[,] individual, bool[,] isFixed, double mutationRate, Random random)
    {
        int[,] mutatedIndividual = (int[,])individual.Clone();

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (!isFixed[i, j] && random.NextDouble() < mutationRate)
                {
                    mutatedIndividual[i, j] = random.Next(1, 10);
                }
            }
        }

        return mutatedIndividual;
    }

    public static void Visualize(int[,] individual, bool[,] isFixed)
    {
        Console.WriteLine("┌───────┬───────┬───────┐");
        for (int i = 0; i < 9; i++)
        {
            Console.Write("│ ");
            for (int j = 0; j < 9; j++)
            {
                Console.Write((isFixed[i, j] ? BlueColor : YellowColor) + individual[i, j] + ResetColor + (j % 3 == 2 ? " │ " : " "));
            }
            Console.WriteLine();
            if (i % 3 == 2 && i != 8) Console.WriteLine("├───────┼───────┼───────┤");
        }
        Console.WriteLine("└───────┴───────┴───────┘");
    }

    public static void WriteIndividualToConsole(int[,] individual)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                Console.Write(individual[i, j] + (j < 8 ? " " : ""));
            }
            Console.WriteLine();
        }
    }

    public static int[,] InitializePopulation(int populationSize, int[,] sudoku, bool[,] isFixed)
    {
        int[,] population = new int[populationSize, 81];
        for (int i = 0; i < populationSize; i++)
        {
            int[,] individual = GetIndividual(sudoku, isFixed);
            for (int row = 0; row < 9; row++)
            {
                for (int col = 0; col < 9; col++)
                {
                    population[i, row * 9 + col] = individual[row, col];
                }
            }
        }
        return population;
    }

    public static int[,] TournamentSelection(int[,] population, int[] fitnesses, int tournamentSize, Random random)
    {
        int[,] selected = new int[2, 81];

        for (int i = 0; i < 2; i++)
        {
            int bestIndex = -1;
            int bestFitness = int.MaxValue;

            for (int j = 0; j < tournamentSize; j++)
            {
                int index = random.Next(fitnesses.Length);
                if (fitnesses[index] < bestFitness)
                {
                    bestFitness = fitnesses[index];
                    bestIndex = index;
                }
            }

            for (int k = 0; k < 81; k++)
            {
                selected[i, k] = population[bestIndex, k];
            }
        }

        return selected;
    }

    public static int[,] RunGenerations(int[,] population, int populationSize, int[,] sudoku, bool[,] isFixed, bool needLog, Random random)
    {
        int[,] bestIndividual = new int[9, 9];
        int bestFitness = int.MaxValue;
        int generation = 0;
        int noImprovementCount = 0;
        int noImprovementThreshold = 1000;
        double mutationRate = 0.01;

        while (bestFitness > 0)
        {
            int[] fitnesses = new int[populationSize];
            for (int i = 0; i < populationSize; i++)
            {
                int[,] individual = new int[9, 9];
                for (int row = 0; row < 9; row++)
                {
                    for (int col = 0; col < 9; col++)
                    {
                        individual[row, col] = population[i, row * 9 + col];
                    }
                }
                fitnesses[i] = CalculateFitness(individual, isFixed);
            }

            int bestIndex = Array.IndexOf(fitnesses, fitnesses.Min());
            if (fitnesses[bestIndex] < bestFitness)
            {
                bestFitness = fitnesses[bestIndex];
                for (int row = 0; row < 9; row++)
                {
                    for (int col = 0; col < 9; col++)
                    {
                        bestIndividual[row, col] = population[bestIndex, row * 9 + col];
                    }
                }
                noImprovementCount = 0;
            }
            else
            {
                noImprovementCount++;
            }

            if (needLog)
            {
                Console.Clear();
                Console.WriteLine($"Generation: {generation + 1}, Best Fitness: {bestFitness}");
                Visualize(bestIndividual, isFixed);
            }
            if (bestFitness == 0) break;

            if (noImprovementCount >= noImprovementThreshold)
            {
                if (needLog) Console.WriteLine($"No improvement for {noImprovementThreshold} generations, restarting...");
                population = InitializePopulation(populationSize, sudoku, isFixed);
                generation = 0;
                bestFitness = int.MaxValue;
                noImprovementCount = 0;
                noImprovementThreshold = random.Next(100, 251);
            }

            int[,] newPopulation = new int[populationSize, 81];
            for (int i = 0; i < populationSize; i++)
            {
                int[,] parents = TournamentSelection(population, fitnesses, 3, random);
                int[,] parent1 = new int[9, 9];
                int[,] parent2 = new int[9, 9];
                for (int row = 0; row < 9; row++)
                {
                    for (int col = 0; col < 9; col++)
                    {
                        parent1[row, col] = parents[0, row * 9 + col];
                        parent2[row, col] = parents[1, row * 9 + col];
                    }
                }

                int[,] child = Crossover(parent1, parent2, random);
                int[,] mutatedChild = Mutate(child, isFixed, mutationRate, random);

                for (int row = 0; row < 9; row++)
                {
                    for (int col = 0; col < 9; col++)
                    {
                        newPopulation[i, row * 9 + col] = mutatedChild[row, col];
                    }
                }
            }

            for (int row = 0; row < 9; row++)
            {
                for (int col = 0; col < 9; col++)
                {
                    newPopulation[0, row * 9 + col] = bestIndividual[row, col];
                }
            }

            List<(int fitness, int index)> fitnessList = fitnesses.Select((fit, idx) => (fit, idx)).ToList();
            fitnessList.Sort((a, b) => a.fitness.CompareTo(b.fitness));

            int[,] sortedPopulation = new int[populationSize, 81];
            for (int i = 0; i < populationSize; i++)
            {
                int sortedIndex = fitnessList[i].index;
                for (int j = 0; j < 81; j++)
                {
                    sortedPopulation[i, j] = population[sortedIndex, j];
                }
            }

            int cutOff = populationSize * 2 / 3;
            for (int i = cutOff; i < populationSize; i++)
            {
                for (int row = 0; row < 9; row++)
                {
                    for (int col = 0; col < 9; col++)
                    {
                        sortedPopulation[i, row * 9 + col] = newPopulation[i, row * 9 + col];
                    }
                }
            }

            population = sortedPopulation;
            generation++;
        }

        return bestIndividual;
    }

    public static void Main(string[] args)
    {
        string[] data = new string[9];

#pragma warning disable CS8601 // Possible null reference assignment.
        for (int i = 0; i < 9; i++) { data[i] = Console.ReadLine(); }
#pragma warning restore CS8601 // Possible null reference assignment.
        (int[,] sudoku, bool[,] isFixed) = HandleInput(data);
        int populationSize = 10000;
        Random random = new Random();
        
        int[,] population = InitializePopulation(populationSize, sudoku, isFixed);
        int[,] bestIndividual = RunGenerations(population, populationSize, sudoku, isFixed, false, random);
        WriteIndividualToConsole(bestIndividual);
    }
}
