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
        bool shallOutputBreakExecution{ false };
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

        std::tuple<int64_t, int64_t, OpCodes> run(int64_t param) {
            bool shouldBrake = false;
            std::array<int, 2> answer;
            uint64_t counter = 1;
            std::array<int, 2> res;

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

                    //answer = val;
                    if (shallOutputBreakExecution)
                        shouldBrake = true;
                    //std::cout << val << ',' << ' ';
                    if (counter % 2 == 1) {
                        res[0] = val;
                    }
                    else if (counter % 2 == 0) {
                        res[1] = val;
                        answer = res;
                        shouldBrake = true;
                    }
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
            //return answer;
            return std::make_tuple(answer[0], answer[1], lastOpCode);
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

    constexpr auto BLACK = 0;
    constexpr auto WHITE = 1;

    constexpr auto TURN_LEFT = 0;
    constexpr auto TURN_RIGHT = 1;
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

const auto dumpDirection = [](Direction d) {
    switch (d) {
    case Direction::UP: return "UP";
    case Direction::DOWN: return "DOWN";
    case Direction::LEFT: return "LEFT";
    case Direction::RIGHT: return "RIGHT";
    }
};

struct Position {
    int x;
    int y;
    bool operator<(Position const& p) const {
        if (p.x < x) return true;
        if (p.x == x) {
            return p.y < y;
        }
        return false;
    }
};

const auto getMinMaxXY = [](auto& map) {
    auto min_x = -1;
    auto max_y = 1;
    auto min_y = -1;
    auto max_x = 1;
    std::array<std::array<int, 44>, 7> image{};
    for (auto& e : map) {
        max_x = std::max(e.first.x, max_x);
        min_x = std::min(e.first.x, min_x);

        max_y = std::max(e.first.y, max_y);
        min_y = std::min(e.first.y, min_y);
    }
    return std::make_tuple(min_x, max_x, min_y, max_y);
};

struct PaintingRobot {
    PaintingRobot(IntCodeComputer c) : pc(c),
        current_direction(Direction::UP),
        current_position({ 0,0 }),
        current_colour(WHITE)
    {
        pc.disableDebug();
    }

    void run() {
        int counter = 0;
        while (true) {
            const auto [new_colour, new_direction, op_code] = pc.run(current_colour);
            const auto new_pos = updateCurrentDirectionGetNewPosition(new_direction);
            
            if (!map.contains(current_position))
                ++counter;
            else if (map[current_position] != new_colour)
                ++counter;

            map[current_position] = new_colour;

            current_position = new_pos;
            if (map.contains(current_position))
                current_colour = map[current_position];
            else 
               current_colour = BLACK;

            if (op_code == OpCodes::STOP) break;
        }
        std::cout << "painted: " << counter << std::endl;


        auto [min_x, max_x, min_y, max_y] = getMinMaxXY(map);
        const auto X_LIMIT = std::abs(min_x) + max_x + 1;
        const auto Y_LIMIT = std::abs(min_y) + max_y + 1;
        std::vector<std::vector<int>> image;
        for (auto i = 0; i < X_LIMIT; ++i) {
            image.push_back({});
            for (auto j = 0; j < Y_LIMIT; ++j) {
                image.back().push_back(0);
            }
        }
        for (auto& e : map) {
            image[e.first.x + 1][e.first.y + 1] = e.second;
        }

        for (auto i = 0; i < 7; ++i) {
            for (auto j = 0; j < 44; ++j) {
                if (image[i][j] == 0) std::cout << ' ';
                else if (image[i][j] == 1) std::cout << '@';
                else std::cout << '?';
            }
            std::cout << '\n';
        }
    }

private:
    Position updateCurrentDirectionGetNewPosition(int newDirection) {
        Position newPos{ current_position.x, current_position.y};
        switch (current_direction) {
        case Direction::UP: {
            if (newDirection == TURN_LEFT) {
                newPos.y -= 1;
                current_direction = Direction::LEFT;
            }
            else if (newDirection == TURN_RIGHT) {
                newPos.y += 1;
                current_direction = Direction::RIGHT;
            }
            break;
        }
        case Direction::DOWN: {
            if (newDirection == TURN_LEFT) {
                newPos.y += 1;
                current_direction = Direction::RIGHT;
            }
            else if (newDirection == TURN_RIGHT) {
                newPos.y -= 1;
                current_direction = Direction::LEFT;
            }
            break;
        }
        case Direction::LEFT: {
            if (newDirection == TURN_LEFT) {
                newPos.x += 1;
                current_direction = Direction::DOWN;
            }
            else if (newDirection == TURN_RIGHT) {
                newPos.x -= 1;
                current_direction = Direction::UP;
            }
            break;
        }
        case Direction::RIGHT: {
            if (newDirection == TURN_LEFT) {
                newPos.x -= 1;
                current_direction = Direction::UP;
            }
            else if (newDirection == TURN_RIGHT) {
                newPos.x += 1;
                current_direction = Direction::DOWN;
            }
            break;
        }
        }

        return newPos;
    }

private:
    IntCodeComputer pc;
    Direction current_direction;
    Position current_position;
    int current_colour;

    std::map<Position, int> map;
    std::list<Position> painted;
};
}

