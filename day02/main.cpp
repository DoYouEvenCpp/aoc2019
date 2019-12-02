#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <map>
#include <array>
#include <cmath>

namespace {
	auto STOP = 99;
	auto ADD = 1;
	auto MULTIPLY = 2;
	auto SECOND_PUZZLE = 19690720;
	std::vector input_org = { 1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,1,10,19,1,6,19,23,1,10,23,27,2,27,13,31,1,31,6,35,2,6,35,39,1,39,5,43,1,6,43,47,2,6,47,51,1,51,5,55,2,55,9,59,1,6,59,63,1,9,63,67,1,67,10,71,2,9,71,75,1,6,75,79,1,5,79,83,2,83,10,87,1,87,5,91,1,91,9,95,1,6,95,99,2,99,10,103,1,103,5,107,2,107,6,111,1,111,5,115,1,9,115,119,2,119,10,123,1,6,123,127,2,13,127,131,1,131,6,135,1,135,10,139,1,13,139,143,1,143,13,147,1,5,147,151,1,151,2,155,1,155,5,0,99,2,0,14,0 };


	const auto printInput = [](auto& input) {
		for (auto e : input) {
		std::cout << e << ' ';
	}
	std::cout << '\n';
	};


	const auto runProgram = [](int noun, int verb) {
		auto input = input_org;
		int opCode = 42;
		int index = 0u;
		input[1] = noun;
		input[2] = verb;
		for (auto i = 0u; i < input.size(); i += 4) {
			index = i;
			opCode = input[index];
			if (opCode == STOP) {
				break;
			}

			const auto firstOperandPos = input[index + 1];
			const auto secondOperandPos = input[index + 2];
			const auto resultPos = input[index + 3];

			if (opCode == ADD) {
				const auto res = input[firstOperandPos] + input[secondOperandPos];
				input[resultPos] = res;
			}
			else if (opCode == MULTIPLY) {
				const auto res = input[firstOperandPos] * input[secondOperandPos];
				input[resultPos] = res;
			}
			else {
				std::cout << "ZONK - " << opCode << "\n";
			}
		}
		return input[0];
	};
}

int main()
{
	auto f = runProgram(12, 2);
	auto s = 0;
	bool stop = false;
	for (int i = 0; i <= 99; ++i) {
		if (stop) break;
		for (int j = 0; j <= 99; ++j) {
			if (runProgram(i, j) == SECOND_PUZZLE) {
				s = 100 * i + j;
				std::cout << i << " - " << j << '\n';
				stop = true;
				break;
			}
		}
	}
	std::cout << "first: " << f << " second: " << s << '\n';
	return 0;
}
