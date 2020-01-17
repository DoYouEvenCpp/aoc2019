#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
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

namespace {
struct Position {
    int x;
    int y;
};

const auto loadData = [](auto path){
    std::vector<std::vector<char>> res;
    {
        char ch;
        std::fstream fs(path, std::fstream::in);
        res.push_back({});
        while (fs.get(ch)) {
            if(ch == '\n') {
                res.back().push_back(ch);
                res.push_back({});
            }
            else {
                res.back().push_back(ch);
            }
        }
        fs.close();
    }
    return res;
};
const auto printData = [](auto& data) {
    for (auto& line: data) {
        for (auto ch: line)
            std::cout << ch;
    }
};
const auto getInitialPosition = [](auto& data) -> Position {
    for (auto i = 0; i < data.size(); ++i)
        for (auto j = 0; j < data[i].size(); ++j)
            if (data[i][j] == '@') return {i, j};
    return {};
};

const auto searchPossibleMovesFromPosition = [](auto& input, Position pos) -> std::list<Position> {
    return {};
};

const auto getKeysAndDoorsLocation = [](auto& input){
    std::map<char, Position> locations;
    std::size_t i = 0, j = 0;
    for (auto line: input) {
        j = 0;
        for (char ch: line) {
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                locations[ch] = {i, j};
            }
            ++j;
        }
        ++i;
    }
    return locations;
};
}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    const auto input = loadData(path);
    printData(input);
    auto pos = getInitialPosition(input);
    const auto locations = getKeysAndDoorsLocation(input);
    for (auto &e: locations) {
        std::cout << e.first << " -> (" << e.second.x << ',' << e.second.y << ")\n";
    }
    return 0;
}
