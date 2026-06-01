#include "csv_parser.hpp"

#include <bits/stdc++.h>
using namespace std;

float to_radians(float degree) { return degree * M_PI / 180.0; }

float dist(pair<float, float> A, pair<float, float> B) {
    return (A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second);
}

pair<int, int> closest_pair(const vector<pair<float, float>>& points) {
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
    float min_dist = dist(points[ans.first], points[ans.second]);

    auto sort_and_closest_pair = [&](auto& f, int l, int r) -> pair<float, vector<int>> {
        if (l == r) return {1e15, {order[l]}};

        int m = (l + r) / 2;

        auto [d1, A] = f(f, l, m);
        auto [d2, B] = f(f, m + 1, r);
        float d = min(d1, d2);

        auto S = merge(A, B);

        vector<int> S2;
        for (int i : S) {
            float dx = (points[i].first - points[order[m]].first);
            if (dx * dx <= d) S2.emplace_back(i);
        }

        for (int i = 0; i < (int)S2.size(); ++i)
            for (int j = 1; j <= 7; ++j)
                if (i + j < (int)S2.size()) {
                    float cand_d = dist(points[S2[i]], points[S2[i + j]]);
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

vector<pair<float, float>> spheric_to_cartesian(const vector<pair<float, float>>& points) {
    float mean_lat = 0;
    for (auto [lat, lon] : points) mean_lat += lat;
    mean_lat = to_radians(mean_lat / points.size());

    const float R = 6371.0;
    vector<pair<float, float>> cartesian_points;

    for (auto [lat, lon] : points) {
        float lat_rad = to_radians(lat);
        float lon_rad = to_radians(lon);

        float x = R * lon_rad * cos(mean_lat);
        float y = R * lat_rad;

        cartesian_points.emplace_back(x, y);
    }

    return cartesian_points;
}

int main() {
    vector<pair<string, pair<float, float>>> cities = csv_parser();
    vector<string> names;
    vector<pair<float, float>> points;
    for (const auto& [name, point] : cities) {
        names.emplace_back(name);
        points.emplace_back(point);
    }

    points = spheric_to_cartesian(points);

    auto [i, j] = closest_pair(points);
    cout << names[i] << ' ' << names[j] << '\n';
    cout << "Cerca de " << sqrt(dist(points[i], points[j])) << "km de distância\n";
}
