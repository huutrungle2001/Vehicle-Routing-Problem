#ifndef UTILS_H
#define UTILS_H
#include<bits/stdc++.h>

using namespace std;

class Utils {
public:
    static mt19937 mt;
    static int seed;

    static int integer_random_generator(const int &a , const int &b );
    static double real_random_generator(const double& a, const double& b);
    static void shuffle(vector<int> &vec);
};

#endif // UTILS_H