#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

const int GRID_SIZE = 9; // Define grid size for the game

// Directions for moving: up, down, left, right
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };

// Class to represent a cell in the grid
class Cell {
public:
    int x, y;
    bool isDangerous;
    char entity;

    Cell() : x(0), y(0), isDangerous(false), entity('.') {}
    Cell(int x, int y) : x(x), y(y), isDangerous(false), entity('.') {}
};

// Class to represent Neo's character
class Neo {
public:
    int x, y;
    bool hasBackdoorKey;

    Neo(int x, int y) : x(x), y(y), hasBackdoorKey(false) {}
};

// Class to represent the Keymaker's character
class Keymaker {
public:
    int x, y;

    Keymaker(int x, int y) : x(x), y(y) {}
};

// Node structure for A* algorithm
struct Node {
    int x, y;
    int g, h; // g = cost from start, h = heuristic

    Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h) {}

    bool operator<(const Node& other) const {
        return (g + h) > (other.g + other.h); // Min-heap for A* priority queue
    }
};

// Calculate Manhattan distance as the heuristic for A* algorithm
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Check if a cell is within grid boundaries
bool isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

// Remove Agent Smith's perception zones from the grid
void removeAgentSmithPerception(vector<vector<Cell>>& grid, const vector<pair<int, int>>& agentSmithPositions) {
    for (const auto& pos : agentSmithPositions) {
        int x = pos.first;
        int y = pos.second;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (isValid(x + i, y + j) && (i != 0 || j != 0)) {
                    grid[x + i][y + j].isDangerous = false;
                    grid[x + i][y + j].entity = '.';
                }
            }
        }
    }
}

// A* Algorithm to find the shortest path to the Keymaker
int astar(vector<vector<Cell>>& grid, Neo& neo, Keymaker& keymaker) {
    priority_queue<Node> pq;
    pq.push(Node(neo.x, neo.y, 0, manhattanDistance(neo.x, neo.y, keymaker.x, keymaker.y)));

    vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
    visited[neo.x][neo.y] = true;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // If Neo reaches the Keymaker
        if (current.x == keymaker.x && current.y == keymaker.y) {
            neo.x = current.x;
            neo.y = current.y;
            return current.g; // Return path length
        }

        // Explore adjacent cells
        for (int i = 0; i < 4; ++i) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValid(newX, newY) && !visited[newX][newY] && !grid[newX][newY].isDangerous) {
                visited[newX][newY] = true;
                int newG = current.g + 1;
                int newH = manhattanDistance(newX, newY, keymaker.x, keymaker.y);
                pq.push(Node(newX, newY, newG, newH));
            }
        }
    }

    return -1; // Return -1 if no path is found
}

// Recursive function to process perceptions of entities in the grid
void processPerception(int remaining, vector<vector<Cell>>& grid, Neo& neo, int& sentinelX, int& sentinelY) {
    if (remaining <= 0) return; // Base case: no more perceptions to process

    int x, y;
    char c;
    cin >> x >> y >> c; // Read perception input

    if (isValid(x, y)) {
        grid[x][y].entity = c;
        grid[x][y].isDangerous = (c == 'P' || c == 'S' || c == 'A'); // Mark dangerous entities

        if (c == 'S') { // Update Sentinel position if found
            sentinelX = x;
            sentinelY = y;
        }

        if (c == 'B') { // Key's position
            grid[x][y].entity = 'B';
        }
    }

    processPerception(remaining - 1, grid, neo, sentinelX, sentinelY); // Recurse to process next perception
}

// Function to handle game interaction and Neo's movement
void interact(int variant, Keymaker& keymaker) {
    Neo neo(0, 0); // Start Neo at the top-left corner

    // Initialize the grid
    vector<vector<Cell>> grid(GRID_SIZE, vector<Cell>(GRID_SIZE));
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = Cell(i, j);
        }
    }

    int sentinelX = -1, sentinelY = -1;

    // Start interaction loop
    while (true) {
        // Output Neo's current position
        cout << "m " << neo.x << " " << neo.y << endl;

        // Wait for perception input
        int numPerceived;
        cin >> numPerceived;

        // Process the perceptions
        processPerception(numPerceived, grid, neo, sentinelX, sentinelY);

        // Use A* to find shortest path to the Keymaker
        int shortestPathLength = astar(grid, neo, keymaker);
        if (shortestPathLength == -1) {
            cout << "e -1" << endl;
            break; // End if no path is found
        }

        cout << "e " << shortestPathLength << endl; // Output path length

        // Exit loop if Neo reaches the Keymaker
        if (neo.x == keymaker.x && neo.y == keymaker.y) {
            break;
        }

        // Move Neo down or reset if at the bottom
        if (neo.x < GRID_SIZE - 1) {
            neo.x++; // Move down
        }
        else {
            neo.x = 0; // Reset position or change direction
        }
    }
}

// Main function to initialize the game
int main() {
    int variant;
    cin >> variant;
    int keymakerX, keymakerY;
    cin >> keymakerX >> keymakerY;

    Keymaker keymaker(keymakerX, keymakerY);
    interact(variant, keymaker);

    return 0;
}
