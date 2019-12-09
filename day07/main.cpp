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
    OUTPUT = 4,
    JUMP_TRUE = 5,
    JUMP_FALSE = 6,
    LESS_THAN = 7,
    EQUALS = 8
};

enum class ParamMode {
    Position = 0,
    Value = 1
};

std::ostream& operator<<(std::ostream& s, OpCodes code) {
    switch (code) {
        case OpCodes::STOP:         s << "STOP"; break;
        case OpCodes::ADD:          s << "ADD"; break;
        case OpCodes::MULTIPLY:     s << "MULTIPLY"; break;
        case OpCodes::STORE:        s << "STORE"; break;
        case OpCodes::OUTPUT:       s << "OUTPUT"; break;
        case OpCodes::JUMP_FALSE:   s << "JUMP_FALSE"; break;
        case OpCodes::JUMP_TRUE:    s << "JUMP_TRUE"; break;
        case OpCodes::LESS_THAN:    s << "LESS_THAN"; break;
        case OpCodes::EQUALS:       s << "EQUALS"; break;
        default:                    s << std::to_string(static_cast<int>(code)); break;
    }
    return s;
}


struct OpCode {
    OpCodes code;
    std::vector<int> param_modes;
};

auto SECOND_PUZZLE = 19690720;
std::vector input_org = { 3,8,1001,8,10,8,105,1,0,0,21,38,63,80,105,118,199,280,361,442,99999,3,9,102,5,9,9,1001,9,3,9,1002,9,2,9,4,9,99,3,9,1001,9,4,9,102,4,9,9,101,4,9,9,102,2,9,9,101,2,9,9,4,9,99,3,9,1001,9,5,9,102,4,9,9,1001,9,4,9,4,9,99,3,9,101,3,9,9,1002,9,5,9,101,3,9,9,102,5,9,9,101,3,9,9,4,9,99,3,9,1002,9,2,9,1001,9,4,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,99,3,9,101,1,9,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,99 };




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
    if (value >= 1 && value <= 8) return OpCode{ static_cast<OpCodes>(value), std::vector<int>{0,0,0} };
    const auto v = convertToVector(value);
    OpCode oc;
    auto code = v[v.size() - 2] * 10;
    code += v.back();
    if (!(code == 99 || code == 1 || code == 2 || code == 3 || code == 4 || code == 5 || code == 6 || code == 7 || code == 8)) {
        std::cout << "ALARM! " << code << " from : " << value << '\n';
        assert(false);
    }
    oc.code = static_cast<OpCodes>(code);

    //iterate over every single parameter
    int j = 0;
    if (code == 1 || code == 2 || code == 7 || code == 8) oc.param_modes = { 0, 0, 0 };
    else if (code == 5 || code == 6) oc.param_modes = { 0,0 };
    else if (code == 3 || code == 4) oc.param_modes = { 0 };
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

