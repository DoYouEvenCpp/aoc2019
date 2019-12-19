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

	struct Test {
		int a;
		int b;
		int c;
		int d;

		bool operator==(Test const& o) const {
			return o.a == a && o.b == b && o.c == c && o.d == d;
		}
	};
struct MyHash
{
	std::size_t operator()(Test const& moon) const noexcept
	{
		//const std::size_t h1 = std::hash<int>{}(moon.p.x);
		//const std::size_t h2 = std::hash<int>{}(moon.p.y);
		//const std::size_t h3 = std::hash<int>{}(moon.p.z);
		//const std::size_t h4 = std::hash<int>{}(moon.p.x);
		//const std::size_t h5 = std::hash<int>{}(moon.p.y);
		//const std::size_t h6 = std::hash<int>{}(moon.p.z);
		//const auto h7 = std::hash<int>{}(moon.v.x);
		const auto h4 = std::hash<int>{}(moon.a);
		const auto h5 = std::hash<int>{}(moon.b);
		const auto h6 = std::hash<int>{}(moon.c);
		const auto h7 = std::hash<int>{}(moon.d);
		//return ((((h1 ^ (h2 << 1)) ^ (h3 << 2)) ^ (h4 << 3)) ^ (h5 << 4)) ^ (h6 << 5);
		//return (((h4 ^ (h5 << 1)) ^ (h6 << 2))) ^ (h7 << 3);
		return ((h4 ^ (h5 << 1)) ^ (h6 << 2)) ^ (h7 << 3);
	}
};

std::vector<Moon> input = {
	{15, - 2, -6},
	{-5, - 4, -11},
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
	for (auto i = 0; i < input.size(); ++i) {
		for (auto j = 0; j < input.size(); ++j) {
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

	for (auto i = 0; i < input.size(); ++i) {
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

}


int main() {
	
	/*input = {
		{-1, 0, 2},
		{2, -10, -7},
		{4, -8, 8},
		{3, 5, -1}
	};*/

	//input = {
	//	{-8, -10, 0},
	//	{5, 5, 10},
	//	{2, -7, 3},
	//	{9, -8, -3} 
	//};
	constexpr auto val = 100 * 100 * 100;
	int sum = 0;
	uint64_t counter = 1;
	std::unordered_set<Test, MyHash> moons;
	//moons.insert(input[3]);
	moons.insert({ input[0].p.y, input[1].p.y, input[2].p.y, input[3].p.y });
	while(true) {
		if ((counter % val) == 0) std::cout << counter << '\n';
		applyGravity(input);
		applyVelocity(input);
		counter++;
		const auto [_, isInserted] = moons.insert({ input[0].p.y, input[1].p.y, input[2].p.y, input[3].p.y });
		if (!isInserted)
		{
			std::cout << std::distance(_, moons.end()) << '\n';
			break;
		}
		//if (input[0].v.z == 0 && input[1].v.z == 0 && input[2].v.z == 0 && input[3].v.z == 0)
			//break;
	}


	//puzzle 2:
	// 163244813864492 -> too low
	// 653001901557465 -> too high
	std::cout << counter;
	return 0;
}