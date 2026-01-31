#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

using namespace std;

const int N = 9; // Define grid size
const int dy[4] = { 1, 0, -1, 0 }; // Vertical movement directions
const int dx[4] = { 0, 1, 0, -1 }; // Horizontal movement directions
int globalMinPath = INT_MAX; // Track the minimum path to the Keymaker

// Hash function for unordered_map with pairs as keys
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

// Keymaker and key positions
pair<int, int> keymakerPos;
pair<int, int> keyPos;

// Set of danger zones and visited cells
set<pair<int, int>> dangerZones;
set<pair<int, int>> visited;

// Map of distances to cells to avoid revisiting
unordered_map<pair<int, int>, int, pair_hash> distances;

// Check if the cell is within the grid, not dangerous, and not visited
bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N &&
        dangerZones.find({ x, y }) == dangerZones.end() &&
        visited.find({ x, y }) == visited.end();
}

// Backtracking function to explore paths to the Keymaker
void backtrack(int x, int y, int pathLength) {
    // Output current position of Neo
    cout << "m " << x << " " << y << endl;

    // Get the number of danger zones from perception
    int numDanger;
    cin >> numDanger;

    // Process perception data
    for (int j = 0; j < numDanger; ++j) {
        int dangerX, dangerY;
        char dangerType;
        cin >> dangerX >> dangerY >> dangerType;

        // Update positions based on type
        if (dangerType == 'K') {
            keymakerPos = { dangerX, dangerY };
        }
        if (dangerType == 'B') {
            keyPos = { dangerX, dangerY };
        }
        else if (dangerType == 'P' || dangerType == 'A' || dangerType == 'S') {
            dangerZones.insert({ dangerX, dangerY });
        }
    }

    // Check if Neo reached the Keymaker
    if (make_pair(x, y) == keymakerPos) {
        globalMinPath = min(globalMinPath, pathLength);
        return;
    }

    // Early exit if current path is longer than the shortest path found
    if (pathLength >= globalMinPath) {
        return;
    }

    // Mark cell as visited and update distance map
    visited.insert({ x, y });
    if (distances.count({ x, y }) == 0) distances[{x, y}] = INT_MAX;
    distances[make_pair(x, y)] = min(pathLength, distances[make_pair(x, y)]);
    if (distances[make_pair(x, y)] < pathLength) return;

    // Gather valid moves in each direction
    vector<pair<int, int>> directions;
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isValid(newX, newY)) {
            directions.push_back(make_pair(newX, newY));
        }
    }

    // Sort directions to prioritize Keymaker cell if present
    sort(directions.begin(), directions.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a == keymakerPos ? true : b == keymakerPos ? false : false;
        return false;
        });

    // Explore each possible move direction recursively
    if (directions.empty()) return;
    for (const auto& direction : directions) {
        int newX = direction.first;
        int newY = direction.second;

        backtrack(newX, newY, pathLength + 1);

        // Reset Neo's position and reprocess perceptions
        cout << "m " << x << " " << y << endl;
        int numDanger;
        cin >> numDanger;

        for (int j = 0; j < numDanger; ++j) {
            int dangerX, dangerY;
            char dangerType;
            cin >> dangerX >> dangerY >> dangerType;

            // Update perception of positions
            if (dangerType == 'K') {
                keymakerPos = { dangerX, dangerY };
            }
            if (dangerType == 'B') {
                keyPos = { dangerX, dangerY };
            }
            else if (dangerType == 'P' || dangerType == 'A' || dangerType == 'S') {
                dangerZones.insert({ dangerX, dangerY });
            }
        }
    }
    visited.erase({ x, y });
}

// Main function to set up the game
int main() {
    int perceptionVariant;
    cin >> perceptionVariant; // Input variant

    // Read Keymaker position
    cin >> keymakerPos.first >> keymakerPos.second;

    // Start backtracking from initial position (0,0)
    backtrack(0, 0, 0);

    // Output result
    if (globalMinPath == INT_MAX) {
        cout << "e -1" << endl; // No path found
    }
    else {
        cout << "e " << globalMinPath << endl; // Shortest path found
    }

    return 0;
}
