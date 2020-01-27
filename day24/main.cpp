#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <array>
#include <cmath>
#include <tuple>
#include <cassert>
#include <optional>
#include <functional>
#include <stack>
#include <cassert>
#include <queue>
#include <list>
#include <fstream>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <numeric>


namespace{

using data_type = std::vector<std::vector<char>>;

const auto loadData = [](auto path){
    data_type res;
    char ch;
    bool insertNewLine = false;
    std::fstream fs(path, std::fstream::in);
    res.push_back({});
    while (fs.get(ch)) {
        if (insertNewLine) {
            res.push_back({});
            insertNewLine = false;
        }
        if (ch == '\n') {
            insertNewLine = true;
        }
        else {
            res.back().push_back(ch);
        }
    }
    fs.close();
    return res;
};

const auto print = [](data_type& d) {
    for (auto& line: d) {
        for (auto e: line) {
            std::cout << e;
        }
        std::cout << '\n';
    }
};
const auto visitTile = [](data_type& d, auto x, auto y) -> int {
    if (x >= d.size() || x < 0) return 0;
    if (y >= d[0].size() || y < 0) return 0;
    return d[x][y] == '#';
};

const auto getBugsNumberInAdjacentTiles = [](data_type& d, auto x, auto y) {
    return visitTile(d, x - 1, y)
        +  visitTile(d, x + 1, y)
        +  visitTile(d, x, y - 1)
        +  visitTile(d, x, y + 1);
};

const auto iterate = [](data_type& d) {
    auto tmp = d;
    for (auto i = 0u; i < d.size(); ++i) {
        for (auto j = 0u; j < d[0].size(); ++j) {
            const auto bugs = getBugsNumberInAdjacentTiles(d, i, j);
            if (d[i][j] == '#') {
                if (bugs != 1) tmp[i][j] = '.';
            }
            else if (d[i][j] == '.'){
                if (bugs == 1 || bugs == 2) tmp[i][j] = '#';
            }
        }
    }
    d = tmp;
};
const auto dumpToString = [](data_type& d) {
    std::string s;
    for (auto& line: d)
        for (auto e: line)
            s += e;
    return s;
};

const auto calculateBiodiversity = [](data_type& d) {
    uint64_t sum = 0;
    std::size_t i = 0;
    for (auto& line: d) {
        for (auto e: line) {
            if (e == '#') {
                sum += 1 << i;
            }
            ++i;
        }
    }
    return sum;
};
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "too few parameters!\n";
        return -1;
    }
    auto data = loadData(argv[1]);
    std::set<std::string> layouts;

    do {
        iterate(data);
        if (auto [_, isInserted] = layouts.insert(dumpToString(data)); !isInserted) {
            print(data);
            std::cout << "First puzzle answer: " << calculateBiodiversity(data) << '\n';
            break;
        }
    } while(true);
    return 0;
}