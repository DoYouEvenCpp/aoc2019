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
    EQUALS = 8,
    UPDATE_BASE = 9
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
        case OpCodes::UPDATE_BASE:  s << "UPDATE_BASE"; break;
        default:                    s << std::to_string(static_cast<int64_t>(code)); break;
    }
    return s;
}

struct OpCode {
    OpCodes code;
    std::vector<int64_t> param_modes;
};

const auto printInput = [](auto& input) {
    for (auto e : input) {
        std::cout << e << ' ';
    }
    std::cout << '\n';
};

class IntCodeComputer {
private:
    std::vector<int64_t> memory;
    std::optional<int64_t> onceParam;
    size_t ip {0};
    int64_t relativeBase {0};
    OpCodes lastOpCode {};
    bool isDebugEnabled {true};
    bool shallOutputBreakExecution{ false };


public:
IntCodeComputer(std::vector<int64_t> const& mem): memory(mem) {
    for (auto i = 0; i < 100*100*100; ++i)
        memory.push_back(0);
}

void updateMemoryLocation(size_t pos, int64_t value) {
    memory[pos] = value;
}

void setMemory(std::vector<int64_t>& m) {
    memory = m;
}

void useOnce(int64_t param) {
    onceParam = param;
}

void updateInstructionPointerPosition(size_t pos) {
    ip = pos;
}

void enableDebug() {
    isDebugEnabled = true;
}

void disableDebug() {
    isDebugEnabled = false;
}

OpCodes getLatestCode() const {
    return lastOpCode;
}

void runTests(int64_t param) {
    run(param);
}

std::tuple<int64_t, OpCodes, int64_t> run(int64_t param) {
    bool shouldBrake = false;
    int64_t answer = 0;
    while(true) {
        auto [opCode, mode1, mode2, mode3] = getParamModes();
        lastOpCode = opCode;
        switch(opCode) {
            case OpCodes::STOP: {
                ++ip;
                shouldBrake = true;
                if (isDebugEnabled)
                    std::cout << "TERMINATE (ip: " << ip << ", opCode: " << lastOpCode << ")\n";
                break;
            }
            case OpCodes::ADD: {
                const auto firstParam = getValue(mode1, memory[ip + 1]);
                const auto secondParam = getValue(mode2, memory[ip + 2]);
                const auto pos = getIndex(mode3, memory[ip + 3]);
                memory[pos] = firstParam + secondParam;
                if (isDebugEnabled)
                        std::cout << "ip: " << ip << " ADD"
                        << " (" << firstParam << ", " << mode1 << ")"
                        << " (" << secondParam << ", " << mode2 << ")"
                        << " result = " << (firstParam + secondParam)
                        << " pos: " << pos
                        <<'\n';
                ip += 4;
                break;
            }
            case OpCodes::MULTIPLY: {
                const auto firstParam = getValue(mode1, memory[ip + 1]);
                const auto secondParam = getValue(mode2, memory[ip + 2]);
                const auto pos = getIndex(mode3, memory[ip + 3]);

                if (isDebugEnabled)
                    std::cout << "ip: " << ip << " MULTIPLY"
                        << " (" << firstParam << ", " << mode1 << ")"
                        << " (" << secondParam << ", " << mode2 << ")"
                        << " result = " << (firstParam * secondParam)
                        << " pos: " << pos
                        <<'\n';
                memory[pos] = firstParam * secondParam;
                ip += 4;
                break;
            }
            case OpCodes::STORE: {
                const auto pos = getIndex(mode1, memory[ip + 1]);
                const auto val = onceParam? *onceParam: param;
                if (isDebugEnabled)
                    std::cout << "ip: " << ip << " STORE"
                        << " (" << val << ", " << mode1 << ")"
                        << " pos: " << pos
                        <<'\n';

                if (onceParam) {
                    onceParam.reset();
                }
                memory[pos] = val;
                ip += 2;
                break;
            }
            case OpCodes::OUTPUT: {
                const auto val = getValue(mode1, memory[ip + 1]);
                if (isDebugEnabled)
                    std::cout   << "ip: " << ip << " OUTPUT"
                                << " (" << val << ", " << mode1 << ")"
                                << '\n';

                answer = val;
                if (shallOutputBreakExecution)
                    shouldBrake = true;
                std::cout << "\t\t>" << val << "<\n";
                ip += 2;
                break;
            }
            case OpCodes::JUMP_TRUE: {
                const auto firstParam = getValue(mode1, memory[ip + 1]);
                const auto secondParam = getValue(mode2, memory[ip + 2]);
                if (isDebugEnabled)
                        std::cout << "ip: " << ip << " JUMP_TRUE"
                        << " (" << firstParam << ", " << mode1 << ")"
                        << " (" << secondParam << ", " << mode2 << ")"
                        << " result = " << (firstParam != 0)
                        <<'\n';
                if (firstParam != 0) {
                    ip = secondParam;
                }
                else
                    ip += 3;
                break;
            }
            case OpCodes::JUMP_FALSE: {
                const auto firstParam = getValue(mode1, memory[ip + 1]);
                if (firstParam == 0) {
                    const auto secondParam = getValue(mode2, memory[ip + 2]);
                    if (isDebugEnabled)
                        std::cout << "ip: " << ip << " JUMP_FALSE"
                            << " (" << firstParam << ", " << mode1 << ")"
                            << " (" << secondParam << ", " << mode2 << ")"
                            << " result = " << (firstParam == 0)
                            <<'\n';
                    ip = secondParam;
                }
                else
                    ip += 3;
                break;
            }
            case OpCodes::LESS_THAN: {
                const auto firstParam = getValue(mode1, memory[ip + 1]);
                const auto secondParam = getValue(mode2, memory[ip + 2]);
                const auto pos = getIndex(mode3, memory[ip + 3]);
                const auto val = firstParam < secondParam;
                if (isDebugEnabled)
                    std::cout << "ip: " << ip << " LESS_THAN"
                        << " (" << firstParam << ", " << mode1 << ")"
                        << " (" << secondParam << ", " << mode2 << ")"
                        << " result = " << (firstParam < secondParam)
                        << " pos: " << pos
                        <<'\n';
                memory[pos] = val;
                ip += 4;
                break;
            }
            case OpCodes::EQUALS: {
                const auto firstParam = getValue(mode1, memory[ip + 1]);
                const auto secondParam = getValue(mode2, memory[ip + 2]);
                const auto pos = getIndex(mode3, memory[ip + 3]);
                const auto val = firstParam == secondParam;

                if (isDebugEnabled)
                    std::cout << "ip: " << ip << " EQUALS"
                        << " (" << firstParam << ", " << mode1 << ")"
                        << " (" << secondParam << ", " << mode2 << ")"
                        << " result = " << (val)
                        <<'\n';
                memory[pos] = val;
                ip += 4;
                break;
            }
            case OpCodes::UPDATE_BASE: {
                const auto val = getValue(mode1, memory[ip + 1]);
                if (isDebugEnabled)
                    std::cout << "ip: " << ip << " UPDATE_BASE"
                        << " currentVal: " << relativeBase << " updating with : " << val << ", mode:" << mode1
                        <<'\n';
                relativeBase += val;
                ip += 2;
                break;
            }
        }
        if (shouldBrake) break;
    }

    return std::make_tuple(answer, lastOpCode, ip);
}

private:
std::tuple<OpCodes, int64_t, int64_t, int64_t> getParamModes() {
        const auto value = memory[ip];
        const auto opCode = convertToOpCode(value % 100);
        const auto param1Mode = (value / 100) % 10;
        const auto param2Mode = (value / 1000) % 10;
        const auto param3Mode = (value / 10000) % 10;
        return {opCode, param1Mode, param2Mode, param3Mode};
    }

int64_t getIndex(int64_t mode, int64_t val) {
    if (mode == 0) return val;
    if (mode == 2) return val + relativeBase;
    assert(false);
    return {};
}

int64_t getValue(int64_t mode, int64_t val) {
    if (mode == 0) return memory[val];                  //positional
    if (mode == 1) return val;                          //immediate
    if (mode == 2) return memory[val + relativeBase];   //relative
    assert(false);
    return {};
}

OpCodes convertToOpCode(int64_t code) {
    switch(code) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 99:
            return static_cast<OpCodes>(code);
        default: {
            std::cout << "ALARM! " << code << '\n';
            assert(false);
        };
    }
    return {};
}
};
}

