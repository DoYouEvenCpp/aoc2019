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
        if(lhs.x > x) return true;
        else if (lhs.x == x) {
            return lhs.y < y;
        }
        return false;
    }
};

std::map<char, Position> keys;
std::set<uint32_t> steps_sum;
using MAP = std::vector<std::vector<char>>;
using Visited = std::map<Position, std::size_t>;
using KEYS = std::map<char, Position>;

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

void traverse(Position p, MAP& map, uint32_t counter, Visited& visited, KEYS keys) {
    if (keys.size() == 0) {
        return;
    }
    if (p.x >= map.size() || p.y >= map[0].size()) return;
    if (map[p.x][p.y] == '#') return;

    char ch = map[p.x][p.y];
    //commented code required for printing purposes, duh
    //auto tmp = ch;

    if (ch >= 'a' && ch <= 'z'){
        //std::cout << "\tGot key: " << ch << '\n';
        keys.erase(ch);
        //tmp = '.';
        if (keys.size() == 0) {
            //std::cout << "\tFINISHED in " << counter << " steps\n";
            steps_sum.insert(counter);
        }
    }
    else if (ch >= 'A' && ch <= 'Z') {
        if (keys.count(ch + 32) == 1){
            //key has not been yet collected
            //can't open that very door
            //return
            return;
        }
        //std::cout << "\tOpening door " << ch << '\n';
        //tmp = '.';
    }

    //map[p.x][p.y] = '*';
    //printData(map);
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //map[p.x][p.y] = tmp;

    if (visited.count(p) > 0) {
        if (visited[p] <= keys.size()) return;
            visited[p] = keys.size();
    }
    else {
        visited[p] = keys.size();
    }

    traverse({p.x, p.y +1}, map, counter + 1, visited, keys);
    traverse({p.x, p.y -1}, map, counter + 1, visited, keys);
    traverse({p.x+1, p.y}, map, counter + 1, visited, keys);
    traverse({p.x-1, p.y}, map, counter + 1, visited, keys);
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
    //std::cout << pos.x << ' ' << pos.y << std::endl;

    map[pos.x][pos.y] = '.';
    auto keys_ = keys;
    Visited v;
    traverse(pos, map, 0, v, keys_);

    std::cout << "\n\nShortest: " << *steps_sum.begin() << '\n';

    //7864 - too high
    return 0;
}
