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
using level = int32_t;
using world = std::map<level, data_type>;

const auto getEmptyGrid = []{
    std::vector<std::vector<char>> g(5, std::vector<char>(5, '.'));
    g[2][2] = '?';
    return g;
};

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


const auto getBugsNumberInTilesInner = [](data_type& current, data_type& adjacent, auto x, auto y) {
    int sum = 0;
    //this gonna be gut.
    //8  == 2,1
    //12 == 1,2
    //18 == 2,3
    //14 == 3,2

    if (x == 0 && y == 0){
        sum += visitTile(adjacent, 2, 1);   //N
        sum += visitTile(current, x, y+1);  //S
        sum += visitTile(adjacent, 1, 2);   //W
        sum += visitTile(current, x+1, y);  //E
    }
    else if (x == 0 && y == 4){
        sum += visitTile(current, x, y-1);  //N
        sum += visitTile(adjacent, 2, 3);   //S
        sum += visitTile(adjacent, 1,2);    //W
        sum += visitTile(current, x+1,y);   //E
    }
    else if (x == 4 && y == 0) {
        sum += visitTile(adjacent, 2,1);    //N
        sum += visitTile(current, x , y+1); //S
        sum += visitTile(current, x-1, y);  //W
        sum += visitTile(adjacent, 3,2);    //E
    }
    else if (x == 4 && y == 4) {
        sum += visitTile(current, x, y-1);
        sum += visitTile(adjacent, 2,3);
        sum += visitTile(current, x-1, y);
        sum += visitTile(adjacent, 3,2);
    }
    else if ((x == 0 && y == 1) || (x == 0 && y == 2) || (x == 0 && y == 3)) {
        sum += visitTile(current, x, y-1);  //N
        sum += visitTile(current, x, y+1);  //S
        sum += visitTile(adjacent, 1, 2);   //W
        sum += visitTile(current, x+1, y);  //E
    }
    else if ((x == 4 && y == 1) || (x == 4 && y == 2) || (x == 4 && y == 3)) {
        sum += visitTile(current, x, y-1);  //N
        sum += visitTile(current, x, y+1);  //S
        sum += visitTile(current, x-1, y);  //W
        sum += visitTile(adjacent, 3, 2);   //E
    }
    else if ((x == 1 && y == 0) || (x == 2 && y == 0) || (x == 3 && y == 0)) {
        sum += visitTile(adjacent, 2, 1);   //N
        sum += visitTile(current, x, y+1);  //S
        sum += visitTile(current, x-1, y);  //W
        sum += visitTile(current, x+1, y);  //E
    }
    else if ((x == 1 && y == 4) || (x == 2 && y == 4) || (x == 3 && y == 4)) {
        sum += visitTile(current, x, y-1);  //N
        sum += visitTile(adjacent, 2, 3);   //S
        sum += visitTile(current, x-1, y);  //W
        sum += visitTile(current, x+1, y);  //E
    }

    return sum;
};

