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


struct Node{
    char element;
    std::size_t range;
};

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

std::size_t searchPossibleMovesFromPosition(std::vector<std::vector<char>>& input, Position pos, std::size_t range) {
    const auto x = pos.x;
    const auto y = pos.y;
    if (x >= input.size()) return 0;
    if (y >= input[x].size()) return 0;
    if (input[x][y] == '@')
        return 0;
    if (input[x][y] == '#' || input[x][y] == ' '){
        //std::cout << "Hit: " << input[x][y] << " on (" << x << ", " << y << ")\n";
        return 0;
    }
    if (input[x][y] == 'A') {
        //std::cout << "Hit: A on (" << x << ", " << y << ")\n";
        return 0;
    }
    if (input[x][y] >= 'B' && input[x][y] <= 'Z') {
        //std::cout << "Hit: " << input[x][y] << " on (" << x << ", " << y << ")\n";
        nodes.push_back({input[x][y], range-1});
        return 0;
    }

    if (input[x][y] == '.') {
        //std::cout << "Hit: . on (" << x << ", " << y << ")\n";
        input[x][y] = '@';
        range++;
    }

    //printData(input);
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto sum = 0;
    sum += searchPossibleMovesFromPosition(input, {x-1, y}, range);
    sum += searchPossibleMovesFromPosition(input, {x+1, y}, range);
    sum += searchPossibleMovesFromPosition(input, {x, y+1}, range);
    sum += searchPossibleMovesFromPosition(input, {x, y-1}, range);
    return range + sum -1;
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
            if ((input[i][j] >= 'A' && input[i][j] <= 'Z') && (input[i][j + 1] >= 'A' && input[i][j + 1] <= 'Z'))
                std::cout << input[i][j] << '-' << input[i][j + 1] << '\n';
        }
    }

    const auto width = input[0].size();
    for (auto i = 0; i < width; ++i) {
        for (auto j = 0; j < input.size() - 1; ++j) {
            if ((input[j][i] >= 'A' && input[j][i] <= 'Z') && (input[j+1][i] >= 'A' && input[j+1][i] <= 'Z'))
                std::cout << input[j][i] << '-' << input[j+1][i] << '\n';
        }
    }
};
}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    //const std::string path = argv[1];
    const std::string path = "C:\\Users\\staho\\projects\\aoc2019\\_sln\\ConsoleApplication1\\x64\\Debug\\input";
    auto input = loadData(path);
    printData(input);
    auto startingPos = getStartingPosition(input);
    auto ending = getEndingPos(input);
    std::cout << startingPos.x << '-' << startingPos.y;
    std::cout << '\n';
    std::cout << ending.x << '-' << ending.y;
    std::cout << '\n';
    auto map = input;
    //searchPossibleMovesFromPosition(map, startingPos);
    map = input;
    //auto dd = findPosition(map, 'T', 'Y');
    std::size_t sum = 0;
    searchPossibleMovesFromPosition(map, startingPos, sum);
    std::cout << "Sum: " << sum << std::endl;
    for (auto& e: nodes){
        std::cout << e.element << '-' << e.range << '\n';
    }
    std::cout << '\n';
    std::cout << '\n';
    searchPortals(input);
    return 0;
}
