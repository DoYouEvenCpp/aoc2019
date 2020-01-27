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


namespace{
constexpr auto new_stack=  "deal into new stack";
constexpr auto cut = "cut";
constexpr auto increment = "deal with increment";

enum class task {
    stack,
    cut,
    inc
};

using data_type = std::vector<std::pair<task, int>>;

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
}

int main(int argc, char** argv) {
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    const auto data = loadData(path);
}