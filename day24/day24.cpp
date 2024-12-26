#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

struct instr {
    string oprnd0, op, oprnd1, rslt;
};

static map<string, uint64_t> w;
static vector<string> zW;
static vector<instr> is;
static vector<pair<string, int>> is2;

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

static void Propagate() {
    for (;;) {
        bool allDone = true;
        for (auto z : zW)
            if (w[z] == -1) {
                allDone = false;
                break;
            }
        if (allDone) break;
        for (auto i : is) {
            auto [oprnd0, op, oprnd1, rslt] = i;
            if (w[oprnd0] == -1 || w[oprnd1] == -1) continue;
            if (op == "XOR")
                w[rslt] = w[oprnd0] ^ w[oprnd1];
            else if (op == "AND")
                w[rslt] = w[oprnd0] & w[oprnd1];
            else if (op == "OR")
                w[rslt] = w[oprnd0] | w[oprnd1];
        }
    }
}

static string Find(string a, string oper, string b) {
    for (auto i : is)
        if (i.op == oper)
            if ((i.oprnd0 == a && i.oprnd1 == b) || (i.oprnd0 == b && i.oprnd1 == a)) return i.rslt;
    return "";
}

static uint64_t Part1() {
    sort(zW.begin(), zW.end());
    Propagate();
    uint64_t r = 0;
    for (int i = 0; i < zW.size(); i++) r += (w[zW[i]] << i);
    return r;
}

static string Part2() {
    vector<pair<string, string>> swaps;
    string in = "";
    for (int ix = 0; ix < 45; ix++) {
        string n = (ix < 10) ? "0" + to_string(ix) : to_string(ix);
        string out = "", sumWith = "", fromInAndSum = "";
        string sumWithout = Find("x" + n, "XOR", "y" + n),
               fromXandY = Find("x" + n, "AND", "y" + n);
        if (in != "") {
            fromInAndSum = Find(in, "AND", sumWithout);
            sumWith = Find(in, "XOR", sumWithout);
            if (fromInAndSum == "") {
                swap(fromXandY, sumWithout);
                swaps.push_back({fromXandY, sumWithout});
                fromInAndSum = Find(in, "AND", sumWithout);
            }
            if (sumWithout != "" && sumWithout[0] == 'z') {
                swap(sumWithout, sumWith);
                swaps.push_back({sumWithout, sumWith});
            }
            if (fromXandY != "" && fromXandY[0] == 'z') {
                swap(fromXandY, sumWith);
                swaps.push_back({fromXandY, sumWith});
            }
            if (fromInAndSum != "" && fromInAndSum[0] == 'z') {
                swap(fromInAndSum, sumWith);
                swaps.push_back({fromInAndSum, sumWith});
            }
            out = Find(fromInAndSum, "OR", fromXandY);
        }
        if (out != "" && out[0] == 'z' && out != "z45") {
            swap(out, sumWith);
            swaps.push_back({out, sumWith});
        }
        in = (in == "") ? fromXandY : out;
    }
    vector<string> rslt;
    for (auto [a, b] : swaps) {
        rslt.push_back(a);
        rslt.push_back(b);
    }
    sort(rslt.begin(), rslt.end());
    string s = "";
    for (const auto& r : rslt) {
        if (s.size()) s += ",";
        s += r;
    }
    return s;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream fi("day24.txt");
    string line;
    while (getline(fi, line)) {
        if (line == "") break;
        vector<string> parts = Split(line, ": ");
        int v = stoi(parts[1]);
        w[parts[0]] = v;
        if (parts[0][0] == 'z') zW.push_back(parts[0]);
        is2.push_back({parts[0], v});
    }
    while (getline(fi, line)) {
        char a[20], b[20], c[20], d[20];
        sscanf(line.c_str(), "%s %s %s -> %s", a, b, c, d);
        is.push_back({string(a), string(b), string(c), string(d)});
        if (d[0] == 'z') zW.push_back(string(d));
    }
    for (auto z : zW) w[z] = -1;
    for (auto i : is) {
        w[i.oprnd0] = -1;
        w[i.oprnd1] = -1;
        w[i.rslt] = -1;
    }
    for (auto i : is2) w[i.first] = i.second;
    cout << "Day 24: Crossed Wires" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
