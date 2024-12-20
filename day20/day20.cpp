#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> grid;
static pair<int, int> start, finish;
static const vector<pair<int, int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static auto Search(vector<string>& grid, pair<int, int>& start, pair<int, int>& finish) {
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    vector<vector<int>> dist(grid.size(), vector<int>(grid[0].size(), -1));
    queue<pair<pair<int, int>, int>> q;
    q.push({start, 0});
    visited[start.first][start.second] = true;
    dist[start.first][start.second] = 0;
    while (!q.empty()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) break;
        for (auto dir : dirs) {
            int y = cur.first + dir.first;
            int x = cur.second + dir.second;
            if (x < 0 || x >= grid[0].size() || y < 0 || y >= grid.size() || visited[y][x] ||
                grid[y][x] == '#')
                continue;
            visited[y][x] = true;
            dist[y][x] = d + 1;
            q.push({{y, x}, d + 1});
        }
    }
    return dist;
}

static int Manhattan(pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

static int Part(int minStps) {
    int s = 0;
    const int rows = int(grid.size());
    const int cols = int(grid[0].size());
    vector<vector<int>> fromStart = Search(grid, start, finish);
    vector<vector<int>> fromFinish = Search(grid, finish, start);
    int deflt = fromStart[finish.first][finish.second];
    map<pair<pair<int, int>, pair<int, int>>, bool> checked;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '#') continue;
            for (int newR = max(0, r - minStps); newR <= min(rows, r + minStps); newR++) {
                if (newR < 0 || newR >= rows) continue;
                for (int newC = max(0, c - minStps); newC <= min(cols, c + minStps); newC++) {
                    if ((newC < 0 || newC >= cols) || (grid[newR][newC] == '#') ||
                        (Manhattan({r, c}, {newR, newC}) > minStps) ||
                        (fromStart[r][c] < 0 || fromFinish[newR][newC] < 0))
                        continue;
                    int cheat =
                        fromStart[r][c] + fromFinish[newR][newC] + Manhattan({r, c}, {newR, newC});
                    if (deflt - cheat >= 100) s++;
                }
            }
        }
    }
    return s;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream fi("day20.txt");
    string line;
    int r = 0;
    while (getline(fi, line)) {
        grid.push_back(line);
        for (int c = 0; c < line.size(); c++)
            if (line[c] == 'S')
                start = {r, c};
            else if (line[c] == 'E')
                finish = {r, c};
        r++;
    }
    cout << "Day 20: Race Condition" << endl
         << "Part 1   - " << Part(2) << endl
         << "Part 2   - " << Part(20) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
