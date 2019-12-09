#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <map>
#include <array>
#include <cmath>

using namespace std;
namespace {
    constexpr auto input_begin = 235741;
    constexpr auto input_end = 706948;

    const auto convertToVector = [](int value) {
        const auto s = std::to_string(value);
        std::vector<int> res;
        for (auto e: s) {
            res.push_back(static_cast<int>(e - 48));
        }
        return res;
    };

    const auto isMonotonic = [](std::vector<int> d) {
        return std::is_sorted(d.begin(), d.end()) && std::adjacent_find(d.begin(), d.end()) != d.end();
    };

    const auto checkMonotonic = [](std::vector<int> d) {
        if (!std::is_sorted(d.begin(), d.end())) return false;
        return std::any_of(d.begin(), d.end(), [&](auto v){return std::count(d.cbegin(), d.cend(), v) == 2;});
    };
}

int main()
{
    int sum = 0;
    int sum_puzzle2 = 0;
    for(auto i = input_begin; i < input_end; ++i) {
        sum += isMonotonic(convertToVector(i));
        sum_puzzle2 += checkMonotonic(convertToVector(i));
    }
    std::cout << "First: " << sum << " Second: " << sum_puzzle2 << std::endl;
    return 0;
}
