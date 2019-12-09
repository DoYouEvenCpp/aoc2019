#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <array>
#include <cmath>
#include <tuple>
#include <cassert>
#include <fstream>
#include <iterator>


namespace {
    constexpr auto WIDTH = 25;
    constexpr auto HEIGHT = 6;
    using LINE_TYPE = std::array<int, WIDTH>;
    using LAYER_TYPE = std::array<LINE_TYPE, HEIGHT>;

    const auto loadData = [](){
        std::vector<int> out;
        {
            using namespace std;
            char ch;
            fstream fs("input", fstream::in);
            while (fs >> ch) {
                out.push_back(static_cast<int>(ch - 48));
            }
            fs.close();
        }

        std::cout << "parsing image, read " << out.size() << " bytes" << std::endl;
        std::vector<LAYER_TYPE> image;
        LINE_TYPE single_line;
        LAYER_TYPE single_layer;

        int line = 0; //for line
        int row = 0; //for rows
        for (int i = 0; i < out.size(); ++i) {
            single_line[line++] = out[i];
            if (line == WIDTH) {
                //line is ready, add it to the layer
                single_layer[row++] = single_line;
                single_line = {};
                line = 0;
                if (row == HEIGHT) {
                    //row is ready, add layet to vector
                    image.push_back(single_layer);
                    single_layer = {};
                    row = 0;
                }
            }
        }
        return image;
    };

    const auto printImage = [](auto& image) {
        for (auto& layer : image) {
        for (auto& line: layer) {
            for (auto i =0; i < line.size(); ++i) {
                //std::cout << line[i] << ' ';
                if (line[i] == 0) std::cout << ' ' << ' ';
                else if (line[i] == 1) std::cout << '#' << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    };

    const auto getZeroNumbers = [](LAYER_TYPE& layer) {
        auto sum = 0u;
        auto ones = 0u;
        auto twos = 0u;
        for (auto& line: layer){
            sum += std::count(line.begin(), line.end(), 0);
            ones += std::count(line.begin(), line.end(), 1);
            twos += std::count(line.begin(), line.end(), 2);
        }
        return std::make_pair(sum, ones * twos);
    };

    const auto parseLayers = [](auto& image) {
        LAYER_TYPE final = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
        for (auto i = 0; i < HEIGHT; ++i) {
            for (auto j = 0; j < WIDTH; ++j) {
                for (auto k = 0; k < image.size(); ++k) {
                    if (final[i][j] == 0 || final[i][j] == 1) break;

                    if (image[k][i][j] == 1 || image[k][i][j] == 0) {
                        final[i][j] = image[k][i][j];
                        break;
                    }
                }
            }
        }
        return final;
    };
}


int main()
{
    auto image = loadData();
    std::map<int, int> r;
    for (auto& layer: image) {
        r.emplace(getZeroNumbers(layer));
    }

    auto res = r.begin();
    std::cout << "First: = " << res->second << '\n';

    std::vector<LAYER_TYPE> f;
    f.emplace_back(parseLayers(image));
    printImage(f);
    return 0;
}