const auto getBugsNumberInTilesOutter = [](data_type& current, data_type& outter, auto x, auto y) {
    int sum = 0;
    //this gonna be gut.
    //8  == 2,1
    //12 == 1,2
    //18 == 2,3
    //14 == 3,2

    if (x == 2 && y == 1){ //8
        sum += visitTile(current, x, y-1);
        sum += visitTile(current, x-1, y);
        sum += visitTile(current, x+1, y);
        for (auto i = 0; i < 5; ++i) {
            sum += visitTile(outter, i, 0);
        }

    }
    else if (x == 2 && y == 3){ //18
        sum += visitTile(current, x, y+1);
        sum += visitTile(current, x+1, y);
        sum += visitTile(current, x-1, y);
        for (auto i = 0; i < 5; ++i) {
            sum += visitTile(outter, i, 4);
        }
    }
    else if (x == 1 && y == 2){ //12
        sum += visitTile(current, x, y-1);
        sum += visitTile(current, x, y+1);
        sum += visitTile(current, x-1, y);
        for (auto i = 0; i < 5; ++i) {
            sum += visitTile(outter, 0, i);
        }
    }

    else if (x == 3 && y == 2){ //14
        sum += visitTile(current, x, y-1);
        sum += visitTile(current, x, y+1);
        sum += visitTile(current, x+1, y);
        for (auto i = 0; i < 5; ++i) {
            sum += visitTile(outter, 4, i);
        }
    }

    return sum;
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

const auto calculateTotalNumberOfBugs = [](world& w){
    auto sum = 0;
    for (auto& e: w){
        for (auto& line: e.second) {
            for (auto e: line) {
                sum += e == '#';
            }
        }
    }
    return sum;
};

void iterateWorld(world& w) {
    auto copy = w;

    for (auto lvl = -100; lvl <= 100; ++lvl){
        auto& d = w[lvl];
        auto tmp = d;
        for (auto i = 0u; i < d.size(); ++i) {
            for (auto j = 0u; j < d[0].size(); ++j) {
                if (i == 0 || i == d.size() || j == 0 || j == d[0].size()) {
                    if (not w.count(lvl-1)) {
                        w[lvl-1] = getEmptyGrid();
                    }
                    auto bugs = getBugsNumberInTilesInner(d, w[lvl-1], i, j);

                    if (d[i][j] == '#'){
                        if (bugs != 1) tmp[i][j] = '.';
                    }
                    else if (d[i][j] == '.') {
                        if (bugs == 1 || bugs == 2) tmp[i][j] = '#';
                    }
                }
                else if ((i == 1 && j == 2) || (i == 2 && j == 1) || (i == 2 && j == 3) || (i == 3 && j == 2)) {
                    if (not w.count(lvl+1))
                        w[lvl+1] = getEmptyGrid();
                    auto bugs = getBugsNumberInTilesOutter(d, w[lvl+1], i, j);
                    if (d[i][j] == '#'){
                        if (bugs != 1) tmp[i][j] = '.';
                    }
                    else if (d[i][j] == '.') {
                        if (bugs == 1 || bugs == 2) tmp[i][j] = '#';
                    }
                }
                else {
                    const auto bugs = getBugsNumberInAdjacentTiles(d, i, j);
                    if (d[i][j] == '#'){
                        if (bugs != 1) tmp[i][j] = '.';
                    }
                    else if (d[i][j] == '.') {
                        if (bugs == 1 || bugs == 2) tmp[i][j] = '#';
                    }
                }
            }
        }
        copy[lvl] = tmp;
    }
    w = copy;
}
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "too few parameters!\n";
        return -1;
    }
    auto input = loadData(argv[1]);
    auto data = input;
    std::set<std::string> layouts;

    do {
        iterate(data);
        if (auto [_, isInserted] = layouts.insert(dumpToString(data)); !isInserted) {
            print(data);
            std::cout << "First puzzle answer: " << calculateBiodiversity(data) << '\n';
            break;
        }
    } while(true);

    world w;
    input[2][2] = '?';
    for (auto i = -100; i <= 100; ++i) {
        w[i] = getEmptyGrid();
        w[i][2][2] = '?';
    }
    w[0] = input;
    for (auto i = 0; i < std::stoi(argv[2]); ++i) {
        iterateWorld(w);

        // for (auto i = -5; i <= 5; ++i){
        //     std::cout << "Depth: " << i << '\n';
        //     print(w[i]);
        //     std::cout << '\n';
        // }
        // getchar();

    }

    for (auto i = -5; i <= 5; ++i){
        std::cout << "Depth: " << i << '\n';
        print(w[i]);
        std::cout << '\n';
    }
    std::cout << "Second puzzle asnwer: " << calculateTotalNumberOfBugs(w) << '\n';

    //2021 too low
    return 0;
}