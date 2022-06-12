#pragma once
#include<bits/stdc++.h>

using namespace std;

class Instance;
class Solution {
public:
    Solution();
    Solution(const Solution& s);
    Solution& operator = (const Solution& s);

    vector<vector<int> > Tours;
    int index_MaxTour;
    double distance_MaxTour;
    vector<double> distance_SubTours;
    vector<pair<int, int> > serviceDetails;

    void update(Instance& instance, int update_iter = 50000);
    void updateMaxTour();
    double getMaxDistanceTour();
    void display(Instance* instance);
    vector<vector<double> > getDistanceMatrixSubTour(int idSubTour, Instance& instance);
    void print();
};
