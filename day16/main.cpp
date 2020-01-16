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
#include <numeric>
#include <chrono>

namespace {

template <typename F, typename... Args>
auto measureTimeInSeconds(F fun, Args&&... args){
    auto start = std::chrono::steady_clock::now();
    fun(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();
    std::cout << " elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";
}

const auto convertToVector = [](std::string input) {
    std::vector<int> res;
    for (auto ch : input) {
        res.push_back(ch - '0');
    }
    return res;
};

inline auto extractLastDigit(int val) {
    return std::abs(val) % 10;
};
const auto print = [](auto const& v) {
    for (auto e : v) {
        std::cout << e;
    }
};

inline auto partial_sum(std::vector<int>& input, int len, int offset) {
    int sum = 0;
    int i = offset;
    for (; i < offset + len && i < input.size(); i++)
        sum += input[i];
    return std::make_pair(sum, i - offset);
}

int getValuesFromRange(std::vector<int>& input, int iteration, int offset) {
    const auto count = (input.size() - offset) / iteration;
    const auto size = input.size();
    int sum = 0;
    int i = offset;
    int j = 0;
    bool flag = true;
    do {
        if (j % 4 == 0) {
            const auto [s, ii] = partial_sum(input, iteration, i);
            sum += s;
            i += ii;
            flag = i < size;
        }
        else if (j % 4 == 1) {
            i += iteration;
            flag = i < size;
        }
        else if (j % 4 == 2){
            const auto [s, ii] = partial_sum(input, iteration, i);
            sum -= s;
            i += ii;
            flag = i < size;
        }
        else {
            i += iteration;
            flag = i < size;
        }
        ++j;
    } while (flag);

    return sum;
}

const auto calculatePhase = [](std::vector<int>& input) {
    std::vector<int> new_input(input.size());
    for (int j = 0; j < input.size(); ++j) {
        int sum = extractLastDigit(getValuesFromRange(input, j + 1, j));
        new_input[j] = sum;
    }
    std::swap(input, new_input);
    //return new_input;
    //print(input);
};
}

int main()
{
    std::string input = "59777373021222668798567802133413782890274127408951008331683345339720122013163879481781852674593848286028433137581106040070180511336025315315369547131580038526194150218831127263644386363628622199185841104247623145887820143701071873153011065972442452025467973447978624444986367369085768018787980626750934504101482547056919570684842729787289242525006400060674651940042434098846610282467529145541099887483212980780487291529289272553959088376601234595002785156490486989001949079476624795253075315137318482050376680864528864825100553140541159684922903401852101186028076448661695003394491692419964366860565639600430440581147085634507417621986668549233797848";
    //input = "12345678";
    //input = "03036732577212944063491565474664";
    std::string ss;
    for (auto i = 1; i <= 10000; ++i) {
        ss += input;
    }
    auto offset = std::stoi(ss.substr(0, 7));
    auto data = convertToVector(ss);

    std::cout << "starting... " << data.size() << ", offset: " << offset << '\n';
    auto d = data;
    for (auto i = 0u; i < 100; ++i) {
        auto start = std::chrono::steady_clock::now();
        measureTimeInSeconds(calculatePhase, d);
    }

    print(d);
    std::cout << d.size() << '\n';
    if (d.size() >= offset + 8)
        print(std::vector<int>{d.begin() + offset, d.begin() + offset + 8});
    std::cout << '\n';
    return 0;
}
