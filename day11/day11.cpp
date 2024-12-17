#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

static unordered_map<string, uint64_t> pebbles;

static vector<string> Split(string s, string delimiter) {
    size_t pos = 0, posEnd, delimLen = delimiter.length();
    string token;
    vector<string> r;
    while ((posEnd = s.find(delimiter, pos)) != string::npos) {
        token = s.substr(pos, posEnd - pos);
        pos = posEnd + delimLen;
        r.push_back(token);
    }
    r.push_back(s.substr(pos));
    return r;
}

static inline void Insert(string& key, uint64_t count) {
    pebbles[key] += count;
}

static void NextGen() {
    unordered_map<string, uint64_t> prev;
    prev.swap(pebbles);
    for (auto& pebble : prev) {
        auto& [id, count] = pebble;
        if (id == "0") {
            string s1("1");
            Insert(s1, count);
            continue;
        }
        int sz = int(id.size());
        if ((sz & 1) == 0) {
            auto s1 = id.substr(0, sz / 2);
            s1 = to_string(stoll(s1));
            auto s2 = id.substr(sz / 2, sz / 2);
            s2 = to_string(stoll(s2));
            Insert(s1, count);
            Insert(s2, count);
            continue;
        }
        auto s1 = to_string(2024 * stoll(id));
        Insert(s1, count);
    }
}

static uint64_t Part(int start, int blinks) {
    for (int i = start; i < blinks; i++) NextGen();
    uint64_t s = 0;
    for (auto pebble : pebbles) s += pebble.second;
    return s;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day11.txt");
    string line;
    while (getline(fi, line)) {
        vector<string> nums = Split(line, " ");
        for (auto n : nums) Insert(n, 1);
    }
    cout << "Day 11: Plutonian Pebbles" << endl
         << "Part 1   - " << Part(0, 25) << endl
         << "Part 2   - " << Part(25, 75) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
