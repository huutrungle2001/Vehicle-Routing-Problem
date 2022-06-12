#pragma once
#include "Config.h"
#include "InstanceTSP.h"
#include "SolverTSP.h"
#include<bits/stdc++.h>

#define matrix vector<vector<double> >

using namespace std;

class TSP {
public:
    double min_distance;
    vector<int> Tour;
    double Run_TSP(matrix distance_matrix, int iter = 50000) {
        InstanceTSP instanceTSP(distance_matrix);
        SolverTSP s(&instanceTSP);
        s.solve(iter);
        min_distance = s.getDistanceMinimumTour();
        Tour = s.bestSol->tour;
        return min_distance;
    }
};
