#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<uint32_t> secrets;

static void Next(uint32_t& secret) {
    uint32_t next = secret << 6;
    secret = (secret ^ next) & 0xffffff;
    next = secret >> 5;
    secret = (secret ^ next) & 0xffffff;
    next = secret << 11;
    secret = (secret ^ next) & 0xffffff;
}

static uint64_t Part1() {
    uint64_t result = 0;
    for (auto secret : secrets) {
        for (int i = 0; i < 2000; i++) Next(secret);
        result += secret;
    }
    return result;
}

constexpr int MaxSeq = 19 * 19 * 19 * 19;
static array<uint32_t, MaxSeq> sequences{}, prices{};

static uint32_t Part2() {
    for (auto secret : secrets) {
        int buyer = secret, price = secret % 10, seq = 0;
        for (int i = 0; i < 2000; i++) {
            Next(secret);
            int price2 = secret % 10, diff = price2 - price + 9;
            price = price2;
            seq = (seq * 19 + diff) % MaxSeq;
            if (sequences[seq] != buyer) {
                sequences[seq] = buyer;
                prices[seq] += price;
            }
        }
    }
    uint32_t totals = 0;
    for (const auto price : prices)
        if (price > totals) totals = price;
    return totals;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream fi("day22.txt");
    string line;
    while (getline(fi, line)) secrets.push_back(stoul(line));
    cout << "Day 22: Monkey Market" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
