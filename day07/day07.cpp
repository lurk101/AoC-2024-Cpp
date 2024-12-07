#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> split(string s, char c) {
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = s.find(c)) != string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + 1);
    }
    tokens.push_back(s);
    return tokens;
}
bool match1(const int64_t ans, const vector<int>& oprnds) {
    if (oprnds.size() == 1) return oprnds[0] == ans;
    for (int64_t i = 0; i < pow(2, oprnds.size() - 1) + 1; ++i) {
        int64_t res = oprnds[0];
        for (int j = 1; j < oprnds.size(); ++j) {
            bool is_plus = ((1 << (j - 1)) & i) == 0;
            const int o = oprnds[j];
            res = is_plus ? res + o : res * o;
            if (res == ans) return true;
        }
    }
    return false;
}

int64_t concat(int64_t a, int64_t b) {
    int64_t magnitude = 1;
    while (magnitude <= b) magnitude *= 10;
    return magnitude * a + b;
}

bool match2(const int64_t ans, const vector<int>& oprnds) {
    if (oprnds.size() == 1) return oprnds[0] == ans;
    const int exp = 2 * int(oprnds.size()) - 2;
    for (int64_t i = 0; i <= pow(2, exp); ++i) {
        int64_t res = oprnds[0];
        for (int j = 1; j < oprnds.size(); ++j) {
            const int o = oprnds[j];
            const int indices = 2 * j - 2;
            const int want = (3 << indices) & i;
            const int adjusted = want >> indices;
            switch (adjusted) {
            case 0:
                res = res + o;
                break;
            case 1:
                res = res * o;
                break;
            case 2:
            case 3:
                res = concat(res, o);
                break;
            }
            if (res > ans) break;
        }
        if (res == ans) return true;
    }
    return false;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day07.txt");
    struct fmt {
        fmt(int64_t a, vector<int> n) : ans(a), nums(n) {}
        int64_t ans;
        vector<int> nums;
    };
    vector<fmt> inputs;
    while (true) {
        string line;
        getline(fi, line);
        if (line.empty())
            break;
        vector<string> sp = split(line, ':');
        const int64_t line_ans = stoll(sp[0]);
        vector<string> sp2 = split(sp[1], ' ');
        vector<int> numbers;
        numbers.reserve(sp2.size());
        for (const string& s : sp2) {
            if (s == "")
                continue;
            numbers.push_back(stoi(s));
        }
        inputs.push_back(fmt(line_ans, numbers));
    }
    int64_t p1 = 0;
    for (const fmt& format : inputs) {
        const int64_t ans = format.ans;
        const vector<int> oprnds = format.nums;
        if (match1(ans, oprnds))
            p1 += ans;
    }
    int64_t p2 = 0;
    for (const fmt& f : inputs) {
        const int64_t ans = f.ans;
        const vector<int> oprnds = f.nums;
        if (match1(ans, oprnds) || match2(ans, oprnds)) p2 += ans;
    }
    cout << "Day 7: Bridge Repair" << endl
         << "part 1   - " << p1 << endl
         << "part 2   - " << p2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
