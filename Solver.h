#pragma once
#include <bits/stdc++.h>

#include "Instance.h"
#include "Solution.h"
#include "TSP.h"

using namespace std;

class Solver {
   public:
    int iter;
    int timeLimit;
    double threshold;
    double cooling_rate;
    Instance *instance;
    Solution *bestSol;
    Solution *currentSol;
    Solver(Instance *instance, int timeLimit, int iter);
    void solution_construction();
    void solve(int iter);
    void mutate(Solution *s);

    double getDistance(int i, int j);
    void local_search(Solution *s);
    void LS_swap(Solution *s, bool &improved);
    void LS_relocate(Solution *s, bool &improved);
    void do_relocate(Solution *s, int nodePos, int newPos, double delta);
    void check(Solution *s);

    void nearestHeuristic(Solution *s);
    double getAngleXOY(pair<double, double> const &X, pair<double, double> const &Y, pair<double, double> const O = {0, 0});
    void TSP_start_sol(Solution *s);

    vector<vector<double> > getDistanceMatrixTour(vector<int> Tour);
    void swap_two_nodes(Solution *s, int pi, int pj);
};