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
constexpr auto new_stack=  "deal into new stack";
constexpr auto cut = "cut";
constexpr auto increment = "deal with increment";

enum class task {
    stack,
    cut,
    inc
};

using data_type = std::vector<std::pair<task, int32_t>>;
using deck = std::vector<int32_t>;

const auto loadData = [](auto path){
    data_type res;
    std::fstream fs(path, std::fstream::in);
    std::string line;
    while (std::getline(fs, line)) {
        if (auto pos = line.find(new_stack); pos != std::string::npos) {
            res.push_back(std::make_pair(task::stack, 0));
        }
        else if (auto pos = line.find(cut); pos != std::string::npos) {
            res.push_back(std::make_pair(task::cut, std::stoi(line.substr(pos + 4))));
        }
        else if (auto pos = line.find(increment); pos != std::string::npos) {
            res.push_back(std::make_pair(task::inc, std::stoi(line.substr(pos + 20))));
        }
    }
    fs.close();
    return res;
};

const auto handleNewStack = [](deck& d) {
    std::reverse(d.begin(), d.end());
};

const auto handleCut = [](deck& d, const auto len) {
    if (len > 0) std::rotate(d.begin(), d.begin() + len, d.end());
    else std::rotate(d.rbegin(), d.rbegin() + std::abs(len), d.rend());
};

const auto handleIncrement = [](deck& d, const auto step) {
    deck res (d.size());
    std::size_t j = 0;
    for (auto i = 0u; i < d.size(); ++i) {
        res[j] = d[i];
        j += step;
        j %= d.size();
    }
    d = res;
};
}

int main(int argc, char** argv) {
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    const auto data = loadData(path);
    std::vector<int32_t> v(10007);
    std::iota(v.begin(), v.end(), 0);


    for (auto& e: data) {
        if (e.first == task::stack) {
            handleNewStack(v);
        }
        else if (e.first == task::cut) {
            handleCut(v, e.second);
        }
        else if (e.first == task::inc) {
            handleIncrement(v, e.second);
        }
    }

    std::cout << "First puzzle: " << std::distance(v.begin(), std::find(v.begin(), v.end(), 2019)) << '\n';
    return 0;
}