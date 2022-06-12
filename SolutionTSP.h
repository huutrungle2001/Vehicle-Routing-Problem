#pragma once
#include "Utils.h"
#include<bits/stdc++.h>

using namespace std;

class InstanceTSP;
class SolutionTSP {
public:
    SolutionTSP();
    SolutionTSP(const SolutionTSP& s);
    SolutionTSP& operator=(const SolutionTSP& s);
    vector<int> tour;
    double distance;
    double getDistance();
    void display(InstanceTSP* instance);
    void print();
};
