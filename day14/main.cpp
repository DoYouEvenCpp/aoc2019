#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_map>
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
#include <sstream>


namespace {

struct Component {
    std::string name;
    int quantity;
};

using DataType = std::unordered_map<std::string, std::vector<Component>>;

const auto loadData = [](auto path){
    DataType res;
    std::fstream fs(path, std::fstream::in);

    std::string line;

    while (std::getline(fs, line)) {
        const auto productStartPos = line.find(" => ") + 4;
        const auto product = line.substr(productStartPos);
        auto ingredients = std::istringstream(line.substr(0, productStartPos - 4));

        std::vector<Component> tmp;
        Component comp;
        std::string token;
        while (std::getline(ingredients, token, ',')) {
            if (token[0] == ' ') token.erase(0, 1);
            const auto numberPos = token.find(' ');
            auto number = std::stoi(token.substr(0, numberPos));
            auto name = token.substr(numberPos + 1);
            tmp.push_back({name, number});
        }
        res[product] = tmp;
        tmp = {};
    }

    fs.close();
    return res;
};

const auto printData = [](auto& data) {
    for (auto& line: data) {
        for (auto ch: line)
            std::cout << ch;
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
    for (auto& e: input) {
        std::cout << e.first << " => ";
        for (auto& ee: e.second) {
            std::cout << ee.quantity << '*' << ee.name << ',';
        }
        std::cout << '\n';
    }
    return 0;
}
