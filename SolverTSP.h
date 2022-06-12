#pragma once
#include "InstanceTSP.h"
#include "SolutionTSP.h"
#include<bits/stdc++.h>

class SolverTSP {
public:
    double threshold;
    double cooling_rate;
    InstanceTSP *instance;
    SolutionTSP *bestSol;
    SolutionTSP *currentSol;
    SolverTSP(InstanceTSP *instance);
    void solution_construction();
    void solve(int iter = 5000);
    void mutate(SolutionTSP *s);

    void insertNode(SolutionTSP *s, int node, int position);
    void nearestHeuristic(SolutionTSP *s);
    void cheapestHeuristic(SolutionTSP *s);
    void cheapestHeuristic_random(SolutionTSP *s);
    int find_nearest(unordered_set<int> &list, int i);

    double getDistance(int i, int j);
    void local_search(SolutionTSP *s);
    void LS_swap(SolutionTSP *s, bool &improved);
    void LS_relocate(SolutionTSP *s, bool &improved);
    void LS_two_opt(SolutionTSP *s, bool &improved);

    void swap_two_nodes(SolutionTSP *s, int pi, int pj, double delta);
    void reverse(SolutionTSP *s, int pi, int pj);
    void two_opt_nodes(SolutionTSP *s, int pi, int pj, double delta);

    void do_relocate(SolutionTSP *s, int nodePos, int newPos, double delta);
    void check(SolutionTSP *s);

    double const getDistanceMinimumTour();
};
