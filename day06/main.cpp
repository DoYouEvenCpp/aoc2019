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

const auto loadData = [](auto path){
    std::unordered_map<std::string, std::vector<std::string>> res;
    {
        char ch;
        std::string name;
        std::string value;
        bool parsingName = true;
        std::fstream fs(path, std::fstream::in);
        while (fs.get(ch)) {
            if (ch == ')') {
                parsingName = false;
                continue;
            }
            if (ch == '\n') {
                if (res.count(name) == 0) {
                    res[name] = {value};
                }
                else {
                    res[name].push_back(value);
                }
                parsingName = true;
                name = {};
                value = {};
                continue;

            }
            if (parsingName)
                name += ch;
            else
                value += ch;
        }
        fs.close();
    }
    return res;
};
const auto printData = [](auto& data) {
    for (auto& node: data) {
        std::cout << node.first << " -> ";
        for (auto& e: node.second) {
            std::cout << e << ' ';
        }
        std::cout << '\n';
    }
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
    printData(input);
    return 0;
}
