#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

unordered_map<string, uint64_t> stones;

static vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> r;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        r.push_back(token);
    }
    r.push_back(s.substr(pos_start));
    return r;
}

static void Insert(unordered_map<string, uint64_t>& stones, string& key, uint64_t count) {
    if (stones.find(key) != stones.end())
        stones[key] += count;
    else
        stones[key] = count;
}

static void NextGen() {
    unordered_map<string, uint64_t> next;
    for (auto x : stones) {
        auto& [name, count] = x;
        if (name == "0") {
            string s1("1");
            Insert(next, s1, count);
            continue;
        }
        int sz = int(name.size());
        if ((sz & 1) == 0) {
            auto s1 = name.substr(0, sz / 2);
            auto s2 = name.substr(sz / 2, sz / 2);
            s1 = to_string(stoll(s1, NULL, 10));
            Insert(next, s1, count);
            s2 = to_string(stoll(s2, NULL, 10));
            Insert(next, s2, count);
            continue;
        }
        auto s1 = to_string(2024 * stoll(name));
        Insert(next, s1, count);
    }
    stones.swap(next);
}

static uint64_t part(int start, int blinks) {
    for (int i = start; i < blinks; i++) NextGen();
    uint64_t s = 0ULL;
    for (auto x : stones) {
        auto& [_, v] = x;
        s += v;
    }
    return s;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day11.txt");
    string line;
    while (getline(fi, line)) {
        vector<string> nums = split(line, " ");
        for (auto x : nums) Insert(stones, x, 1);
    }
    cout << "Day 11: Plutonian Pebbles" << endl
        << "part 1   - " << part(0, 25) << endl
        << "part 2   - " << part(25, 75) << endl
        << "run time - "
        << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
        << " ms." << endl;
}
