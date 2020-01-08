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


std::map<std::string, int> values;

void getValue(DataType& map, std::string val, int count){
    auto it = std::find_if(map.begin(), map.end(), [val](DataType::value_type& p) {
            return p.first.name == val;});

    if (it != map.end()) {
        const auto base = std::ceil(count / static_cast<double>(it->first.quantity));
        if (it->second.size() == 1 && it->second[0].name == "ORE") {
            std::cout << "ORE: " << it->second[0].quantity << " * " <<  count <<  " base: " << base;
            values[val] += count;
        }
        else {
            for (auto& entry : it->second) {
                std::cout << "Going into " << entry.name << ", with " << entry.quantity << " * " << base << " -> ";
                getValue(map, entry.name, entry.quantity * base);
                std::cout << '\n';
            }
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

    const std::string path = argv[1];
    auto input = loadData(path);

    //printData(input);
    getValue(input, "FUEL", 1);

    auto sum = 0;
    for (auto& entry: values) {
        auto it = std::find_if(input.begin(), input.end(), [&entry](DataType::value_type& p) {return p.first.name == entry.first;});
        const auto count = std::ceil(entry.second / static_cast<double>(it->first.quantity));
        sum += it->second[0].quantity * count;
        std::cout << entry.first << '[' << it->first.quantity << "]: " << entry.second  << "-> " << it->second[0].quantity << " * " << count << std::endl;
    }
    std::cout << '\n' << '\n' << sum << '\n';
    //451849 too high
    return 0;
}
