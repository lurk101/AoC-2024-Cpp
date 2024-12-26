#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;
using namespace chrono;

struct gate {
    string s0, op, s1, d;
};

static vector<gate> gates;
static map<string, uint64_t> wires;
static vector<string> zWires;
static vector<pair<string, int>> wireLevels;

static vector<string> Split(const string s, const string delimiter) {
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
    bool run = true;
    while (run) {
        for (const auto& [s0, op, s1, d] : gates) {
            if (wires[s0] == -1 || wires[s1] == -1) continue;
            if (op[0] == 'X')
                wires[d] = wires[s0] ^ wires[s1];
            else if (op[0] == 'A')
                wires[d] = wires[s0] & wires[s1];
            else if (op[0] == 'O')
                wires[d] = wires[s0] | wires[s1];
        }
        run = false;
        for (const auto wire : zWires)
            if (wires[wire] == -1) {
                run = true;
                break;
            }
    }
}

static string Cache(const string a, const string oper, const string b) {
    for (const auto& gate : gates)
        if (gate.op == oper)
            if ((gate.s0 == a && gate.s1 == b) || (gate.s0 == b && gate.s1 == a)) return gate.d;
    return "";
}

static void TrySwap(string& a, string& b, vector<pair<string, string>>& swaps) {
    if (a.size() && a[0] == 'z') {
        swap(a, b);
        swaps.push_back({a, b});
    }
}

static uint64_t Part1() {
    sort(zWires.begin(), zWires.end());
    Propagate();
    uint64_t r = 0;
    for (int i = 0; i < zWires.size(); i++) r |= (wires[zWires[i]] << i);
    return r;
}

static string Part2() {
    vector<pair<string, string>> swaps;
    string I = "";
    for (int ix = 0; ix < 45; ix++) {
        string nn = (ix < 10) ? "0" + to_string(ix) : to_string(ix);
        string out = "", IxorSum = "", IandSum = "";
        string XxorY = Cache("x" + nn, "XOR", "y" + nn);
        string XandY = Cache("x" + nn, "AND", "y" + nn);
        if (I.size()) {
            IandSum = Cache(I, "AND", XxorY);
            IxorSum = Cache(I, "XOR", XxorY);
            if (IandSum.empty()) {
                swap(XandY, XxorY);
                swaps.push_back({XandY, XxorY});
                IandSum = Cache(I, "AND", XxorY);
            }
            TrySwap(XxorY, IxorSum, swaps);
            TrySwap(XandY, IxorSum, swaps);
            TrySwap(IandSum, IxorSum, swaps);
            out = Cache(IandSum, "OR", XandY);
        }
        if (out.size() && out[0] == 'z' && out != "z45") {
            swap(out, IxorSum);
            swaps.push_back({out, IxorSum});
        }
        I = I.empty() ? XandY : out;
    }
    vector<string> d;
    for (const auto [a, b] : swaps) {
        d.push_back(a);
        d.push_back(b);
    }
    sort(d.begin(), d.end());
    string s = "";
    for (const auto& d : d) {
        if (s.size()) s += ",";
        s += d;
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
        string w = parts[0];
        wires[w] = v;
        if (w[0] == 'z') zWires.push_back(w);
        wireLevels.push_back({w, v});
    }
    while (getline(fi, line)) {
        string s0, op, s1, d;
        stringstream(line) >> s0 >> op >> s1 >> d >> d;
        gates.push_back({s0, op, s1, d});
        if (d[0] == 'z') zWires.push_back(d);
    }
    for (auto gate : gates) {
        wires[gate.s0] = -1;
        wires[gate.s1] = -1;
        wires[gate.d] = -1;
    }
    for (auto l : wireLevels) wires[l.first] = l.second;
    cout << "Day 24: Crossed Wires" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