const auto runProgram = [](int paramA, int paramB, int ip, bool& firstParamUsed, std::vector<int>& input) {
    //auto input = input_org;
    auto answer = 0;
    int index = ip;
    OpCodes latestCode;
    for (auto i = 0u; i < input.size();) {
        auto code = parseOpCode(input[index]);
        auto opCode = code.code;
        latestCode = opCode;

        if (opCode == OpCodes::STOP) {
            index += 1;
            break;
        }

        if (opCode == OpCodes::ADD) {
            const auto firstOperandPos = input[index + 1];
            const auto secondOperandPos = input[index + 2];
            const auto resultPos = input[index + 3];
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandPos] : firstOperandPos;
            const auto secondParam = code.param_modes[1] == 0 ? input[secondOperandPos] : secondOperandPos;
            const auto res = firstParam + secondParam;
            input[resultPos] = res;
            //std::cout << "ADD: input[" << resultPos << "] = " << res << '\n';

            index += 4;
        }
        else if (opCode == OpCodes::MULTIPLY) {
            const auto firstOperandPos = input[index + 1];
            const auto secondOperandPos = input[index + 2];
            const auto resultPos = input[index + 3];
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandPos] : firstOperandPos;
            const auto secondParam = code.param_modes[1] == 0 ? input[secondOperandPos] : secondOperandPos;
            const auto res = firstParam * secondParam;
            input[resultPos] = res;
            //std::cout << "MULTIPLY: input[" << resultPos << "] = " << res << '\n';

            index += 4;
        }
        else if (opCode == OpCodes::STORE) {
            const auto firstOperandPos = input[index + 1];
            const auto pos = firstOperandPos;
            input[pos] = firstParamUsed ? paramB : paramA;
            firstParamUsed = true;
            //std::cout << "STORE: input[" << pos << "] = " << param << '\n';

            index += 2;
        }
        else if (opCode == OpCodes::OUTPUT) {
            const auto firstParamVal = input[index + 1];
            const auto val = code.param_modes[0] == 0 ? input[firstParamVal] : firstParamVal;
            //std::cout << '>' << val << "<\n";
            //return val;
            answer = val;
            index += 2;
            break;
        }
        else if (opCode == OpCodes::JUMP_TRUE) {
            const auto firstOperandVal = input[index + 1];
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandVal] : firstOperandVal;
            if (firstParam != 0) {
                const auto secondOperandVal = input[index + 2];
                const auto val = code.param_modes[1] == 0 ? input[secondOperandVal] : secondOperandVal;;
                //std::cout << "JUMP_TRUE: IP(" << index << ") set to " << val << '\n';
                index = val;
            }
            else index += 3;
        }
        else if (opCode == OpCodes::JUMP_FALSE) {
            const auto firstOperandVal = input[index + 1];
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandVal] : firstOperandVal;
            if (firstParam == 0) {
                const auto secondOperandVal = input[index + 2];
                const auto val = code.param_modes[1] == 0 ? input[secondOperandVal] : secondOperandVal;;
                //std::cout << "JUMP_TRUE: IP(" << index << ") set to " << val << '\n';
                index = val;
            }
            else index += 3;
        }
        else if (opCode == OpCodes::LESS_THAN) {
            const auto firstOperandVal = input[index + 1];
            const auto secondOperandVal = input[index + 2];
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandVal] : firstOperandVal;
            const auto secondParam = code.param_modes[1] == 0 ? input[secondOperandVal] : secondOperandVal;
            const auto pos = input[index + 3];
            if (firstParam < secondParam) {
                input[pos] = 1;
                //std::cout << "LESS_THAN: input[" << pos << "] = 1\n";
            }
            else {
                input[pos] = 0;
                //std::cout << "LESS_THAN: input[" << pos << "] = 0\n";
            }
            index += 4;
        }
        else if (opCode == OpCodes::EQUALS) {
            const auto firstOperandVal = input[index + 1];
            const auto secondOperandVal = input[index + 2];
            const auto firstParam = code.param_modes[0] == 0 ? input[firstOperandVal] : firstOperandVal;
            const auto secondParam = code.param_modes[1] == 0 ? input[secondOperandVal] : secondOperandVal;
            const auto pos= input[index + 3];
            if (firstParam == secondParam) {
                input[pos] = 1;
                //std::cout << "EQUALS: input[" << pos << "] = 1\n";
            }
            else {
                input[pos] = 0;
                //std::cout << "EQUALS: input[" << pos << "] = 0\n";
            }
            index += 4;
        }
        else {
            index += 1;
            std::cout << "ZONK - " << opCode << " on index: " << index << "\n";
        }
        i = index;
    }
    return std::make_tuple(answer, latestCode, index);
};
}

struct Amp {
    Amp(int phase, std::vector<int>& input) :
        phase(phase),
        mInput(input)
    {
    }

    int process(int inputParam) {
        const auto [signal, code, instruction_pointer] = runProgram(phase, inputParam, ip, firstParamUsed, mInput);
        isFinished = (code == OpCodes::STOP);
        ip = instruction_pointer;
        return signal;
    }

    bool isRunning() const {
        return !isFinished;
    }
private:
    int phase;
    int ip {0};
    std::vector<int> mInput;
    bool isFinished {false};
    bool firstParamUsed{false};
};

int main()
{
    std::vector<int> thrusters = { 5,6,7,8,9 };
    unsigned long long sum = 0u;
    do {
        std::vector<Amp> amps;
        for (auto t : thrusters) {
            amps.push_back({t, input_org});
        }
        auto outE = 0;
        while (amps[4].isRunning()) {
            const auto outA = amps[0].process(outE);
            const auto outB = amps[1].process(outA);
            const auto outC = amps[2].process(outB);
            const auto outD = amps[3].process(outC);
            outE            = amps[4].process(outD);

            if (outE > sum) {
                sum = outE;
            }
        }
    } while (std::next_permutation(thrusters.begin(), thrusters.end()));

    std::cout << sum << '\n';
    return 0;
}
