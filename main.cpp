#include "csv_parser.hpp"

#include <bits/stdc++.h>
using namespace std;

double to_radians(double degree) { return degree * M_PI / 180.0; }

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

vector<pair<double, double>> spheric_to_cartesian(const vector<pair<double, double>>& points) {
    double mean_lat = 0;
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

int main() {
    vector<pair<string, pair<double, double>>> cities = csv_parser();
    vector<string> names;
    vector<pair<double, double>> points;
    for (const auto& [name, point] : cities) {
        names.emplace_back(name);
        points.emplace_back(point);
    }

    points = spheric_to_cartesian(points);
    
    for (int k= 0; k < 20; ++k) {
        auto [i, j] = closest_pair(points);
        if (i > j) swap(i, j);
        cout << names[i] << ' ' << names[j] << '\n';
        cout << "Cerca de " << dist(points[i], points[j]) << "km de distância\n\n";
        names.emplace_back(names[i]+" - "+names[j]); 
        int pt1 = (points[i].first+points[j].first)/2;
        int pt2 = (points[i].second+points[j].second)/2; 
        points.emplace_back(pt1, pt2); 
        points.erase(points.begin() + j);
        points.erase(points.begin() + i);
        names.erase(names.begin() + j);
        names.erase(names.begin() + i);
    }
 }
