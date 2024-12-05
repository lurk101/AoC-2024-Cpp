#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace chrono;

static unordered_map<int, unordered_set<int>> pages;
static int part1, part2;

static bool LT(int lhs, int rhs) { return pages[lhs].find(rhs) != pages[lhs].end(); }

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day05.txt");
    string line;
    while (getline(fi, line) && (line != ""))
        pages[stoi(line.substr(0, 2))].insert(stoi(line.substr(3, 2)));
    while (getline(fi, line)) {
        vector<int> ordered;
        for (int i = 0; i < line.length(); i += 3) ordered.push_back(stoi(line.substr(i, 2)));
        auto unOrdered = ordered;
        sort(ordered.begin(), ordered.end(), LT);
        if (unOrdered == ordered)
            part1 += ordered[ordered.size() / 2];
        else
            part2 += ordered[ordered.size() / 2];
    }
    cout << "Day 5: Print Queue" << endl
         << "part 1   - " << part1 << endl
         << "part 2   - " << part2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
