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
#include <thread>

namespace {

struct Position {
    std::size_t x;
    std::size_t y;
    bool operator<(Position const& lhs) const noexcept {
        if(lhs.x < x) return true;
        else if (lhs.x == x) {
            return lhs.y < y;
        }
        return false;
    }
};

std::map<char, Position> keys;
using MAP = std::vector<std::vector<char>>;
using Visited = std::set<Position>;

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
    for (auto i = 0u; i < data.size(); ++i)
        for (auto j = 0u; j < data[i].size(); ++j)
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
            if (ch >= 'A' && ch <= 'Z') {
                locations[ch] = {i, j};
            }
            else if(ch >= 'a' && ch <= 'z') {
                locations[ch] = {i, j};
                keys[ch] = {i, j};
            }
            ++j;
        }
        ++i;
    }
    return locations;
};

void traverse(Position p, MAP map, uint32_t counter, Visited visited) {
    if (keys.size() == 0) {
        std::cout << "FINISHED in " << counter << " steps\n";
        return;
    }
    if (p.x >= map.size() || p.y >= map[0].size()) return;
    if (map[p.x][p.y] == '#') return;
    if (visited.count(p) > 0) {
        std::cout << "been here: (" << p.x << ',' << p.y << ")\n";
        return;
    }

    visited.insert(p);

    char& ch = map[p.x][p.y];
    //std::cout << "\n\t\t>" << ch << "<\n";
    if (ch >= 'a' && ch <= 'z'){
        std::cout << "Got key: " << ch << '\n';
        if (keys.size() == 1) {
            //std::cout << "FINISHED in " << counter << " steps\n";
        }
        keys.erase(ch);
    }
    //if we hit a door, and have no key then return!
    else if (ch >= 'A' && ch <= 'Z') {
        if (keys.count(ch + 32) == 1){
            //key has not been yet collected
            //can't open that very door
            return;
        }
        ch = '.';
    }
    auto c = ch;
    ch = 'S';
    printData(map);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ch = c;


    auto visiteda = visited;
    traverse({p.x, p.y +1}, map, counter + 1, visiteda);
    auto visitedb = visited;
    traverse({p.x, p.y -1}, map, counter + 1, visitedb);
    auto visitedc = visited;
    traverse({p.x+1, p.y}, map, counter + 1, visitedc);
    auto visitedd = visited;
    traverse({p.x-1, p.y}, map, counter + 1, visitedd);
}
}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    const auto input = loadData(path);
    //printData(input);

    auto map = input;
    auto pos = getInitialPosition(input);
    const auto locations = getKeysAndDoorsLocation(input);
    // for (auto &e: locations) {
    //     std::cout << e.first << " -> (" << e.second.x << ',' << e.second.y << ")\n";
    // }

    std::cout << pos.x << ' ' << pos.y << std::endl;

    traverse(pos, map, 1, {});
    return 0;
}
