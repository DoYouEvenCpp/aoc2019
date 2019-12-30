#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <map>
#include <array>
#include <cmath>
#include <tuple>
#include <cassert>
#include <optional>
#include <unordered_set>
#include <numeric>

namespace {
struct Pos {
    int x;
    int y;
    int z;
};
struct Velocity {
    int x{ 0 };
    int y{ 0 };
    int z{ 0 };
};
struct Moon {
    Pos p;
    Velocity v;
    bool operator==(Moon const& o) const {
        return o.p.x == p.x && o.p.y == p.y && o.p.z == p.z
            && o.v.x == v.x && o.v.y == v.y && o.v.z == v.z;
    }
};

std::vector<Moon> input = {
    {15, -2, -6},
    {-5, -4, -11},
    {0, -6, 0},
    {5, 9, 6},

};

const auto print = [](std::vector<Moon>& input) {
    for (auto& moon : input) {
        std::cout << "Pos:(" << moon.p.x << ", " << moon.p.y << ", " << moon.p.z << ")"
            << "\tVel:(" << moon.v.x << ", " << moon.v.y << ", " << moon.v.z << ")\n";
    }
};

const auto getKineticEnery = [](Moon const& m) {
    return std::abs(m.v.x) + std::abs(m.v.y) + std::abs(m.v.z);
};

const auto getPotentialEnery = [](Moon const& m) {
    return std::abs(m.p.x) + std::abs(m.p.y) + std::abs(m.p.z);
};

const auto getMoonTotalEnergy = [](Moon const& m) {
    return getKineticEnery(m) * getPotentialEnery(m);
};

const auto applyGravity= [](std::vector<Moon>& input) {
    std::vector<Moon> updated_values{ {}, {}, {},{} };
    for (auto i = 0u; i < input.size(); ++i) {
        for (auto j = 0u; j < input.size(); ++j) {
            if (j == i) continue;

            auto& first = input[i];
            auto& second = input[j];

            if (first.p.x != second.p.x) {
                updated_values[i].v.x += first.p.x < second.p.x ? 1 : -1;
            }

            if (first.p.y != second.p.y) {
                updated_values[i].v.y += first.p.y < second.p.y ? 1 : -1;
            }

            if (first.p.z != second.p.z) {
                updated_values[i].v.z += first.p.z < second.p.z ? 1 : -1;
            }
        }
    }

    for (auto i = 0u; i < input.size(); ++i) {
        input[i].v.x += updated_values[i].v.x;
        input[i].v.y += updated_values[i].v.y;
        input[i].v.z += updated_values[i].v.z;
    }
};

const auto applyVelocity = [](std::vector<Moon>& input) {
    for (auto& moon : input) {
        moon.p.x += moon.v.x;
        moon.p.y += moon.v.y;
        moon.p.z += moon.v.z;
    }
};

const auto getMoon = [](auto & input, char axis) -> Moon {
    if (axis == 'x') {
        return { input[0].p.x, input[1].p.x, input[2].p.x, input[3].p.x };
    }
    else if (axis == 'y') {
        return { input[0].p.y, input[1].p.y, input[2].p.y, input[3].p.y };
    }
    else {
        return { input[0].p.z, input[1].p.z, input[2].p.z, input[3].p.z };
    }
};

}

int main() {

    int sum = 0;
    const auto copy_input = input;
    for (auto i = 0u; i < 1000; ++i) {
        applyGravity(input);
        applyVelocity(input);
    }
    for (auto& moon : input)
        sum += getMoonTotalEnergy(moon);
    std::cout << "First puzzle answer: " << sum << '\n';
    input = copy_input;

    uint64_t counter = 1;
    Moon x({ input[0].p.x, input[1].p.x, input[2].p.x, input[3].p.x });
    Moon y({ input[0].p.y, input[1].p.y, input[2].p.y, input[3].p.y });
    Moon z({ input[0].p.z, input[1].p.z, input[2].p.z, input[3].p.z });
    uint64_t x_counter = 0;
    uint64_t y_counter = 0;
    uint64_t z_counter = 0;
    while (x_counter == 0 || y_counter == 0 || z_counter == 0) {
        applyGravity(input);
        applyVelocity(input);
        counter++;
        if (x_counter == 0 && x == getMoon(input, 'x')) x_counter = counter;
        if (y_counter == 0 && y == getMoon(input, 'y')) y_counter = counter;
        if (z_counter == 0 && z == getMoon(input, 'z')) z_counter = counter;
    }

    std::cout << "Second puzzle answer: " << std::lcm(x_counter, std::lcm(y_counter, z_counter)) << '\n';
    return 0;
}