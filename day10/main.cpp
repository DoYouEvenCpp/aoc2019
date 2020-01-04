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


const auto getVisibleAsteroidsCount = [](int ii, int jj, DataType& map) {
    std::set<float> angles;
    //TODO: i'm too tired to analyze that right now
    //i & j & ii && jj shall be unsigned ints( i.e. std:size_t)
    for (auto i = 0; i < static_cast<int>(map.size()); ++i) {
        for (auto j = 0; j < static_cast<int>(map[i].size()); ++j) {
            if (map[i][j] == '#') {
                if (i == ii && j == jj) continue;
                const auto xx = ii - i;
                const auto yy = jj - j;
                const auto v = std::atan2(yy, xx);
                //std::cout << (v * 180) /3.14 <<' ' << j << ',' << i << '\n';
                angles.insert(v);
            }
        }
    }
    return std::make_tuple(angles.size(), ii, jj);
};


struct Asteroid {
    int x;
    int y;
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
    //std::vector<std::vector<float>> angles;
    std::vector<Asteroid> burned_asteroids;
    std::map<int, std::vector<Asteroid>> angles;
    auto map = input;

    for (auto i = 0; i < static_cast<int>(map.size()); ++i) {
        //angles.push_back({});
        for (auto j = 0; j < static_cast<int>(map[i].size()); ++j) {
            if (map[i][j] == '#') {
                if (i == ii && j == jj) {
                    //angles.back().push_back(std::numeric_limits<float>::min());
                    continue;
                }
                const auto yy = ii - (i);
                const auto xx = jj - (j);
                const auto v = getAngle(yy, xx);// (std::atan2(yy, xx);
                angles[v].push_back({i,j});
                //std::cout << v << ' ' << i - ii << ',' << j - jj << '\n';
                //angles.back().push_back(v);
                //map[i][j] = 'S';
                //std::this_thread::sleep_for(std::chrono::milliseconds(200));
                //printData(map);
                //std::cout << '\n' << '\n' << '\n' << '\n';
                //std::getchar();
                //map[i][j] = '#';
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


    //calculateAngles(ii, jj, input);
    auto m = calculateAngles(3, 8, input);

    return 0;
}
