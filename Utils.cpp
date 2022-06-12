#include "Utils.h"
#include<bits/stdc++.h>

using namespace std;

int Utils::seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 Utils::mt = mt19937(Utils::seed);

int Utils::integer_random_generator(const int &a, const int &b){
    if (b <= a) throw string("ERROR | int random problem");
    return uniform_int_distribution<int>{a, b - 1}(Utils::mt);
}

double Utils::real_random_generator(const double &a, const double &b){
    if (b <= a) throw string("ERROR | double random problem");
    return uniform_real_distribution<double>{a, b}(Utils::mt);
}

void Utils::shuffle(vector<int> &vec){
    std::shuffle(vec.begin(), vec.end(), Utils::mt);
}
