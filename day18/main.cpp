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
#include <bitset>

namespace {

struct Position {
    std::size_t x;
    std::size_t y;
    int32_t steps;
    int32_t keys_;
    bool operator<(Position const& lhs) const noexcept {
        if(lhs.x > x) return true;
        else if (lhs.x == x) {
            return lhs.y < y;
        }
        return false;
    }

    bool operator==(Position const& lhs) const {
        return lhs.x == x && lhs.y == y;
    }
};

using MAP = std::vector<std::vector<char>>;
using Visited = std::map<Position, std::size_t>;
using KEYS = std::map<char, Position>;
KEYS keys;
std::set<uint32_t> steps_sum;

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

const auto getInitialPositions = [](auto& data) {
    std::vector<Position> res;
    for (auto i = 0u; i < data.size(); ++i)
        for (auto j = 0u; j < data[i].size(); ++j)
            if (data[i][j] == '@') res.push_back({ i, j });
    return res;
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

int32_t bfs(MAP map, Position startingPos, int32_t expectedSize = -1){
    std::queue<Position> q;
    std::set<std::tuple<int, int, int32_t>> visited;

    std::vector<int> x_dir = {-1, 0 ,0, 1};
    std::vector<int> y_dir = {0, -1 ,1, 0};
    std::bitset<26> keys_collected;
    const auto total_keys = expectedSize == -1? keys.size():expectedSize;


    q.push(startingPos);
    while (not q.empty()) {
        auto m = q.front();
        q.pop();

        keys_collected = m.keys_;

        if (keys_collected.count() == total_keys)
            return m.steps;

        for (int i = 0; i < 4; ++i) {

            const auto x = m.x + x_dir[i];
            const auto y = m.y + y_dir[i];

            if (x >= map.size() || y >= map[0].size()) continue;
            const char c = map[x][y];
            if (c == '#') continue;


            if (visited.count(std::make_tuple(x, y, keys_collected.to_ulong()))) continue;

            const auto cp = keys_collected;
            if (c >= 'A' && c <= 'Z') {
                if (!keys_collected.test(c - 'A')) continue;
            }
            else if (c >= 'a' && c <= 'z') {
                if (!keys_collected.test(c - 'a')) {
                    keys_collected.set(c-'a');
                }
            }

            visited.emplace(x, y, keys_collected.to_ulong());
            q.push({x, y, m.steps + 1, static_cast<int32_t>(keys_collected.to_ulong())});
            keys_collected = cp;
        }

    }

    return -1;
}

const auto collectDataAndIgnoreDoors = [](MAP& map, Position pos){
    const auto max_x = map.size();
    const auto max_y = map[0].size();

    const auto x_start = (max_x / 2) > pos.x?0:pos.x-1;
    const auto y_start = (max_y / 2) > pos.y?0:pos.y-1;

    const auto x_end = x_start == 0?pos.x+1:max_x;
    const auto y_end = y_start == 0?pos.y+1:max_y;

    std::vector<char> doors;
    std::vector<char> k;
    for (auto i = x_start; i < x_end; ++i) {
        for (auto j = y_start; j < y_end; ++j){
            if (map[i][j] >= 'a' && map[i][j] <= 'z') k.push_back(map[i][j]);
            else if (map[i][j] >= 'A' && map[i][j] <= 'Z') doors.push_back(map[i][j]);
        }
    }

    for (auto i = x_start; i < x_end; ++i) {
        for (auto j = y_start; j < y_end; ++j){
            if (map[i][j] >= 'A' && map[i][j] <= 'Z') {
                const auto ch = map[i][j];
                if (std::none_of(k.begin(), k.end(), [ch](char c){return c == ch+32;}))
                    if (std::any_of(keys.begin(), keys.end(), [ch](auto const& c){return c.first == ch+32;})) {
                        map[i][j] = '.';
                    }
            }
        }
    }

    return std::tuple(k, doors);
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
    //printData(input);

    auto map = input;
    auto startingPosition = getInitialPosition(input);
    const auto locations = getKeysAndDoorsLocation(input);

    auto keys_ = keys;

    std::cout << "First puzzle answer: " <<  bfs(map, startingPosition) << '\n';

    map[startingPosition.x][startingPosition.y] = '#';
    map[startingPosition.x - 1][startingPosition.y] = '#';
    map[startingPosition.x+1][startingPosition.y] = '#';
    map[startingPosition.x][startingPosition.y - 1] = '#';
    map[startingPosition.x][startingPosition.y + 1] = '#';
    map[startingPosition.x + 1][startingPosition.y - 1] = '@';
    map[startingPosition.x + 1][startingPosition.y + 1] = '@';
    map[startingPosition.x - 1][startingPosition.y + 1] = '@';
    map[startingPosition.x - 1][startingPosition.y - 1] = '@';

    auto positions = getInitialPositions(map);


    //int -> (keys, doors)
    std::map<int, std::tuple<std::vector<char>, std::vector<char>>> tmp;
    int i = 0;
    for (auto p : positions) {
        tmp[i++] = collectDataAndIgnoreDoors(map, p);
    }

    int32_t sum = 0;

    for (auto i = 0; i < 4; ++i) {
        sum += bfs(map, positions[i], std::get<0>(tmp[i]).size());
    }
    std::cout << "second puzzle: " << sum << '\n';
    return 0;
}
