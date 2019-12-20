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
#include <chrono>

namespace {
struct Position {
    std::size_t x;
    std::size_t y;
};

struct Portal {
    Position pos;
    char mark;
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

void searchPossibleMovesFromPosition(std::vector<std::vector<char>>& input, Position pos) {
    const auto x = pos.x;
    const auto y = pos.y;
    if (x >= input.size()) return;
    if (y >= input[x].size()) return;
    if (input[x][y] == '@')
        return;
    if (input[x][y] == '#' || input[x][y] == ' '){
        std::cout << "Hit: " << input[x][y] << " on (" << x << ", " << y << ")\n";
        return;
    }
    if (input[x][y] == 'A') {
        std::cout << "Hit: A on (" << x << ", " << y << ")\n";
        return;
    }
    if (input[x][y] >= 'B' && input[x][y] <= 'Z') {
        std::cout << "Hit: " << input[x][y] << " on (" << x << ", " << y << ")\n";
        input[x][y] = '@';
    }

    if (input[x][y] == '.') {
        std::cout << "Hit: . on (" << x << ", " << y << ")\n";
        input[x][y] = '@';
    }

    printData(input);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    searchPossibleMovesFromPosition(input, {x-1, y});
    searchPossibleMovesFromPosition(input, {x+1, y});
    searchPossibleMovesFromPosition(input, {x, y+1});
    searchPossibleMovesFromPosition(input, {x, y-1});
};

const auto findPosition = [](auto& input, char mark, char otherMark) -> Position{
    std::vector<Position> pos;
    bool firstParsed = false;
    for (auto i = 0u; i < input.size(); ++i) {
        if (pos.size() == 2) break;
        for (auto j = 0u; j < input[i].size(); ++j) {
            if (!firstParsed &&  input[i][j] == mark) {
                pos.push_back({i,j});
                firstParsed = true;
            }
            else if (firstParsed && input[i][j] == otherMark) {
                pos.push_back({i,j});
            }
            if (pos.size() == 2) break;
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
        /*
        * .
        * A
        * A
        */
        if (first.x > 1)
            if (input[first.x-1][first.y] == '.')
                return {first.x-1, first.y};

        /*
        * A
        * A
        * .
        */
        if (input[second.x+1][first.y] == '.')
            return {second.x+1, first.y};
    }
    assert(false);
    return {};
};

const auto getStartingPosition = [](auto& input) -> Position{
    return findPosition(input, 'A', 'A');
};

const auto getEndingPos = [](auto& input) -> Position{
    return findPosition(input, 'Z', 'Z');
};


const auto traverseFromPosition = [](auto& map, Position pos) -> std::list<Portal> {
    auto& x = pos.x;
    auto& y = pos.y;
    std::list<Portal> portals;

    return portals;
};

}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    auto input = loadData(path);
    //printData(input);
    auto startingPos = getStartingPosition(input);
    auto ending = getEndingPos(input);
    std::cout << startingPos.x << '-' << startingPos.y;
    std::cout << '\n';
    std::cout << ending.x << '-' << ending.y;
    std::cout << '\n';
    auto map = input;
    searchPossibleMovesFromPosition(map, startingPos);
    return 0;
}
