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

namespace {
struct Position {
    std::size_t x;
    std::size_t y;
    bool operator==(Position const& o) const {
        return o.x == x && o.y == y;
    }
};

struct Portal {
    Position out;
    Position in;
};
struct Node{
    std::string element;
    std::size_t range;
    Position pos;
};


using map_type = std::vector<std::vector<char>>;
std::map<std::string, Portal> m;
std::map<std::string, std::list<Node>> tree;
std::list<Node> nodes;
std::vector<map_type> maps;
std::vector<size_t> ranges;
std::size_t level = 0;
std::unordered_set<std::string> visited;
std::size_t range = 0;


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
    std::cout << "\n\tLEVEL: " << level << '\n';
    for (auto& line: data) {
        for (auto ch: line)
            std::cout << ch;
    }
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
    const auto width = input[0].size();
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

                if (j == 0 || j == width - 2 - 1)
                    m[portalName].out = { x, y };
                else
                    m[portalName].in = { x, y };
            }
        }
    }

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

                if (j == 0 || j == input.size() - 2)
                    m[portalName].out = { x, y };
                else
                    m[portalName].in = { x, y };
            }
        }
    }
    if (m["AA"].in == Position{0,0}) {
        m["AA"].in = m["AA"].out;
    }
    else
        m["AA"].out = m["AA"].in;

    if (m["ZZ"].in == Position{0,0}) {
        m["ZZ"].in = m["ZZ"].out;
    }
    else
        m["ZZ"].out = m["ZZ"].in;
};
const auto getPositions(std::string point) {
    return std::make_tuple(m[point].in, m[point].out);
}
const auto getEmptyPositionCount = [](auto& map) {
    std::size_t sum = 0;
    for (auto& line : map) {
        sum += std::count_if(line.begin(), line.end(), [](auto el) {return el == '.'; });
    }
    return sum;
};

std::size_t searchPossibleMovesFromPosition(std::string from, Position pos, Position end) {
    auto x = pos.x;
    auto y = pos.y;

    if (x >= maps[level].size()) return 0;
    if (y >= maps[level][x].size()) return 0;
    if (maps[level][x][y] == '#' || maps[level][x][y] == ' ' || maps[level][x][y] == '@') {
        return 0;
    }
    if (pos == end && level == 0) {
        std::cout << "\t\t\t\t\t\t\tFINISHED! In " << range << " steps\n";
        return 0;
    }

    if (maps[level][x][y] == '.') {
        maps[level][x][y] = '@';
        if (level == 0)
            range++;

        auto xx = x;
        auto yy = y;
        auto it = std::find_if(m.begin(), m.end(), [&](auto& p) {
            return p.second.in == pos || p.second.out == pos; });
        if (it != m.end()) {
            const std::string current = it->first;
            if (from != current && current != "ZZ" && current != "AA") {
                if (x == it->second.in.x && y == it->second.in.y) {
                    xx = it->second.out.x;
                    yy = it->second.out.y;
                    //if (level < m.size() - 2)
                    ++level;
                    std::cout << "recurse into level " << level << " through " << current << std::endl;
                    range += 1;
                }
                else {
                    xx = it->second.in.x;
                    yy = it->second.in.y;
                    if (level > 0)
                        --level;
                    std::cout << "Return to level " << level << " through " << current <<  std::endl;
                    range += 1;
                }
                searchPossibleMovesFromPosition(current, { xx, yy }, end);
            }
        }
    }

    if (maps[level][x][y] >= 'A' && maps[level][x][y] <= 'Z') {
        return 0;
    }


    // printData(maps[level]);
    // std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // std::getchar();

    auto sum = 0;
    sum += searchPossibleMovesFromPosition(from, { x - 1, y }, end);
    sum += searchPossibleMovesFromPosition(from, { x + 1, y }, end);
    sum += searchPossibleMovesFromPosition(from, { x, y + 1 }, end);
    sum += searchPossibleMovesFromPosition(from, { x, y - 1 }, end);
    if (getEmptyPositionCount(maps[level]) == 0) {
        while (getEmptyPositionCount(maps[level]) == 0) {
            if (level > 0) --level;
        }
    }
    return range + sum;
};
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

    for (auto& entry: m) {
        std::cout << entry.first << '\t'
                  << '(' << entry.second.out.x << ',' << entry.second.out.y << ')'
                  << ", " << entry.second.in.x << ',' << entry.second.in.y << ")\n";
    }

    for (auto i = 0; i < m.size() - 1 /*except ZZ portal*/; ++i) {
        maps.push_back(input);
        ranges.push_back(0);
    }
    map = input;
    searchPossibleMovesFromPosition("AA", startingPos, ending);

    //516 - OK
    return 0;
}
