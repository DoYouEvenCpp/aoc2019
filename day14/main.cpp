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

using DataType = std::vector<std::pair<Component, std::vector<Component>>>;

const auto loadData = [](auto path){
    DataType res;
    std::fstream fs(path, std::fstream::in);

    std::string line;

    while (std::getline(fs, line)) {
        const auto productStartPos = line.find(" => ") + 4;
        auto product = std::istringstream(line.substr(productStartPos));
        auto ingredients = std::istringstream(line.substr(0, productStartPos - 4));

        std::vector<Component> tmp;
        Component comp;
        std::string token;

        product >> comp.quantity >> comp.name;

        while (std::getline(ingredients, token, ',')) {
            if (token[0] == ' ') token.erase(0, 1);
            const auto numberPos = token.find(' ');
            auto number = std::stoi(token.substr(0, numberPos));
            auto name = token.substr(numberPos + 1);
            tmp.push_back({name, number});
        }
        res.push_back(std::make_pair(comp, tmp));
        tmp = {};
    }

    fs.close();
    return res;
};

const auto printData = [](auto& input) {
    for (auto& e: input) {
        std::cout << e.first.name << '[' << e.first.quantity << "] => ";
        for (auto& ee: e.second) {
            std::cout << ee.quantity << '*' << ee.name << ',';
        }
        std::cout << '\n';
    }
};

std::map<std::string, int64_t> values;
std::map<std::string, int64_t> leftovers;

void calculateSingleIngredient(DataType& map, std::string name, int64_t count) {
    auto it = std::find_if(map.begin(), map.end(), [name](DataType::value_type& p) {
            return p.first.name == name;});

    if (it != map.end()) {
        if (it->second.size() == 1 && it->second[0].name == "ORE") {
            const auto finalCount = count;
            values[name] += finalCount;
        }
        else {
            auto& leftover = leftovers[name];
            count -= leftover;
            int64_t needed = (count + it->first.quantity  - 1) / it->first.quantity;
            leftover = needed * it->first.quantity - count;
            for (auto& entry : it->second) {
                calculateSingleIngredient(map, entry.name, needed * entry.quantity);
            }
        }
    }
};

int64_t getOreNeeded(DataType& input, int64_t counter) {
    calculateSingleIngredient(input, "FUEL", counter);
    //std::cout << '\n';

    for (auto& entry: leftovers) {
        values[entry.first] -= entry.second;
    }
    int64_t sum = 0;
    for (auto& entry: values) {
        auto it = std::find_if(input.begin(), input.end(), [&entry](DataType::value_type& p) {return p.first.name == entry.first;});
        const auto count = std::ceil(entry.second / static_cast<double>(it->first.quantity));
        sum += it->second[0].quantity * count;
    }
    //std::cout << "\n\nSum: " << sum << '\n';
    return sum;
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
    std::cout << "First puzzle answer: " << getOreNeeded(input, 1) << '\n';

    int64_t low = 1;
    int64_t high = 1000000000000;
    while (true) {
        leftovers.clear();
        values.clear();
        const int64_t medium = (low + high) / 2;
        const auto res = getOreNeeded(input, medium);

        if (res > 1000000000000)
            high = medium;
        else
            low = medium;

        if ((high - low) == 1) {
                std::cout << "Second puzzle answer: " << medium - 1 << '\n';
                break;
        }
    }
    return 0;
}
