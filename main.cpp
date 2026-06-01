#include "csv_parser.hpp"

#include <bits/stdc++.h>
using namespace std;

float to_radians(float degree) { return degree * M_PI / 180.0; }

float dist(pair<float, float> A, pair<float, float> B) {
    return hypot(abs(A.first - B.first), abs(A.second - B.second));
}

// float dist(pair<float, float> A, pair<float, float> B) {
//     auto [lat1, lon1] = A;
//     auto [lat2, lon2] = B;
//     const float R = 6371.0; // Raio da Terra em km

//     float rLat1 = to_radians(lat1);
//     float rLon1 = to_radians(lon1);
//     float rLat2 = to_radians(lat2);
//     float rLon2 = to_radians(lon2);

//     float dLat = rLat2 - rLat1;
//     float dLon = rLon2 - rLon1;

//     // Fórmula Haversine
//     float a = pow(sin(dLat / 2), 2) + cos(rLat1) * cos(rLat2) * pow(sin(dLon / 2), 2);
//     float c = 2 * atan2(sqrt(a), sqrt(1 - a));

//     return R * c;
// }

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
    
    auto sort_and_closest_pair = [&](auto& f, int l, int r) -> pair<float, vector<int>> {
        if (l == r) return {1e15, {order[l]}};
        
        int m = (l + r) / 2;
        
        auto [d1, A] = f(f, l, m);
        auto [d2, B] = f(f, m + 1, r);
        float d = min(d1, d2);

        auto S = merge(A, B);
        
        vector<int> S2;
        for (int i : S)
            if (abs(points[i].first - points[order[m]].first) <= d)
                S2.emplace_back(i);

        for (int i = 0; i < (int)S2.size(); ++i)
            for (int j = 1; j <= 7; ++j)
                if (i + j < (int)S2.size()) {
                    float cand_d = dist(points[S2[i]], points[S2[i + j]]);
                    if (cand_d < d) {
                        d = cand_d;
                        if (dist(points[S2[i]], points[S2[i + j]]) < dist(points[ans.first], points[ans.second]))
                            ans = {S2[i], S2[i + j]};
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

    // float d = INT_MAX;
    // for (int i = 0; i < points.size(); ++i)
    //     for (int j = i + 1; j < points.size(); ++j)
    //         d = min(d, dist(points[i], points[j]));
    // cout << d << '\n';

    auto [i, j] = closest_pair(points);
    cout << names[i] << ' ' << names[j] << '\n';
    cout << "Cerca de " << dist(points[i], points[j]) << "km de distância\n";
}