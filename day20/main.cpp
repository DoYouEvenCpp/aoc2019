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
    int steps;
    int level;
};

bool operator == (Position a, Position b) { return a.x == b.x && a.y == b.y; }
bool operator <  (Position a, Position b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }

struct Portal {
    Position out;
    Position in;
};


using map_type = std::vector<std::vector<char>>;
std::map<std::string, Portal> m;
std::vector<map_type> maps;
std::map<Position, Position> portals;
std::map<Position, int> depth_change;


const auto loadData = [](auto path) {
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
const auto printData = [](auto& data, int level, int x=0, int y=0, char c=0) {
    if (x != 0 && y != 0)
        data[x][y] = '*';

    std::cout << "\n\tLEVEL: " << level << '\n';
    for (auto& line : data) {
        for (auto ch : line)
            std::cout << ch;
    }
    if (x != 0 && y != 0){
        data[x][y] = c;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
};
const auto findPosition = [](auto& input, char mark, char otherMark) -> Position {
    std::vector<Position> pos;
    for (auto i = 0u; i < input.size(); ++i) {
        if (pos.size() == 2) break;
        for (auto j = 0u; j < input[i].size(); ++j) {
            if (input[i][j] == mark) {
                if (input[i + 1][j] == otherMark) {
                    pos.push_back({ i,j });
                    pos.push_back({ i + 1,j });
                    break;
                }
                else if (input[i][j + 1] == otherMark) {
                    pos.push_back({ i,j });
                    pos.push_back({ i,j + 1 });
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
                return { first.x, first.y - 1 }; // .AA
        if (input[first.x][second.y + 1] == '.')
            return { first.x, second.y + 1 }; // AA.
    }
    else { //vertical
        if (first.x > 1)                            // .
            if (input[first.x - 1][first.y] == '.')   // A
                return { first.x - 1, first.y };        // A


        if (input[second.x + 1][first.y] == '.')      // A
            return { second.x + 1, first.y };           // A
    }                                               // .
    assert(false);
    return {};
};
const auto getStartingPosition = [](auto& input) -> Position {
    return findPosition(input, 'A', 'A');
};
const auto getEndingPos = [](auto& input) -> Position {
    return findPosition(input, 'Z', 'Z');
};
const auto searchPortals = [](auto const& input) {
    const auto width = input[0].size();
    //scan for horizontal ones
    for (auto i = 0u; i < input.size(); ++i) {
        for (auto j = 0u; j < input[i].size() - 1; ++j) {
            if ((input[i][j] >= 'A' && input[i][j] <= 'Z') && (input[i][j + 1] >= 'A' && input[i][j + 1] <= 'Z')) {
                const std::size_t x = i;
                std::size_t y = 0;
                if (j > 1)
                    if (input[i][j - 1] == '.') {
                        y = j;
                    }
                if (input[i][j + 2] == '.') {
                    y = j + 1;
                }
                std::string portalName;
                portalName += input[i][j];
                portalName += input[i][j + 1];

                if (j == 0 || i ==0 || i == width - 2 || j == width -2 || i == width - 3 || j == width -3){
                    m[portalName].out = { x, y };
                    depth_change[{x,y}] = -1;
                }
                else {
                    m[portalName].in = { x, y };
                    depth_change[{x,y}] = 1;
                }
            }
        }
    }

    for (auto i = 0u; i < width; ++i) {
        for (auto j = 0u; j < input.size() - 1; ++j) {
            if ((input[j][i] >= 'A' && input[j][i] <= 'Z') && (input[j + 1][i] >= 'A' && input[j + 1][i] <= 'Z')) {
                std::size_t x = 0;
                const std::size_t y = i;
                if (j > 1)
                    if (input[j - 1][i] == '.') {
                        x = j;
                    }
                if (j < input.size() - 2u)
                    if (input[j + 2][i] == '.') {
                        x = j + 1;
                    }
                std::string portalName;
                portalName += input[j][i];
                portalName += input[j + 1][i];

                if (j == 0 || i ==0 || i == input.size() - 2u || j == input.size() -2u|| i == input.size() - 3u || j == input.size() -3u) {
                    m[portalName].out = { x, y };
                    depth_change[{x,y}] = -1;
                }
                else{
                    m[portalName].in = { x, y };
                    depth_change[{x,y}] = 1;
                }
            }
        }
    }
    if (m["AA"].in == Position{ 0,0 }) {
        m["AA"].in = m["AA"].out;
    }
    else
        m["AA"].out = m["AA"].in;

    if (m["ZZ"].in == Position{ 0,0 }) {
        m["ZZ"].in = m["ZZ"].out;
    }
    else
        m["ZZ"].out = m["ZZ"].in;

    for (auto& e: m) {
        if (e.first != "AA" && e.first != "ZZ") {
            portals[e.second.in] = e.second.out;
            portals[e.second.out] = e.second.in;
        }
    }
};

int bfs(map_type& map, Position startingPos, Position endPos) {
    std::queue<Position> q;
    const auto MAX_DEPTH = m.size() - 1;
    std::array<int, 4> x_dir = { -1, 0 ,0, 1 };
    std::array<int, 4> y_dir = { 0, -1 ,1, 0 };
    std::vector<std::set<std::tuple<int, int>>> visited(MAX_DEPTH + 1);

    visited[0].emplace(startingPos.x, startingPos.y);
    q.push(startingPos);

    while (not q.empty()) {
        auto move = q.front();
        q.pop();

        if (move == endPos && move.level == 0) {
            return move.steps-1;
        }


        for (int i = 0; i < 4; ++i) {

            auto x = move.x + x_dir[i];
            auto y = move.y + y_dir[i];
            auto level = move.level;

            if (visited[level].count(std::make_tuple(x,y)) > 0) continue;
            if (x >= maps[level].size() || y >= maps[level][0].size()) continue;
            const char c = maps[level][x][y];
            if ( c == '#') continue;

            if (c == '.'){
                q.push({x,y, move.steps +1, level});
                visited[level].emplace(x, y);
            }
            else if (c >= 'A' && c <= 'Z' && portals.count({x,y})) {
                auto newPos = portals[{x,y}];
                newPos.steps = move.steps;
                level += depth_change[{x,y}];
                if (level > MAX_DEPTH) level = MAX_DEPTH;
                else if (level < 0) continue;
                newPos.level = level;
                q.push(newPos);
                visited[level].emplace(newPos.x, newPos.y);
            }
        }

    }
    return -1;
}
}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    auto input = loadData(path);
    searchPortals(input);
    auto map = input;
    auto startingPos = m["AA"].in;
    auto ending = getEndingPos(input);

    for (auto& entry: m) {
        std::cout << entry.first << '\t'
                  << '(' << entry.second.out.x << ',' << entry.second.out.y << ')'
                  << ", " << entry.second.in.x << ',' << entry.second.in.y << ")\n";
    }

    for (auto i = 0u; i < m.size() - 1 /*except ZZ portal*/; ++i) {
        maps.push_back(input);
    }
    map = input;
    std::cout << bfs(map, startingPos, ending) << '\n';

    //516  - OK
    //5966 - OK
    return 0;
}