int main()
{
    std::vector<int64_t> input = { 3,8,1005,8,318,1106,0,11,0,0,0,104,1,104,0,3,8,102,-1,8,10,1001,10,1,10,4,10,1008,8,1,10,4,10,101,0,8,29,1,107,12,10,2,1003,8,10,3,8,102,-1,8,10,1001,10,1,10,4,10,1008,8,0,10,4,10,1002,8,1,59,1,108,18,10,2,6,7,10,2,1006,3,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,1002,8,1,93,1,1102,11,10,3,8,102,-1,8,10,1001,10,1,10,4,10,108,1,8,10,4,10,101,0,8,118,2,1102,10,10,3,8,102,-1,8,10,101,1,10,10,4,10,1008,8,0,10,4,10,101,0,8,145,1006,0,17,1006,0,67,3,8,1002,8,-1,10,101,1,10,10,4,10,1008,8,0,10,4,10,101,0,8,173,2,1109,4,10,1006,0,20,3,8,102,-1,8,10,1001,10,1,10,4,10,108,0,8,10,4,10,102,1,8,201,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,0,10,4,10,1002,8,1,224,1006,0,6,1,1008,17,10,2,101,5,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,108,1,8,10,4,10,1001,8,0,256,2,1107,7,10,1,2,4,10,2,2,12,10,1006,0,82,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,1002,8,1,294,2,1107,2,10,101,1,9,9,1007,9,988,10,1005,10,15,99,109,640,104,0,104,1,21102,1,837548352256,1,21102,335,1,0,1105,1,439,21102,1,47677543180,1,21102,346,1,0,1106,0,439,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,21102,1,235190374592,1,21101,393,0,0,1105,1,439,21102,3451060455,1,1,21102,404,1,0,1105,1,439,3,10,104,0,104,0,3,10,104,0,104,0,21102,837896909668,1,1,21102,1,427,0,1105,1,439,21102,1,709580555020,1,21102,438,1,0,1105,1,439,99,109,2,21201,-1,0,1,21102,1,40,2,21102,1,470,3,21102,460,1,0,1106,0,503,109,-2,2105,1,0,0,1,0,0,1,109,2,3,10,204,-1,1001,465,466,481,4,0,1001,465,1,465,108,4,465,10,1006,10,497,1101,0,0,465,109,-2,2105,1,0,0,109,4,1201,-1,0,502,1207,-3,0,10,1006,10,520,21101,0,0,-3,21202,-3,1,1,22101,0,-2,2,21101,1,0,3,21101,0,539,0,1106,0,544,109,-4,2105,1,0,109,5,1207,-3,1,10,1006,10,567,2207,-4,-2,10,1006,10,567,21202,-4,1,-4,1105,1,635,22101,0,-4,1,21201,-3,-1,2,21202,-2,2,3,21101,0,586,0,1105,1,544,22102,1,1,-4,21102,1,1,-1,2207,-4,-2,10,1006,10,605,21102,1,0,-1,22202,-2,-1,-2,2107,0,-3,10,1006,10,627,21202,-1,1,1,21101,627,0,0,105,1,502,21202,-2,-1,-2,22201,-4,-2,-4,109,-5,2105,1,0
    };

    IntCodeComputer pc(input);
    PaintingRobot r(pc);
    r.run();
    return 0;
}
