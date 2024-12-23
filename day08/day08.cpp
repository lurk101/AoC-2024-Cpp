#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;
using namespace chrono;

static int maxCol, maxRow;
static map<pair<int, int>, char> ants;

static bool inside(const pair<int, int>& p) {
    return p.first >= 0 && p.first < maxRow && p.second >= 0 && p.second < maxCol;
}

static int nodes() {
    set<pair<int, int>> n;
    for (const auto& [point, kind] : ants) {
        for (const auto& [other, otherKind] : ants) {
            if (point == other || kind != otherKind) continue;
            int dx{other.second - point.second}, dy{other.first - point.first};
            pair<int, int> p{other.first + dy, other.second + dx};
            if (inside(p)) n.insert(p);
        }
    }
    return int(n.size());
}

static int harmonics() {
    set<pair<int, int>> n;
    for (const auto& [point, kind] : ants) {
        for (const auto& [other, otherKind] : ants) {
            if (point == other || kind != otherKind) continue;
            int dx{other.second - point.second}, dy{other.first - point.first};
            for (pair<int, int> p{other.first + dy, other.second + dx}; inside(p);
                 p.first += dy, p.second += dx)
                n.insert(p);
            for (pair<int, int> p{other.first - dy, other.second - dx}; inside(p);
                 p.first -= dy, p.second -= dx)
                n.insert(p);
        }
    }
    return int(n.size());
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day08.txt");
    string line;
    while (getline(fi, line)) {
        int col = 0;
        for (const auto& c : line) {
            if (c != '.') ants.try_emplace({maxRow, col}, c);
            col++;
        }
        maxRow++;
        maxCol = int(line.size());
    }
    cout << "Day 8: Resonant Collinearity" << endl
         << "Part 1   - " << nodes() << endl
         << "Part 2   - " << harmonics() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
