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
#include <set>


namespace {

using DataType = std::vector<std::vector<char>>;
const auto loadData = [](auto path){
    DataType res;
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

//THAT IS SHITTY.
auto firstPuzzleX = 0;
auto firstPuzzleY = 0;


const auto getVisibleAsteroidsCount = [](int ii, int jj, DataType& map) {
    std::set<float> angles;
    for (auto i = 0; i < static_cast<int>(map.size()); ++i) {
        for (auto j = 0; j < static_cast<int>(map[i].size()); ++j) {
            if (map[i][j] == '#') {
                if (i == ii && j == jj) continue;
                const auto xx = ii - i;
                const auto yy = jj - j;
                const auto v = std::atan2(yy, xx);
                angles.insert(v);
            }
        }
    }
    return std::make_tuple(angles.size(), ii, jj);
};

const auto getManhattanDistance = [](auto x, auto y, auto x1, auto y1) {
    return std::abs(x - x1) + std::abs(y - y1);
};

struct Asteroid {
    int x;
    int y;

    bool operator<(Asteroid const& lhs) const noexcept {
        const auto myDistance = getManhattanDistance(x, y, firstPuzzleY, firstPuzzleY);
        const auto lhsDistance = getManhattanDistance(lhs.x, lhs.y, firstPuzzleY, firstPuzzleY);
        return lhsDistance < myDistance;
    }
};

constexpr auto PI = 3.14159265359f;

const auto getAngle = [](auto xx, auto yy) {
    auto deg = (180*std::atan2(yy, xx)) / PI;

    if (deg <= 90 && deg >= 0) {
        deg = std::abs(deg - 90);
    }
    else if (deg < 0) {
        deg = std::abs(deg) + 90;
    }
    else {
        deg = 450 - deg;
    }
    return deg;
};
const auto calculateAngles = [](int ii, int jj, DataType& input) {
    std::map<float, std::vector<Asteroid>> angles;
    auto map = input;

    for (auto i = 0; i < static_cast<int>(map.size()); ++i) {
        for (auto j = 0; j < static_cast<int>(map[i].size()); ++j) {
            if (map[i][j] == '#') {
                if (i == ii && j == jj) {
                    continue;
                }
                const auto yy = ii - i;
                const auto xx = jj - j;
                const auto v = getAngle(yy, xx);
                angles[v].push_back({j, i});
            }
        }
    }
    return angles;
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
    auto sum = 0u;
    auto ii = 0;
    auto jj = 0;
    for (auto i = 0u; i < input.size(); ++i) {
        for (auto j = 0u; j < input[i].size(); ++j) {
            if (input[i][j] == '#') {
                const auto [temp, i_, j_] = getVisibleAsteroidsCount(i,j, input);
                if (temp > sum){
                    sum = temp;
                    ii = i_;
                    jj = j_;
                }
            }
        }
    }
    std::cout <<"First puzzle answer: " << sum << ' ' << ii << ' ' << jj << '\n';
    firstPuzzleX = jj;
    firstPuzzleY = ii;

    auto m = calculateAngles(ii, jj, input);
    //for testing
    //auto m = calculateAngles(3, 8, input);

    std::vector<float> angles;
    for (auto& e: m) {
        angles.push_back(e.first);
        std::sort(e.second.begin(), e.second.end(), [](Asteroid const& lhs, Asteroid const& rhs){
            return getManhattanDistance(lhs.x, lhs.y, firstPuzzleX, firstPuzzleY) <
            getManhattanDistance(rhs.x, rhs.y, firstPuzzleX, firstPuzzleY);
        });
    }
    auto currentAngle = std::upper_bound(angles.begin(), angles.end(), 89.99);
    std::vector<Asteroid> burned_asteroids;

    while(!m.empty()) {
        if (m.count(*currentAngle)) {
            if (m[*currentAngle].empty()) m.erase(*currentAngle);
            else {
                burned_asteroids.push_back(m[*currentAngle].front());
                m[*currentAngle].erase(m[*currentAngle].begin());
            }
        }
        if (currentAngle == angles.end()) {
            currentAngle = angles.begin();
        }
        else {
            ++currentAngle;
        }
    }
    if (burned_asteroids.size() > 199) {
        auto& asteroid = burned_asteroids[199];
        std::cout <<"Second puzzle answer: " <<  asteroid.x * 100 + asteroid.y << '\n';
    }

    return 0;
}
