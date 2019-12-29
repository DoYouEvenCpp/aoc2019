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

namespace {
struct Position {
    std::size_t x;
    std::size_t y;
    bool operator==(Position const& o) const {
        return o.x == x && o.y == y;
    }
};

struct Portal {
    Position first;
    Position second;
};
struct Node{
    std::string element;
    std::size_t range;
    Position pos;
};

std::map<std::string, Portal> m;
std::map<std::string, std::list<Node>> tree;
std::list<Node> nodes;

const auto loadData = [](auto path){
    std::vector<std::vector<char>> res;
    {
        char ch;
        bool insertNewLine = false;
        std::fstream fs(path, std::fstream::in);
        res.push_back({});
        while (fs.get(ch)) {
            if (insertNewLine) {
                res.push_back({});
                insertNewLine = false;
            }
            res.back().push_back(ch);
            if (ch == '\n') {
                insertNewLine = true;
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
auto flag = true;

std::stack<std::string> level;

std::size_t searchPossibleMovesFromPosition(std::vector<std::vector<char>>& input, std::string from, Position pos, Position end, std::size_t range) {
    auto x = pos.x;
    auto y = pos.y;

    if (x >= input.size()) return 0;
    if (y >= input[x].size()) return 0;
    if (input[x][y] == '#' || input[x][y] == ' ' || input[x][y] == '@'){
        return 0;
    }
    if (pos == end && level.size() == 0) {
        std::cout << "\t\t\t\t\t\t\tFINISHED! In " << range << " steps\n";
    }

    if (input[x][y] == '.') {
        input[x][y] = '@';
        range++;

        auto xx = x;
        auto yy = y;
        auto it = std::find_if(m.begin(), m.end(), [&](auto& p) {
            return p.second.first == pos || p.second.second == pos; });
        if (it != m.end()) {
            if (x == it->second.first.x && y == it->second.first.y) {
                xx = it->second.second.x;
                yy = it->second.second.y;
            }
            else {
                xx = it->second.first.x;
                yy = it->second.first.y;
            }
            if (from != it->first){
                nodes.push_back({ it->first, range - 1, {xx, yy} });
            /*if (flag) {*/
                if (level.size() > 0 && it->first == level.top()) {
                    level.pop();
                }
                else {
                    level.push(it->first);
                }
                searchPossibleMovesFromPosition(input, it->first, { xx, yy }, end, range);
            }
        }
    }

    if (input[x][y] >= 'A' && input[x][y] <= 'Z') {
        return 0;
    }


    printData(input);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto sum = 0;
    sum += searchPossibleMovesFromPosition(input, from, {x-1, y}, end, range);
    sum += searchPossibleMovesFromPosition(input, from, {x+1, y}, end, range);
    sum += searchPossibleMovesFromPosition(input, from, {x, y+1}, end, range);
    sum += searchPossibleMovesFromPosition(input, from, {x, y-1}, end, range);
    return range + sum;
};

const auto findPosition = [](auto& input, char mark, char otherMark) -> Position{
    std::vector<Position> pos;
    for (auto i = 0u; i < input.size(); ++i) {
        if (pos.size() == 2) break;
        for (auto j = 0u; j < input[i].size(); ++j) {
            if (input[i][j] == mark) {
                if (input[i+1][j] == otherMark) {
                    pos.push_back({i,j});
                    pos.push_back({i+1,j});
                    break;
                }
                else if (input[i][j+1] == otherMark) {
                    pos.push_back({i,j});
                    pos.push_back({i,j+1});
                    break;
                }
            }
        }
    }
    assert(pos.size() == 2);
    auto& first = pos[0];
    auto& second = pos[1];
    if (first.x == second.x) {
        //horizontal
        if (first.y > 1)
            if (input[first.x][first.y - 1] == '.')
                return {first.x, first.y-1}; // .AA
        if (input[first.x][second.y + 1] == '.')
            return {first.x, second.y+1}; // AA.
    }
    else { //vertical
        if (first.x > 1)                            // .
            if (input[first.x-1][first.y] == '.')   // A
                return {first.x-1, first.y};        // A


        if (input[second.x+1][first.y] == '.')      // A
            return {second.x+1, first.y};           // A
    }                                               // .
    assert(false);
    return {};
};

const auto getStartingPosition = [](auto& input) -> Position{
    return findPosition(input, 'A', 'A');
};
const auto getEndingPos = [](auto& input) -> Position{
    return findPosition(input, 'Z', 'Z');
};
const auto searchPortals = [](auto const& input) { 
    
    //scan for horizontal ones
    for (auto i = 0u; i < input.size(); ++i) {
        for (auto j = 0u; j < input[i].size()- 1; ++j) {
            if ((input[i][j] >= 'A' && input[i][j] <= 'Z') && (input[i][j + 1] >= 'A' && input[i][j + 1] <= 'Z')) {
                const std::size_t x = i;
                std::size_t y = 0;
                if (j > 1)
                    if (input[i][j - 1] == '.') {
                        y = j - 1;
                    }
                if (input[i][j + 2] == '.') {
                    y = j + 2;
                }
                std::string portalName;
                portalName += input[i][j];
                portalName += input[i][j + 1];
                if (m.count(portalName) == 0) {
                    m[portalName].first = { x, y };
                }
                else
                    m[portalName].second = { x, y };
            }
        }
    }

    const auto width = input[0].size();
    for (auto i = 0; i < width; ++i) {
        for (auto j = 0; j < input.size() - 1; ++j) {
            if ((input[j][i] >= 'A' && input[j][i] <= 'Z') && (input[j + 1][i] >= 'A' && input[j + 1][i] <= 'Z')) {
                std::size_t x = 0;
                const std::size_t y = i;
                if (j > 1)
                    if (input[j - 1][i] == '.') {
                        x = j - 1;
                    }
                if (j < input.size() - 2)
                    if (input[j + 2][i] == '.') {
                        x = j + 2;
                    }
                std::string portalName;
                portalName += input[j][i];
                portalName += input[j+1][i];
                if (m.count(portalName) == 0) {
                    m[portalName].first = { x, y };
                }
                else
                    m[portalName].second = { x, y };
            }
        }
    }
    m["AA"].second = m["AA"].first;
    m["ZZ"].second = m["ZZ"].first;
};
const auto getPositions(std::string point) {
    return std::make_tuple(m[point].first, m[point].second);
}
}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    //const  std::string path = "C:\\Users\\staho\\projects\\aoc2019\\_sln\\ConsoleApplication1\\x64\\Debug\\test1";
    auto input = loadData(path);
    auto startingPos = getStartingPosition(input);
    auto ending = getEndingPos(input);
    auto map = input;
    searchPortals(input);

   /* std::map<std::string, std::list<Node>> connections;
    connections["AA"] = {};
    connections["ZZ"] = {};
    for (auto& p : m) {
         auto& f = p.second.first;
         auto& s = p.second.second;
         std::cout << p.first << ':' << f.x << ',' << f.y
             << " -> " << s.x << ',' << s.y << '\n';
        connections[p.first] = {};
    }

    flag = false;
    for (auto& p : connections) {
        nodes = {};
        map = input;
        searchPossibleMovesFromPosition(map, p.first, m[p.first].first, ending, 0);
        map = input;
        searchPossibleMovesFromPosition(map, p.first, m[p.first].second, ending, 0);
        tree[p.first] = nodes;
    }

    for (auto& e : tree) {
        std::cout << e.first << "=> ";
        for (auto& node : e.second) {
            std::cout << '\t' << node.element << ": " << node.range;
        }
        std::cout << '\n';
    }*/

    flag = true;
    map = input;
    searchPossibleMovesFromPosition(map, "AA", startingPos, ending, 0);

    //516 - OK
    return 0;
}
