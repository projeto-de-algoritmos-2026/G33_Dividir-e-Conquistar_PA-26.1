#include "csv_parser.hpp"

#include <vector>
#include <utility>
#include <fstream>
#include <string>

std::vector<std::pair<std::string, std::pair<double, double>>> csv_parser() {
    std::string file_name = "BR_Municipios_2025.csv";
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
                if (cnt == 16) latitude = cur;
                cur.clear();
            }
            else cur += c;
        }
        longitude = cur;

        if (latitude[0] != 'L')  // A primeira linha isso vai ser "LATITUDE"
            cities.emplace_back(name, std::make_pair(std::stod(latitude), std::stod(longitude)));
    }

    file.close();
    return cities;
}
