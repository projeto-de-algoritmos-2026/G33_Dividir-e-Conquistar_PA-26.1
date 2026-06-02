#include "csv_parser.hpp"

#include <bits/stdc++.h>
using namespace std;

double to_radians(double degree) { return degree * M_PI / 180.0; }

double mean_lat = 0;

vector<pair<double, double>> spheric_to_cartesian(const vector<pair<double, double>>& points) {
    for (auto [lat, lon] : points) mean_lat += lat;
    mean_lat = to_radians(mean_lat / points.size());

    const double R = 6371.0;
    vector<pair<double, double>> cartesian_points;

    for (auto [lat, lon] : points) {
        double lat_rad = to_radians(lat);
        double lon_rad = to_radians(lon);

        double x = R * lon_rad * cos(mean_lat);
        double y = R * lat_rad;

        cartesian_points.emplace_back(x, y);
    }

    return cartesian_points;
}


pair<double, double> cartesian_to_spheric(pair<double, double> p) {
    const double R = 6371.0;
    double x = p.first;
    double y = p.second;
    
    double lat_rad = y / R;
    double lon_rad = x / (R * cos(mean_lat));
    
    double lat = lat_rad * 180.0 / M_PI;
    double lon = lon_rad * 180.0 / M_PI;
    
    return {lat, lon};
}

double dist(pair<double, double> A, pair<double, double> B) {
    return (A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second);
}

pair<int, int> closest_pair(const vector<pair<double, double>>& points) {
    vector<int> order(points.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        return points[i] < points[j];
    });

    auto merge = [&](const vector<int>& A, const vector<int>& B) {
        vector<int> res(A.size() + B.size());
        int A_ptr = 0, B_ptr = 0;
        for (int i = 0; i < (int)res.size(); ++i) {
            if (A_ptr == (int)A.size()) res[i] = B[B_ptr++];
            else if (B_ptr == (int)B.size()) res[i] = A[A_ptr++];
            else if (points[A[A_ptr]].second <= points[B[B_ptr]].second) res[i] = A[A_ptr++];
            else res[i] = B[B_ptr++];
        }
        return res;
    };

    pair<int, int> ans = {0, 1};
    double min_dist = dist(points[ans.first], points[ans.second]);

    auto sort_and_closest_pair = [&](auto& f, int l, int r) -> pair<double, vector<int>> {
        if (l == r) return {1e15, {order[l]}};

        int m = (l + r) / 2;

        auto [d1, A] = f(f, l, m);
        auto [d2, B] = f(f, m + 1, r);
        double d = min(d1, d2);

        auto S = merge(A, B);

        vector<int> S2;
        for (int i : S) {
            double dx = (points[i].first - points[order[m]].first);
            if (dx * dx <= d) S2.emplace_back(i);
        }

        for (int i = 0; i < (int)S2.size(); ++i)
            for (int j = 1; j <= 7; ++j)
                if (i + j < (int)S2.size()) {
                    double cand_d = dist(points[S2[i]], points[S2[i + j]]);
                    if (cand_d < d) {
                        d = cand_d;
                        if (d < min_dist) {
                            ans = {S2[i], S2[i + j]};
                            min_dist = dist(points[ans.first], points[ans.second]);
                        }
                    }
                }

        return {d, S};
    };

    sort_and_closest_pair(sort_and_closest_pair, 0, points.size() - 1);

    return ans;
}

void print_map_link(pair<double, double> p, pair<double, double> q) {
    auto [lat1, lon1] = p;
    auto [lat2, lon2] = q;
    // O formato exige a ordem [longitude, latitude]
    cout << "Link do mapa: "
         << "\033[34mhttps://geojson.io/#data=data:application/json,%7B%22type%22:%22LineString%22,%22coordinates%22:[["
         << lon1 << "," << lat1 << "],[" << lon2 << "," << lat2 << "]]%7D\033[m\n";
}

int main() {
    vector<pair<string, pair<double, double>>> cities = csv_parser();
    
    vector<string> names;
    vector<pair<double, double>> points;
    
    for (const auto& [name, point] : cities) {
        names.emplace_back(name);
        points.emplace_back(point);
    }

    points = spheric_to_cartesian(points);
    
    for (int k = 0; k < 20; ++k) {
        auto [i, j] = closest_pair(points);
        if (i > j) swap(i, j);
        cout << "\033[35m" << names[i] << "\033[m e \033[36m" << names[j] << "\033[m\n";
        cout << "Cerca de " << sqrt(dist(points[i], points[j])) << " km de distância\n\n";
        print_map_link(cartesian_to_spheric(points[i]), cartesian_to_spheric(points[j]));
        names.emplace_back(names[i] + " - " + names[j]); 
        double pt1 = (points[i].first + points[j].first) / 2;
        double pt2 = (points[i].second + points[j].second) / 2; 
        points.emplace_back(pt1, pt2); 
        points.erase(points.begin() + j);
        points.erase(points.begin() + i);
        names.erase(names.begin() + j);
        names.erase(names.begin() + i);
    }
 }
