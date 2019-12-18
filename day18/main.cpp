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
#include <fstream>

namespace {
    const auto loadData = [](auto path){
        std::vector<char> out;
        {
            using namespace std;
            char ch;
            fstream fs(path, fstream::in);
            while (fs >> ch) {
                out.push_back(ch);
            }
            fs.close();
        }
        return out;
    };
    const auto printData = [](auto& data) {
        for (auto ch: data)
            std::cout << ch << ' ';
        std::cout << '\n';
    };
}

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "too few parameters\n";
        return -1;
    }

    const std::string path = argv[1];
    const auto input = loadData(path);
    printData(input);
    return 0;
}