int main()
{
    std::vector<int64_t> input = { 1102,34463338,34463338,63,1007,63,34463338,63,1005,63,53,1102,3,1,1000,109,988,209,12,9,1000,209,6,209,3,203,0,1008,1000,1,63,1005,63,65,1008,1000,2,63,1005,63,904,1008,1000,0,63,1005,63,58,4,25,104,0,99,4,0,104,0,99,4,17,104,0,99,0,0,1101,0,26,1014,1102,1,30,1013,1101,22,0,1000,1101,0,35,1015,1101,0,34,1011,1102,0,1,1020,1102,1,481,1022,1101,0,36,1003,1102,1,28,1005,1101,857,0,1024,1101,20,0,1008,1101,0,385,1026,1102,37,1,1006,1101,33,0,1017,1101,0,38,1002,1102,23,1,1007,1102,32,1,1010,1101,29,0,1016,1102,1,25,1009,1102,1,27,1012,1101,24,0,1018,1101,474,0,1023,1102,1,39,1004,1101,0,31,1001,1102,378,1,1027,1101,0,848,1025,1102,21,1,1019,1102,760,1,1029,1102,1,1,1021,1101,769,0,1028,109,-6,2107,21,6,63,1005,63,199,4,187,1106,0,203,1001,64,1,64,1002,64,2,64,109,16,2101,0,-6,63,1008,63,39,63,1005,63,225,4,209,1106,0,229,1001,64,1,64,1002,64,2,64,109,5,2108,20,-7,63,1005,63,247,4,235,1105,1,251,1001,64,1,64,1002,64,2,64,109,-1,2108,36,-8,63,1005,63,267,1106,0,273,4,257,1001,64,1,64,1002,64,2,64,109,-13,1201,-1,0,63,1008,63,22,63,1005,63,299,4,279,1001,64,1,64,1106,0,299,1002,64,2,64,109,15,2102,1,-8,63,1008,63,20,63,1005,63,321,4,305,1106,0,325,1001,64,1,64,1002,64,2,64,109,-13,21108,40,40,8,1005,1011,347,4,331,1001,64,1,64,1105,1,347,1002,64,2,64,109,-2,1207,8,24,63,1005,63,363,1105,1,369,4,353,1001,64,1,64,1002,64,2,64,109,35,2106,0,-9,1001,64,1,64,1106,0,387,4,375,1002,64,2,64,109,-26,21102,41,1,3,1008,1013,41,63,1005,63,409,4,393,1106,0,413,1001,64,1,64,1002,64,2,64,109,2,1202,-6,1,63,1008,63,36,63,1005,63,433,1106,0,439,4,419,1001,64,1,64,1002,64,2,64,109,-3,21102,42,1,10,1008,1019,40,63,1005,63,463,1001,64,1,64,1106,0,465,4,445,1002,64,2,64,109,15,2105,1,-1,1001,64,1,64,1106,0,483,4,471,1002,64,2,64,109,-27,1207,3,23,63,1005,63,505,4,489,1001,64,1,64,1105,1,505,1002,64,2,64,109,13,2102,1,-9,63,1008,63,28,63,1005,63,525,1105,1,531,4,511,1001,64,1,64,1002,64,2,64,109,1,2101,0,-8,63,1008,63,35,63,1005,63,551,1105,1,557,4,537,1001,64,1,64,1002,64,2,64,109,6,21107,43,44,-4,1005,1013,575,4,563,1106,0,579,1001,64,1,64,1002,64,2,64,109,-9,1201,-4,0,63,1008,63,40,63,1005,63,599,1105,1,605,4,585,1001,64,1,64,1002,64,2,64,109,12,1206,1,621,1001,64,1,64,1106,0,623,4,611,1002,64,2,64,109,-22,1202,9,1,63,1008,63,23,63,1005,63,649,4,629,1001,64,1,64,1105,1,649,1002,64,2,64,109,17,1206,5,667,4,655,1001,64,1,64,1106,0,667,1002,64,2,64,109,-3,1205,9,685,4,673,1001,64,1,64,1106,0,685,1002,64,2,64,109,3,1208,-9,37,63,1005,63,707,4,691,1001,64,1,64,1105,1,707,1002,64,2,64,109,7,1205,-2,723,1001,64,1,64,1106,0,725,4,713,1002,64,2,64,109,-15,21101,44,0,8,1008,1015,45,63,1005,63,745,1105,1,751,4,731,1001,64,1,64,1002,64,2,64,109,28,2106,0,-7,4,757,1001,64,1,64,1106,0,769,1002,64,2,64,109,-12,21101,45,0,-5,1008,1018,45,63,1005,63,791,4,775,1105,1,795,1001,64,1,64,1002,64,2,64,109,-9,2107,26,-5,63,1005,63,815,1001,64,1,64,1106,0,817,4,801,1002,64,2,64,109,-1,21107,46,45,-3,1005,1010,833,1105,1,839,4,823,1001,64,1,64,1002,64,2,64,109,3,2105,1,8,4,845,1001,64,1,64,1106,0,857,1002,64,2,64,109,-9,1208,-4,37,63,1005,63,877,1001,64,1,64,1105,1,879,4,863,1002,64,2,64,109,8,21108,47,46,2,1005,1017,895,1106,0,901,4,885,1001,64,1,64,4,64,99,21102,1,27,1,21102,1,915,0,1106,0,922,21201,1,14429,1,204,1,99,109,3,1207,-2,3,63,1005,63,964,21201,-2,-1,1,21102,1,942,0,1105,1,922,21202,1,1,-1,21201,-2,-3,1,21101,957,0,0,1106,0,922,22201,1,-1,-2,1105,1,968,21201,-2,0,-2,109,-3,2105,1,0
    };
    {
        IntCodeComputer pc(input);
        pc.disableDebug();
        pc.runTests(1);
    }

    {
        IntCodeComputer pc(input);
        pc.disableDebug();
        pc.runTests(2);
    }


    //sanity checks
    // pc.runTests(7);
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(8);
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(9);

    // input = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(0);
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(3);
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(0);

    // input ={3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(7);
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(8);
    // pc.setMemory(input);
    // pc.updateInstructionPointerPosition(0);
    // pc.runTests(9);

    return 0;
}
