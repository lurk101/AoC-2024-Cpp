#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static bool safe(vector<int>& levels) {
    bool ascending(levels[1] > levels[0]);
    for (int i = 1; i < levels.size(); i++) {
        int dif = levels[i - 1] - levels[i];
        if ((dif == 0) || (abs(dif) > 3)) return false;
        if (ascending) {
            if (levels[i - 1] > levels[i]) return false;
        } else if (levels[i - 1] < levels[i])
            return false;
    }
    return true;
}

static bool safe2(vector<int>& levels) {
    for (int i = 0; i < levels.size(); i++) {
        vector<int> l(levels);
        l.erase(l.begin() + i);
        if (safe(l)) return true;
    }
    return false;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day02.txt");
    string line;
    int part1(0), part2(0);
    while (getline(fi, line)) {
        stringstream ssline(line);
        vector<int> levels;
        int level;
        while (ssline >> level) levels.push_back(level);
        if (safe(levels)) {
            part1++;
            part2++;
        } else if (safe2(levels))
            part2++;
    }
    cout << "Day 2: Red-Nosed Reports" << endl
         << "part 1   - " << part1 << endl
         << "part 2   - " << part2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
