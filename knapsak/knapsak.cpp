#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
int best_price = 0;

struct Interval {
    double coast;
    int weight;
    int value;
};

// Compares two intervals
// according to staring times.
bool compareInterval(Interval i1, Interval i2)
{
    return (i1.coast > i2.coast);
}

void bounds(int k, int max_k, int weight_can, Interval* data, int price, int can_be)
{
    can_be = 0;
    int we = weight_can;
    for (int i = k; i < max_k; i++)
    {
        if (we >= data[i].weight)
        {
            we -= data[i].weight;
            can_be += data[i].value;
        }
        else
        {
            can_be += int(data[i].coast * we);
        }
    }
    if (k < max_k && can_be + price > best_price)
    {
        can_be -= data[k].value;
        bounds(k + 1, max_k, weight_can, data, price, can_be);
        if (data[k].weight <= weight_can)
        {
            weight_can -= data[k].weight;
            price += data[k].value;
            if (price > best_price)
                best_price = price;
            bounds(k + 1, max_k, weight_can, data, price, can_be);
        }

    }
}

int main()
{
    int max_weight = 0, n_elem = 0;
    cin >> max_weight;
    cin >> n_elem;
    Interval mass_weight_val[n_elem];
    for (int i = 0; i < n_elem; i++)
    {
        cin >> mass_weight_val[i].weight >> mass_weight_val[i].value;
        mass_weight_val[i].coast = 1.0 * mass_weight_val[i].value / mass_weight_val[i].weight;
    }

    // sort the intervals in increasing order of
    // start time
    /*for (int i = 0; i < n; i++)
        cout << "[" << mass_weight_val[i].coast << "," << mass_weight_val[i].weight << "," << mass_weight_val[i].value
             << "] ";*/
             //cout << "\n after sort \n";
    sort(mass_weight_val, mass_weight_val + n_elem, compareInterval);
    /*for (int i = 0; i < n; i++)
        cout << "[" << mass_weight_val[i].coast << "," << mass_weight_val[i].weight << "," << mass_weight_val[i].value
             << "] ";*/
    int low_opt = 0;
    int wight = max_weight;
    int can_be = 0;
    for (int i = 0; i < n_elem; i++)
    {
        can_be += mass_weight_val[i].value;
        if (wight >= mass_weight_val[i].weight)
        {
            wight -= mass_weight_val[i].weight;
            low_opt += mass_weight_val[i].value;
        }
    }
    best_price = low_opt;
    bounds(0, n_elem, max_weight, mass_weight_val, 0, can_be);
    cout << best_price;
    return 0;

}
