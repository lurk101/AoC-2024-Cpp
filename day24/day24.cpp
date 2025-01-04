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
    string in0, op, in1, out;  // input 0, operator, input 1, output
};

static vector<gate> gates;
static map<string, int64_t> wires;
static vector<string> zWires;
static vector<pair<string, int8_t>> wireLevels;

static void Propagate() {
    bool run = true;
    while (run) {  // repeat till all z wires are resolved
        for (const auto& [in0, op, in1, d] : gates) {
            if (wires[in0] == -1 || wires[in1] == -1) continue;
            if (op[0] == 'X')  // XOR
                wires[d] = wires[in0] ^ wires[in1];
            else if (op[0] == 'A')  // AND
                wires[d] = wires[in0] & wires[in1];
            else if (op[0] == 'O')  // OR
                wires[d] = wires[in0] | wires[in1];
        }
        run = false;
        for (const auto wire : zWires)
            if (wires[wire] == -1) {
                run = true;
                break;
            }
    }
}

static auto Part1() {
    sort(zWires.begin(), zWires.end());
    Propagate();
    uint64_t r = 0;
    for (int i = 0; i < zWires.size(); i++) r |= (wires[zWires[i]] << i);
    return r;
}

static string GateOutput(const string a, const string oper, const string b) {
    for (const auto& gate : gates)
        if (gate.op == oper)
            if ((gate.in0 == a && gate.in1 == b) || (gate.in0 == b && gate.in1 == a))
                return gate.out;
    return "";
}

static vector<pair<string, string>> swaps;

static void TrySwap(string& a, string& b) {
    if (a.size() && a[0] == 'z') {
        swap(a, b);
        swaps.push_back({a, b});
    }
}

static auto Part2() {
    string I = "";
    for (int ix = 0; ix < 45; ix++) {
        string nn = (ix < 10) ? "0" + to_string(ix) : to_string(ix);
        string O = "", IxorSum = "", IandSum = "";
        string XxorY = GateOutput("x" + nn, "XOR", "y" + nn);
        string XandY = GateOutput("x" + nn, "AND", "y" + nn);
        if (I.size()) {
            IandSum = GateOutput(I, "AND", XxorY);
            IxorSum = GateOutput(I, "XOR", XxorY);
            if (IandSum.empty()) {
                swap(XandY, XxorY);
                swaps.push_back({XandY, XxorY});
                IandSum = GateOutput(I, "AND", XxorY);
            }
            TrySwap(XxorY, IxorSum);
            TrySwap(XandY, IxorSum);
            TrySwap(IandSum, IxorSum);
            O = GateOutput(IandSum, "OR", XandY);
        }
        if (O.size() && (O[0] == 'z') && (O != "z45")) {
            swap(O, IxorSum);
            swaps.push_back({O, IxorSum});
        }
        I = I.empty() ? XandY : O;
    }
    vector<string> d;
    for (const auto& [a, b] : swaps) {
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
        string w = line.substr(0, 3);
        int l = stoi(line.substr(4));
        wires[w] = l;
        wireLevels.push_back({w, l});
    }
    while (getline(fi, line)) {
        string in0, op, in1, d;
        stringstream(line) >> in0 >> op >> in1 >> d >> d;
        gates.push_back({in0, op, in1, d});
        if (d[0] == 'z') zWires.push_back(d);
    }
    for (const auto& gate : gates) {
        wires[gate.in0] = -1;
        wires[gate.in1] = -1;
        wires[gate.out] = -1;
    }
    for (const auto& l : wireLevels) wires[l.first] = l.second;
    cout << "Day 24: Crossed Wires" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
