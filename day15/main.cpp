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
#include <functional>
#include <stack>
#include <cassert>
#include <queue>

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

    constexpr auto NORTH = 1;
    constexpr auto SOUTH = 2;
    constexpr auto WEST = 3;
    constexpr auto EAST = 4;

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
        size_t ip{ 0 };
        int64_t relativeBase{ 0 };
        OpCodes lastOpCode{};
        bool isDebugEnabled{ true };
        bool shallOutputBreakExecution{ true };


    public:
        IntCodeComputer(std::vector<int64_t> const& mem) : memory(mem) {
            for (auto i = 0; i < 100 * 100 * 100; ++i)
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

        //std::tuple<int64_t, OpCodes, int64_t> run(int64_t param) {
        int64_t run(int64_t param) {
            bool shouldBrake = false;
            int64_t answer = 0;
            uint64_t counter = 1;

            while (true) {
                auto [opCode, mode1, mode2, mode3] = getParamModes();
                lastOpCode = opCode;
                switch (opCode) {
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
                        << '\n';
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
                        << '\n';
                    memory[pos] = firstParam * secondParam;
                    ip += 4;
                    break;
                }
                case OpCodes::STORE: {
                    const auto pos = getIndex(mode1, memory[ip + 1]);
                    const auto val = onceParam ? *onceParam : param;
                    if (isDebugEnabled)
                        std::cout << "ip: " << ip << " STORE"
                        << " (" << val << ", " << mode1 << ")"
                        << " pos: " << pos
                        << '\n';

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
                        std::cout << "ip: " << ip << " OUTPUT"
                        << " (" << val << ", " << mode1 << ")"
                        << '\n';

                    answer = val;
                    if (shallOutputBreakExecution)
                        shouldBrake = true;
                    //std::cout << val << ',' << ' ';
                    if (val == 2) std::cout << val;
                    ip += 2;
                    ++counter;
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
                        << '\n';
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
                            << '\n';
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
                        << '\n';
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
                        << '\n';
                    memory[pos] = val;
                    ip += 4;
                    break;
                }
                case OpCodes::UPDATE_BASE: {
                    const auto val = getValue(mode1, memory[ip + 1]);
                    if (isDebugEnabled)
                        std::cout << "ip: " << ip << " UPDATE_BASE"
                        << " currentVal: " << relativeBase << " updating with : " << val << ", mode:" << mode1
                        << '\n';
                    relativeBase += val;
                    ip += 2;
                    break;
                }
                }
                if (shouldBrake) break;
            }

            //return std::make_tuple(answer, lastOpCode, ip);
            return answer;
        }

    private:
        std::tuple<OpCodes, int64_t, int64_t, int64_t> getParamModes() {
            const auto value = memory[ip];
            const auto opCode = convertToOpCode(value % 100);
            const auto param1Mode = (value / 100) % 10;
            const auto param2Mode = (value / 1000) % 10;
            const auto param3Mode = (value / 10000) % 10;
            return { opCode, param1Mode, param2Mode, param3Mode };
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
            switch (code) {
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


    struct Point {
        int x;
        int y;
        int64_t type;

        bool operator<(Point const& p) const {
            if (p.x < x) return true;
            if (p.x == x) {
                return p.y < y;
            }
            return false;
        }
    };

    struct RepairRobot {
        RepairRobot(IntCodeComputer c) : pc(c), currentPosition({ 0,0 })
        {
            pc.disableDebug();
        }

        void explore() {
            auto param = NORTH;
            map.insert({ 0,0 });
            makeMove(NORTH, 0, 0);
            makeMove(SOUTH, 0, 0);
            makeMove(EAST, 0, 0);
            makeMove(WEST, 0, 0);
        }

private:
    void makeMove(int direction, int x, int y) {
        std::stack<int> moves;
        std::queue<int> q;
        q.push(direction);

        auto current_x = x;
        auto current_y = y;

        while(!q.empty()) {
            const auto newDirection = q.front();
            q.pop();

            auto [x_, y_] = getNewPositions(newDirection, current_x, current_y);
            const auto type = pc.run(newDirection);
            if (auto [it, isInserted] = map.insert({ x_,y_,type }); !isInserted) {
                //been here already
                continue;
            }
            if (type != 0) {
                moves.push(getOpositeDirection(newDirection));
                current_x = x_;
                current_y = y_;
            }
            if (type != 0) {
                    q.push(NORTH);
                    q.push(SOUTH);
                    q.push(EAST);
                    q.push(WEST);
            }
        }
        while (!moves.empty()) {
            pc.run(moves.top());
            moves.pop();
        }
    }

    std::pair<int, int> getNewPositions (int direction, int x, int y) {
        if (direction == NORTH) return {x-1, y};
        if (direction == SOUTH) return {x+1, y};
        if (direction == EAST) return {x, y+1};
        if (direction == WEST) return {x, y-1};
    }
    int getOpositeDirection(int direction) {
        if (direction == NORTH) return SOUTH;
        if (direction == SOUTH) return NORTH;
        if (direction == EAST) return WEST;
        if (direction == WEST) return EAST;
        assert(false);
        return 0;
    }

    IntCodeComputer pc;
    std::set<Point> map;
    std::set<Point> points_to_visit;
    Point currentPosition;
};
}

int main()
{
    std::vector<int64_t> input = { 3,1033,1008,1033,1,1032,1005,1032,31,1008,1033,2,1032,1005,1032,58,1008,1033,3,1032,1005,1032,81,1008,1033,4,1032,1005,1032,104,99,101,0,1034,1039,101,0,1036,1041,1001,1035,-1,1040,1008,1038,0,1043,102,-1,1043,1032,1,1037,1032,1042,1105,1,124,1001,1034,0,1039,102,1,1036,1041,1001,1035,1,1040,1008,1038,0,1043,1,1037,1038,1042,1106,0,124,1001,1034,-1,1039,1008,1036,0,1041,1001,1035,0,1040,101,0,1038,1043,101,0,1037,1042,1106,0,124,1001,1034,1,1039,1008,1036,0,1041,1002,1035,1,1040,1002,1038,1,1043,1001,1037,0,1042,1006,1039,217,1006,1040,217,1008,1039,40,1032,1005,1032,217,1008,1040,40,1032,1005,1032,217,1008,1039,1,1032,1006,1032,165,1008,1040,35,1032,1006,1032,165,1102,1,2,1044,1105,1,224,2,1041,1043,1032,1006,1032,179,1101,0,1,1044,1106,0,224,1,1041,1043,1032,1006,1032,217,1,1042,1043,1032,1001,1032,-1,1032,1002,1032,39,1032,1,1032,1039,1032,101,-1,1032,1032,101,252,1032,211,1007,0,63,1044,1105,1,224,1102,1,0,1044,1105,1,224,1006,1044,247,1001,1039,0,1034,102,1,1040,1035,1001,1041,0,1036,1001,1043,0,1038,101,0,1042,1037,4,1044,1105,1,0,60,55,93,19,49,51,86,12,18,69,42,30,84,1,28,84,15,15,70,11,75,8,67,37,76,61,72,2,49,82,25,57,77,51,87,60,21,66,5,90,56,21,74,75,51,54,83,69,57,85,99,40,94,14,84,69,34,51,92,29,28,2,76,1,35,70,5,91,91,61,86,2,35,74,78,44,98,44,5,78,4,79,53,99,80,11,75,29,2,82,31,71,82,60,22,90,68,11,84,69,8,66,74,53,22,69,19,49,55,69,75,36,65,18,83,37,17,10,78,89,4,74,29,51,96,11,64,15,99,52,51,99,14,78,66,7,99,20,26,64,91,12,94,38,65,87,91,69,5,87,28,2,62,45,83,35,52,19,21,83,25,51,93,92,7,70,39,92,84,31,1,98,92,58,30,75,22,89,79,44,14,66,11,93,36,45,90,42,18,87,73,99,5,95,94,20,64,78,70,98,41,52,98,5,73,94,19,57,64,88,59,83,33,51,71,25,93,43,14,92,83,44,83,41,52,31,91,95,51,36,98,65,45,10,89,58,51,52,88,94,59,98,2,45,93,83,46,74,76,11,38,9,84,99,43,97,6,28,64,28,72,81,87,74,68,14,27,80,96,44,10,96,36,2,33,96,78,45,30,87,89,90,50,2,72,77,10,12,64,74,53,7,74,57,81,28,68,11,8,47,16,88,17,42,99,58,92,36,70,32,83,37,49,16,97,61,88,91,54,17,33,55,29,22,85,82,30,81,40,62,69,94,47,69,25,77,33,87,67,40,44,96,31,75,27,80,8,16,75,67,41,82,52,95,17,56,99,84,66,53,65,70,87,61,15,82,86,55,96,8,24,79,99,8,79,80,7,64,69,1,67,5,74,20,64,4,98,13,53,2,64,23,33,78,77,51,91,13,24,69,49,56,77,64,10,75,11,67,86,48,98,95,19,94,20,11,62,97,62,83,97,12,95,97,90,20,72,75,49,56,16,65,52,88,95,61,44,86,83,94,9,25,71,99,46,80,80,32,38,56,83,49,89,55,75,98,52,77,85,29,42,94,29,7,75,81,16,28,57,24,92,57,67,27,83,42,75,88,62,50,2,94,3,42,73,17,80,73,91,62,67,84,16,76,44,16,70,36,79,90,41,90,91,62,26,86,94,34,68,59,27,82,74,18,19,98,56,2,90,96,70,28,67,38,51,84,83,13,34,4,52,67,77,31,93,12,41,86,26,61,59,67,73,80,19,48,60,94,57,72,56,36,77,73,57,59,94,69,5,37,90,72,62,4,85,12,65,94,81,5,99,30,58,73,18,90,89,6,87,82,27,41,87,46,97,19,85,11,81,79,17,12,94,46,99,56,77,86,11,20,65,97,37,1,71,21,37,72,29,41,83,39,24,86,72,25,26,20,75,78,34,75,33,38,89,13,31,55,82,81,15,88,36,76,82,22,24,84,73,53,8,82,83,71,15,82,44,88,41,74,80,86,19,59,65,70,76,62,59,79,34,20,30,28,67,35,93,34,56,65,98,97,59,93,54,84,11,85,70,95,17,69,28,79,65,52,69,72,10,72,2,68,84,56,12,64,74,83,13,69,78,5,51,91,41,88,72,10,97,33,97,33,86,19,96,59,64,44,42,88,4,57,20,84,54,44,92,28,17,86,15,50,5,76,37,10,97,39,33,94,5,82,7,92,9,84,55,64,23,69,9,96,49,81,28,69,76,92,53,88,92,92,61,78,44,74,99,96,51,79,65,71,58,86,34,96,96,96,26,88,0,0,21,21,1,10,1,0,0,0,0,0,0
    };

    IntCodeComputer pc(input);
    RepairRobot r(pc);
    r.explore();
    std::cout << '\n';
    return 0;
}
