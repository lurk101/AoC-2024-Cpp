#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> lines;

static int part1() {
    int count(0);
    smatch match;
    regex expr(R"(mul\((\d+)+,(\d+)+\))");
    for (auto str : lines)
        while (regex_search(str, match, expr)) {
            count += stoi(match[1].str()) * stoi(match[2].str());
            str = match.suffix().str();
        }
    return count;
}

static int part2() {
    bool mulOn(true);
    int count(0);
    smatch match;
    regex expr(R"(mul\((\d+)+,(\d+)+\))");
    regex on(R"(do\(\))");
    regex off(R"(don't\(\))");
    regex combo(R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))");
    for (auto str : lines)
        while (regex_search(str, match, combo)) {
            if (!mulOn && regex_match(match.str(), on)) {
                mulOn = true;
                continue;
            } else if (mulOn && regex_match(match.str(), off)) {
                mulOn = false;
                continue;
            } else if (mulOn && regex_match(match.str(), expr))
                count += stoi(match[1].str()) * stoi(match[2].str());
            str = match.suffix().str();
        }
    return count;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream fi("day03.txt");
    string line;
    while (getline(fi, line)) lines.push_back(line);
    cout << "Day 3: Mull It Over" << endl
         << "part 1   - " << part1() << endl
         << "part 2   - " << part2() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
