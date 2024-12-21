#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct Point {
    int x, y;
    bool operator==(const Point& p) const { return (y == p.y) && (x == p.x); }
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
};

static vector<Point> bytes;
static const Point start = {0, 0};
static const Point finish = {70, 70};
static const vector<Point> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static int Part1(int cycle) {
    int dx = finish.x - start.x + 1;
    int dy = finish.y - start.y + 1;
    vector<vector<bool>> grid(dx, vector<bool>(dy, false));
    vector<vector<bool>> visited(dx, vector<bool>(dy, false));
    vector<vector<int>> dist(dx, vector<int>(dy, 0));
    for (int i = 0; i <= cycle; i++) grid[bytes[i].x][bytes[i].y] = true;
    queue<pair<Point, int>> q;
    q.push({start, 0});
    visited[start.x][start.y] = true;
    while (q.size()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) return d;
        for (auto dir : dirs) {
            auto [x, y] = cur + dir;
            if (x < start.x || x > finish.x || y < start.y || y > finish.y || visited[x][y] ||
                grid[x][y])
                continue;
            visited[x][y] = true;
            dist[x][y] = d + 1;
            q.push({{x, y}, d + 1});
        }
    }
    if (!visited[finish.x][finish.y]) return -1;
    return dist[finish.x][finish.y];
}

static string Part2() {
    int ans = 0;
    for (int i = 1024; i < bytes.size(); i++)
        if (Part1(i) == -1) return to_string(bytes[i].x) + "," + to_string(bytes[i].y);
    return "not found";
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream fi("day18.txt");
    string line;
    while (getline(fi, line)) {
        int x, y;
        sscanf(line.c_str(), "%d,%d", &x, &y);
        bytes.push_back({x, y});
    }
    cout << "Day 18: RAM Run" << endl
         << "Part 1   - " << Part1(1023) << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
