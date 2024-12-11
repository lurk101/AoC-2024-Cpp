#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

class Solution {

private:

    fstream fi;
    vector<uint64_t> data;
    map<pair<uint64_t, int>, uint64_t> cache;

    vector<string> split(string s, string delimiter) {
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

    void parse() {
        string line;
        while (getline(fi, line)) {
            vector<string> nums = split(line, " ");
            for (auto x : nums)
                data.push_back(stoll(x, NULL, 10));
        }
    }

    uint64_t count(uint64_t num, int blinks) {
        if (blinks == 0) return 1;
        uint64_t& r = cache[{num, blinks}];
        if (r != 0) return cache[{num, blinks}];
        if (num == 0) return r = count(1, blinks - 1);
        string str = to_string(num);
        if (str.size() % 2 == 0) {
            string fst = str.substr(0, str.size() / 2);
            string scnd = str.substr(str.size() / 2, str.size() / 2);
            return r = count(stoll(fst, NULL, 10), blinks - 1) +
                       count(stoll(scnd, NULL, 10), blinks - 1);
        } else
            return r = count(num * 2024, blinks - 1);
    }

public:

    Solution(string fName) {
        fi.open(fName, ios::in);
        parse();
    }

    uint64_t part(int partNum) {
        int blinks = partNum == 1 ? 25 : 75;
        uint64_t s = 0ULL;
        for (auto x : data)
            s += count(x, blinks);
        return s;
    }
};

int main() {
    auto start = high_resolution_clock::now();
    Solution s{Solution("day11.txt")};
    cout << "Day 11: Plutonian Pebbles" << endl
         << "part 1   - " << s.part(1) << endl
         << "part 2   - " << s.part(2) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
