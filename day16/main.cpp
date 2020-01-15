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
#include <array>

namespace {


const auto convertToVector = [](std::string input) {
    std::vector<int> res;
    for (auto ch: input) {
        res.push_back(ch - '0');
    }
    return res;
};

const auto generatePattern = [](std::size_t count, std::size_t length) {
    std::vector<int> pattern;
    static constexpr std::array<int, 4> base_pattern = {0, 1, 0, -1};
    while (pattern.size() < length + 1) {
        for (auto e: base_pattern) {
            for (auto i = 0u; i < count; ++i) {
                if (pattern.size() < length + 1)
                    pattern.push_back(e);
                else break;
            }
        }
    }
    return std::vector<int>(pattern.begin() + 1, pattern.end());
};

const auto extractLastDigit = [](int val) {
    return std::abs(val) % 10;
};
const auto print = [](auto& v) {
    for (auto e: v) {
        std::cout << e;
    }
};

const auto calculatePhase = [](std::vector<int> input){
        std::vector<int> new_input(input.size());
        for (int j = 0; j < input.size(); ++j) {
            int sum = 0;
            const auto pattern = generatePattern(j + 1u, input.size());
            for (int k = input.size() - 1; k >= 0 + j; --k) {
                //std::cout << input[k] << '*' << pattern[k] << ' ';
                //if (k < input.size() -1) std::cout << " + ";
                sum += pattern[k] * input[k];
            }
            //std::cout <<  " = " << extractLastDigit(sum);
            //std::cout << "\n\n";
            new_input[j] = extractLastDigit(sum);
        }
        return new_input;
        //print(input);
};
}

int main()
{
    std::string input = "59777373021222668798567802133413782890274127408951008331683345339720122013163879481781852674593848286028433137581106040070180511336025315315369547131580038526194150218831127263644386363628622199185841104247623145887820143701071873153011065972442452025467973447978624444986367369085768018787980626750934504101482547056919570684842729787289242525006400060674651940042434098846610282467529145541099887483212980780487291529289272553959088376601234595002785156490486989001949079476624795253075315137318482050376680864528864825100553140541159684922903401852101186028076448661695003394491692419964366860565639600430440581147085634507417621986668549233797848";
    //input = "12345678";
    std::string ss;
    for (auto i = 1; i <= 1000; ++i) {
        ss += input;
    }
    //input = "80871224585914546619083218645595";
    auto data = convertToVector(ss);

    auto d = data;
    for (auto i = 0u; i < 100; ++i) {
        const auto dd = calculatePhase(d);
        d = dd;
        std::cout << '.';
      //  std::cout << '\n';
    }

    print(d);
    std::cout << '\n';
    return 0;
}
