#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace chrono;

static int p1(vector<int>& left, vector<int>& right) {
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    int space = 0;
    for (int i = 0; i < left.size(); i++)
        space += abs(left[i] - right[i]);
    return space;
}

static int p2(vector<int>& left, vector<int>& right) {
    int space = 0;
    for (int i = 0; i < left.size(); i++)
        space += left[i] * int(count(right.begin(), right.end(), left[i]));
    return space;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day01.txt");
    int l, r;
    vector<int> left, right;
    while (fi >> l >> r) {
        left.push_back(l);
        right.push_back(r);
    }
    cout << "Day 1: Historian Hysteria" << endl
         << "Part 1   - " << p1(left, right) << endl
         << "Part 2   - " << p2(left, right) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
