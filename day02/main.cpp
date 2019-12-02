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


	const auto printInput = [](auto& input) {
		for (auto e : input) {
		std::cout << e << ' ';
	}
	std::cout << '\n';
	};
}

int main()
{
	std::vector input = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,1,10,19,1,6,19,23,1,10,23,27,2,27,13,31,1,31,6,35,2,6,35,39,1,39,5,43,1,6,43,47,2,6,47,51,1,51,5,55,2,55,9,59,1,6,59,63,1,9,63,67,1,67,10,71,2,9,71,75,1,6,75,79,1,5,79,83,2,83,10,87,1,87,5,91,1,91,9,95,1,6,95,99,2,99,10,103,1,103,5,107,2,107,6,111,1,111,5,115,1,9,115,119,2,119,10,123,1,6,123,127,2,13,127,131,1,131,6,135,1,135,10,139,1,13,139,143,1,143,13,147,1,5,147,151,1,151,2,155,1,155,5,0,99,2,0,14,0};
	//std::vector testInput = {1,9,10,3,2,3,11,0,99,30,40,50};
	auto f = 0u;
	auto s = 0u;

	int opCode = 42;
	int index = 0u;
	//auto& input = testInput;

	for (auto i = 0u; i < input.size(); i += 4) {
		index = i;
		opCode = input[index];
		if (opCode == STOP) {
			std::cout << "STOP\n";
			break;
		}

		const auto firstOperandPos = input[index + 1];
		const auto secondOperandPos = input[index + 2];
		const auto resultPos = input[index + 3];

		//std::cout << "resultPos : " << resultPos << " 1st : " << firstOperandPos << " 2nd: " << secondOperandPos << '\n';
		if (opCode == ADD) {
			input[resultPos] = input[firstOperandPos] + input[secondOperandPos];
			//std::cout << input[firstOperandPos] << " + " << input[secondOperandPos] << '\n';
		}
		else if (opCode == MULTIPLY) {
			input[resultPos] = input[firstOperandPos] * input[secondOperandPos];
			//std::cout << input[firstOperandPos] << " * " << input[secondOperandPos] << '\n';
		}
		else {
			std::cout << "ZONK - " << opCode << "\n";
		}
		//printInput(input);
	}

	printInput(input);
	f = input[0];
	//797908 too low
	std::cout << "first: " << f << " second: " << s << '\n';
	return 0;
}
