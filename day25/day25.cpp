#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<set<pair<int8_t, int8_t>>> keys, locks;

int main() {
    auto strt = high_resolution_clock::now();
    ifstream fi("day25.txt");
    string line;
    int y = 0;
    set<pair<int8_t, int8_t>> keyOrLock;
    while (getline(fi, line)) {
        if (line.empty()) {
            if (keyOrLock.count({0, 0}))
                locks.push_back(keyOrLock);
            else
                keys.push_back(keyOrLock);
            y = 0;
            keyOrLock.clear();
            continue;
        }
        for (int x = 0; x < line.size(); x++)
            if (line[x] == '#') keyOrLock.insert({x, y});
        y++;
    }
    if (keyOrLock.count({0, 0}))
        locks.push_back(keyOrLock);
    else
        keys.push_back(keyOrLock);
    int sum = 0;
    for (const auto& l : locks)
        for (const auto& k : keys) {
            map<pair<int8_t, int8_t>, int8_t> merged;
            for (const auto& pos : k) merged[pos]++;
            for (const auto& pos : l) merged[pos]++;
            bool match = true;
            for (const auto& m : merged)
                if (m.second != 1) {
                    match = false;
                    break;
                }
            if (match) sum++;
        }
    cout << "Day 25: Code Chronicle" << endl
         << "Part 1   - " << sum << endl
         << "Part 2   - free star" << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
