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


namespace {
enum class OpCodes {
    STOP = 99,
    ADD = 1,
    MULTIPLY = 2,
    STORE = 3,
    OUTPUT = 4
};

enum class ParamMode {
    Position = 0,
    Value = 1
};

std::ostream& operator<<(std::ostream& s, OpCodes code) {
    switch (code) {
        case OpCodes::STOP:     s << "STOP"; break;
        case OpCodes::ADD:      s << "ADD"; break;
        case OpCodes::MULTIPLY: s << "MULTIPLY"; break;
        case OpCodes::STORE:    s << "STORE"; break;
        case OpCodes::OUTPUT:   s << "OUTPUT"; break;
        default:                s << std::to_string(static_cast<int>(code)); break;
    }
    return s;
}


struct OpCode {
    OpCodes code;
    std::vector<int> param_modes;
};

auto SECOND_PUZZLE = 19690720;
std::vector input_org = {3,225,1,225,6,6,1100,1,238,225,104,0,1102,59,58,224,1001,224,-3422,224,4,224,102,8,223,223,101,3,224,224,1,224,223,223,1101,59,30,225,1101,53,84,224,101,-137,224,224,4,224,1002,223,8,223,101,3,224,224,1,223,224,223,1102,42,83,225,2,140,88,224,1001,224,-4891,224,4,224,1002,223,8,223,1001,224,5,224,1,223,224,223,1101,61,67,225,101,46,62,224,1001,224,-129,224,4,224,1002,223,8,223,101,5,224,224,1,223,224,223,1102,53,40,225,1001,35,35,224,1001,224,-94,224,4,224,102,8,223,223,101,6,224,224,1,223,224,223,1101,5,73,225,1002,191,52,224,1001,224,-1872,224,4,224,1002,223,8,223,1001,224,5,224,1,223,224,223,102,82,195,224,101,-738,224,224,4,224,1002,223,8,223,1001,224,2,224,1,224,223,223,1101,83,52,225,1101,36,77,225,1101,9,10,225,1,113,187,224,1001,224,-136,224,4,224,1002,223,8,223,101,2,224,224,1,224,223,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,1007,226,226,224,1002,223,2,223,1006,224,329,1001,223,1,223,1108,226,226,224,102,2,223,223,1006,224,344,101,1,223,223,1007,677,677,224,102,2,223,223,1006,224,359,101,1,223,223,1108,677,226,224,1002,223,2,223,1005,224,374,1001,223,1,223,7,677,226,224,102,2,223,223,1005,224,389,1001,223,1,223,1008,677,677,224,1002,223,2,223,1005,224,404,101,1,223,223,108,226,226,224,1002,223,2,223,1006,224,419,101,1,223,223,1008,226,677,224,1002,223,2,223,1006,224,434,1001,223,1,223,1107,677,226,224,1002,223,2,223,1005,224,449,101,1,223,223,1008,226,226,224,102,2,223,223,1005,224,464,1001,223,1,223,8,226,226,224,1002,223,2,223,1006,224,479,1001,223,1,223,107,226,677,224,102,2,223,223,1005,224,494,1001,223,1,223,7,226,226,224,102,2,223,223,1005,224,509,1001,223,1,223,107,226,226,224,102,2,223,223,1005,224,524,101,1,223,223,107,677,677,224,1002,223,2,223,1006,224,539,101,1,223,223,8,677,226,224,1002,223,2,223,1006,224,554,101,1,223,223,1107,677,677,224,1002,223,2,223,1005,224,569,101,1,223,223,108,226,677,224,1002,223,2,223,1006,224,584,101,1,223,223,7,226,677,224,1002,223,2,223,1005,224,599,1001,223,1,223,8,226,677,224,102,2,223,223,1006,224,614,1001,223,1,223,108,677,677,224,1002,223,2,223,1006,224,629,1001,223,1,223,1007,226,677,224,1002,223,2,223,1006,224,644,101,1,223,223,1108,226,677,224,102,2,223,223,1005,224,659,1001,223,1,223,1107,226,677,224,102,2,223,223,1006,224,674,1001,223,1,223,4,223,99,226};
//std::vector input_org = { 1002, 4,3,4,33 };




const auto printInput = [](auto& input) {
    for (auto e : input) {
        std::cout << e << ' ';
    }
    std::cout << '\n';
};

const auto convertToVector = [](int value) {
        const auto s = std::to_string(value);
        std::vector<int> res;
        for (auto e: s) {
            res.push_back(static_cast<int>(e - 48));
        }
        return res;
    };

const auto extractOpCodeAndParams = [](int value) {
    if (value == 3) return OpCode{OpCodes::STORE, std::vector<int>{0}};
    if (value == 4) return OpCode{OpCodes::OUTPUT, std::vector<int>{0}};
    if (value == 1) return OpCode{ OpCodes::ADD, std::vector<int>{0, 0, 0}};
    if (value == 2) return OpCode{ OpCodes::MULTIPLY, std::vector<int>{0, 0, 0} };
    const auto v = convertToVector(value);
    OpCode oc;
    auto code = v[v.size() - 2] * 10;
    code += v.back();
    if (!(code == 99 || code == 1 || code == 2 || code == 3 || code == 4)) {
        std::cout << "ALARM! " << code << " from : " << value << '\n';
    }
    oc.code = static_cast<OpCodes>(code);

    //iterate over every single parameter
    int j = 0;
    if (code == 1 || code == 2) oc.param_modes = {0, 0, 0};
    else if (code == 3 || code ==4 ) oc.param_modes = {0};
    for (int i = v.size() - 3; i >= 0; --i) {
        oc.param_modes[j++] = v[i];
    }

    return oc;
};

const auto parseOpCode = [](auto value) -> OpCode {
    if (static_cast<OpCodes>(value) == OpCodes::STOP)
        return {OpCodes::STOP, {}};
    else return extractOpCodeAndParams(value);
};

const auto runProgram = [](int param) {
    auto input = input_org;
    //auto opCode = 42;
    int index = 0u;
    //input[1] = noun;
    //input[2] = verb;
    for (auto i = 0u; i < input.size();) {
        auto code = parseOpCode(input[index]);
        auto opCode = code.code;

        if (opCode == OpCodes::STOP) {
            index += 1;
            break;
        }

        if (opCode == OpCodes::ADD) {
            const auto firstOperandPos = input[index + 1];
            const auto secondOperandPos = input[index + 2];
            const auto resultPos = input[index + 3];
            index += 4;
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandPos] : firstOperandPos;
            const auto secondParam = code.param_modes[1] == 0 ? input[secondOperandPos] : secondOperandPos;
            const auto res = firstParam + secondParam;
            input[resultPos] = res;
            //std::cout << "ADD: input[" << resultPos << "] = " << res << '\n';
        }
        else if (opCode == OpCodes::MULTIPLY) {
            const auto firstOperandPos = input[index + 1];
            const auto secondOperandPos = input[index + 2];
            const auto resultPos = input[index + 3];
            index += 4;
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandPos] : firstOperandPos;
            const auto secondParam = code.param_modes[1] == 0 ? input[secondOperandPos] : secondOperandPos;
            const auto res = firstParam * secondParam;

            input[resultPos] = res;
            //std::cout << "MULTIPLY: input[" << resultPos << "] = " << res << '\n';
        }
        else if (opCode == OpCodes::STORE) {
            const auto firstOperandPos = input[index + 1];
            const auto pos = firstOperandPos;
            input[pos] = param;
            index += 2;
            //std::cout << "STORE: input[" << pos << "] = " << param << '\n';
        }
        else if (opCode == OpCodes::OUTPUT) {
            const auto firstOperandPos = input[input[index + 1]];
            index += 2;
            std::cout << '>' << firstOperandPos << "<\n";
        }
        else {
            index += 1;
            std::cout << "ZONK - " << opCode << " on index: " << index << "\n";
        }
        i = index;
    }
    return input[0];
};
}

int main()
{
    auto f = runProgram(1);
    auto s = 0;

    std::cout << "\n\nfirst: " << f << " second: " << s << '\n';
    return 0;
}
