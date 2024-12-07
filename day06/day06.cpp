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

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day06.txt");
    string line;
    while (getline(fi, line))
        ;
    cout << "Day 6: Guard Gallivant" << endl
         << "part 1   - " << 0 << endl
         << "part 2   - " << 0 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
