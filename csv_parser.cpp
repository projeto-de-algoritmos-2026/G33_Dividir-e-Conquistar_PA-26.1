#include "csv_parser.hpp"

#include <vector>
#include <utility>
#include <fstream>
#include <string>

std::vector<std::pair<std::string, std::pair<double, double>>> csv_parser() {
    std::string file_name = "municipios.csv";
    std::ifstream file(file_name);
    std::string line;

    std::vector<std::pair<std::string, std::pair<double, double>>> cities;
    std::string cur, name, latitude, longitude;

    while (std::getline(file, line)) {
        int cnt = 0;

        for (char c : line) {
            if (c == ',') {
                ++cnt;
                if (cnt == 2) name = cur;
                if (cnt == 3) latitude = cur;
                if (cnt == 4) longitude = cur;
                cur.clear();
            }
            else cur += c;
        }

        if (latitude[0] != 'l')  // A primeira linha isso vai ser "latitude"
            cities.emplace_back(name, std::make_pair(std::stod(latitude), std::stod(longitude)));
    }

    file.close();
    return cities;
}
