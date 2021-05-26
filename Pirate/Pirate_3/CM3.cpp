#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>

//18706.339766203535

inline void reverse(std::vector<int>& array, int start, int end) {
    if (start > end) {
        int temp = end;
        end = start;
        start = temp;
    }

    int x_1 = start;
    int x_2 = end;
    while (x_2 > x_1)
    {
        int temp = array[x_2];
        array[x_2] = array[x_1];
        array[x_1] = temp;
        ++x_1;
        --x_2;
    }
}

struct Ret {
    std::vector<int> tour;
    double S;
};

struct Combo {
    int first;
    int second;
    int third;
    int type;
};

Ret opt3(std::vector<int>& tour, double eps, double Sum, const std::vector<std::vector<double>>& A) {
    int N = tour.size();
    bool flag = true;
    while(flag) {
        Combo best_combo = {0, 0, 0, 0};
        double best_delta = 0.0;

        for (int a = 0; a+5 < N; a++) {
            for (int b = a + 1; b + 3 < N; b++) {
                for (int c = b + 1; c + 1 < N; c++) {
                    int _A = tour[a];
                    int _B = tour[a+1];
                    int _C = tour[b];
                    int _D = tour[b+1];
                    int _E = tour[c];
                    int _F = tour[c+1];
                    double d0 = A[_A][_B] + A[_C][_D] + A[_E][_F];
                    double d1 = A[_A][_C] + A[_B][_D] + A[_E][_F];
                    double d2 = A[_A][_B] + A[_C][_E] + A[_D][_F];
                    double d3 = A[_A][_D] + A[_E][_B] + A[_C][_F];
                    double d4 = A[_F][_B] + A[_C][_D] + A[_E][_A];

                    if ((d1 - d0 < best_delta) && (d1 - d0 < -eps)) {
                        best_delta = d1-d0;
                        best_combo = {a, b, c, 1};
                    }
                    if ((d2 - d0 < best_delta) && (d2 - d0 < -eps)) {
                        best_delta = d2-d0;
                        best_combo = {a, b, c, 2};
                    }
                    if ((d4 - d0 <best_delta) && (d4 - d0 < -eps)) {
                        best_delta = d4-d0;
                        best_combo = {a, b, c, 3};
                    }
                    if ((d3 - d0 < best_delta) && (d3 - d0 < -eps)) {
                        best_delta = d3-d0;
                        best_combo = {a, b, c, 4};
                    }
                }
            }
        }
        //std::cout << best_delta << "\n";
        Sum += best_delta;
        if (best_combo.type == 0) {
            flag = false;
        } else if (best_combo.type == 1) {
            reverse(tour, best_combo.first+1, best_combo.second);
        } else if (best_combo.type == 2) {
            reverse(tour, best_combo.second+1, best_combo.third);
        } else if (best_combo.type == 3) {
            reverse(tour, best_combo.first+1, best_combo.third);
        } else if (best_combo.type == 4) {
            std::vector<int> tmp(tour.size());

            int s = 0;
            for (int i = 0; i < best_combo.first; i++) {
                tmp[s+i] = tour[i];
            }
            s += best_combo.first+1;
            for (int i = 0; i < best_combo.third - best_combo.second; i++) {
                tmp[s+i] = tour[best_combo.second+1+i];
            }
            s += best_combo.third - best_combo.second;
            for (int i = 0; i < best_combo.second - best_combo.first; i++) {
                tmp[s+i] = tour[best_combo.first+1+i];
            }
            s += best_combo.second - best_combo.first;
            for (int i = 0; i < tour.size() - best_combo.third - 1; i++) {
                tmp[s+i] = tour[best_combo.third+1+i];
            }

            tour = tmp;
        }
    }
    return {tour, Sum};
}


int main() {
    std::fstream s("dataset_328552_4.txt");

    std::vector<int> m;
    std::vector<double> x, y;
    int n, tmp_p, l, k, v;
    double tmp_x, tmp_y;


    s >> n >> l>> k >> v;
    //std::cin >> n >> l >> k >> v;

    for (int i = 0; i < n; i++) {
        s >> tmp_x >> tmp_y >> tmp_p;
        //std::cin >> tmp_x >> tmp_y >> tmp_p;
        m.push_back(tmp_p);
        x.push_back(tmp_x);
        y.push_back(tmp_y);
    }

    

    std::vector<std::vector<double>> A(n, std::vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double res = sqrt((x[i]-x[j]) * (x[i]-x[j]) + (y[i]-y[j]) * (y[i]-y[j]));
            A[i][j] = A[j][i] = res;
        }
    }

    

    std::vector<int> best;

    std::vector<bool> flags(n, true);
    flags[0] = false;
    std::vector<int> way;
    double S = 0;
    way.push_back(0);

    int M = 0;

    

    for (int i = 1; i < n; i++) {
        int best_ind = n+1;
        double best_dist = 1e27;

        for (int j = 0; j < n; j++) {
            if (flags[j] == true && way[i-1] != j) {
                if (A[way[i-1]][j] < best_dist) {
                    best_dist = A[way[i - 1]][j];
                    best_ind = j;
                }
            }
        }

        way.push_back(best_ind);
        //M += m[best_ind]
        S += best_dist;
        flags[best_ind] = false;
    }

    

    S += A[way[n-1]][way[0]];

    for (const auto& item : way) {
        std::cout << item+1 << " ";
    }
    std::cout << way[0]+1;
    std::cout << "\n" << S << "\n";
}