#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<vector<char>> m;

static int H(int i, int j, const char w[]) {
    if (j <= m.size() - 4) return strncmp(&m[i][j], w, 4) == 0;
    return 0;
}

static int V(int i, int j, const char w[]) {
    if (i <= m.size() - 4) {
        int count = 1;
        for (int k = 0; k < 4; k++)
            if (m[i + k][j] != w[k]) count = 0;
        return count;
    }
    return 0;
}

static int DiagL(int i, int j, const char w[]) {
    if (i <= m.size() - 4 && j <= m.size() - 4) {
        int count(1);
        for (int k = 0; k < 4; k++)
            if (m[i + k][j + k] != w[k]) count = 0;
        return count;
    }
    return 0;
}

static int DiagR(int i, int j, const char w[]) {
    if (i <= m.size() - 4 && j >= 4 - 1) {
        int count(1);
        for (int k = 0; k < 4; k++)
            if (m[i + k][j - k] != w[k]) count = 0;
        return count;
    }
    return 0;
}

static int part1() {
    int count = 0;
    for (int i = 0; i < m.size(); i++)
        for (int j = 0; j < m.size(); j++)
            count += H(i, j, "XMAS") + H(i, j, "SAMX") + V(i, j, "XMAS") + V(i, j, "SAMX") +
                     DiagL(i, j, "XMAS") + DiagL(i, j, "SAMX") + DiagR(i, j, "XMAS") +
                     DiagR(i, j, "SAMX");
    return count;
}

static int part2() {
    int count(0);
    for (int i = 0; i <= m.size() - 3; i++) {
        for (int j = 0; j <= m.size() - 3; j++) {
            int ok(0);
            if (m[i + 1][j + 1] == 'A') {
                ok = 1;
                if (!((m[i][j] == 'M' && m[i + 2][j + 2] == 'S') ||
                      (m[i][j] == 'S' && m[i + 2][j + 2] == 'M')))
                    ok = 0;
                if (!((m[i + 2][j] == 'M' && m[i][j + 2] == 'S') ||
                      (m[i + 2][j] == 'S' && m[i][j + 2] == 'M')))
                    ok = 0;
            }
            count += ok;
        }
    }
    return count;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day04.txt");
    string line;
    while (getline(fi, line)) {
        vector<char> l;
        for (auto& c : line) l.push_back(c);
        m.push_back(l);
    }
    cout << "Day 4: Ceres Search" << endl
         << "part 1   - " << part1() << endl
         << "part 2   - " << part2() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}