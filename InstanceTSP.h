#pragma once
#include<bits/stdc++.h>

#define MY_EPSILON 0.0001  

using namespace std;

class InstanceTSP {
public:
    int nodes;
    vector<double> x;
    vector<double> y;
    vector<vector<double> > dist_matrix;
    InstanceTSP(string instanceName);
    InstanceTSP(vector<vector<double> > dist_matrix);
    void read_input(const string &FileInput);
    void initialize();
    double getDistance(int i, int j);
};
