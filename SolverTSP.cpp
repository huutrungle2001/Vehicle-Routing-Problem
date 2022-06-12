#include "SolverTSP.h"

SolverTSP::SolverTSP(InstanceTSP *instance) : instance(instance){
    cooling_rate = 0.996;
    threshold = 0.2;  //
}

void SolverTSP::solution_construction(){
    bestSol = new SolutionTSP();
    currentSol = new SolutionTSP();
    cheapestHeuristic_random(bestSol);

    *currentSol = *bestSol;
}

void SolverTSP::solve(int iter){
    solution_construction();
    for (int i = 0; i < iter; ++i){
        SolutionTSP s = *currentSol;
        mutate(&s);
        local_search(&s);

        if ((s.distance - currentSol->distance) / currentSol->distance < threshold){
            *currentSol = s;
            if (currentSol->distance - bestSol->distance < -0.0001){
                *bestSol = *currentSol;
            }
        }
        threshold = threshold * cooling_rate;
    }
}

void SolverTSP::local_search(SolutionTSP *s){
    bool improved = true;
    while (improved) {
        improved = false;
        LS_relocate(s, improved);
        LS_swap(s, improved);
        LS_two_opt(s, improved);
    }
}

void SolverTSP::LS_swap(SolutionTSP *s, bool &improved){
here:
    vector<int> &tour = s->tour;
    for (int i = 1; i < tour.size() - 2; ++i){
        for (int j = i + 1; j < tour.size() - 1; ++j){
            double delta;
            int pi = i;
            int pj = j;
            if (j - i > 1){
                delta = -getDistance(tour[pi - 1], tour[pi]) - getDistance(tour[pi], tour[pi + 1]) - getDistance(tour[pj - 1], tour[pj]) - getDistance(tour[pj], tour[pj + 1])

                        + getDistance(tour[pi - 1], tour[pj]) + getDistance(tour[pj], tour[pi + 1]) + getDistance(tour[pj - 1], tour[pi]) + getDistance(tour[pi], tour[pj + 1]);

            }else{
                delta = -getDistance(tour[pi - 1], tour[pi]) - getDistance(tour[pj], tour[pj + 1]) + getDistance(tour[pi - 1], tour[pj]) + getDistance(tour[pi], tour[pj + 1]);
            }
            if (delta < -0.0001){ 
                swap_two_nodes(s, i, j, delta);
                check(s);
                improved = true;
                goto here;
            }
        }
    }
}

void SolverTSP::LS_two_opt(SolutionTSP *s, bool &improved){
here:
    vector<int> &tour = s->tour;
    for (int i = 1; i < tour.size() - 2; ++i){
        for (int j = i + 1; j < tour.size() - 1; ++j){
            double delta;
            int pi = i;
            int pj = j;
            delta = -getDistance(tour[pi - 1], tour[pi]) - getDistance(tour[pj], tour[pj + 1]) + getDistance(tour[pi - 1], tour[pj]) + getDistance(tour[pi], tour[pj + 1]);

            if (delta < -0.0001){  
                two_opt_nodes(s, i, j, delta);
                check(s);
                improved = true;
                goto here;
            }
        }
    }
}

void SolverTSP::LS_relocate(SolutionTSP *s, bool &improved){
here:
    vector<int> &tour = s->tour;
    for (int i = 1; i < tour.size() - 1; ++i){
        int node = tour[i];

        for (int p = 1; p < tour.size(); ++p){
            if (p == i || p == i + 1) continue;
            double delta = getDistance(s->tour[p - 1], node) + getDistance(node, s->tour[p]) - getDistance(s->tour[p - 1], s->tour[p]) + getDistance(s->tour[i - 1], s->tour[i + 1]) - getDistance(s->tour[i], s->tour[i - 1]) - getDistance(s->tour[i], s->tour[i + 1]);
            if (delta < -0.0001){ 
                do_relocate(s, i, p, delta);
                check(s);
                improved = true;
                goto here;
            }
        }
    }
}

void SolverTSP::swap_two_nodes(SolutionTSP *s, int pi, int pj, double delta = 0){
    vector<int> &tour = s->tour;

    if (pi > pj){
        int tmp = pi;
        pi = pj;
        pj = tmp;
    }

    if (delta == 0){
        if (pj - pi > 1){
            delta = -getDistance(tour[pi - 1], tour[pi]) - getDistance(tour[pi], tour[pi + 1]) - getDistance(tour[pj - 1], tour[pj]) - getDistance(tour[pj], tour[pj + 1])

                    + getDistance(tour[pi - 1], tour[pj]) + getDistance(tour[pj], tour[pi + 1]) + getDistance(tour[pj - 1], tour[pi]) + getDistance(tour[pi], tour[pj + 1]);

        }else{
            delta = -getDistance(tour[pi - 1], tour[pi]) - getDistance(tour[pj], tour[pj + 1]) + getDistance(tour[pi - 1], tour[pj]) + getDistance(tour[pi], tour[pj + 1]);
        }
    }

    s->distance += delta;
    int temp = tour[pi];
    tour[pi] = tour[pj];
    tour[pj] = temp;
}

void SolverTSP::two_opt_nodes(SolutionTSP *s, int pi, int pj, double delta){
    vector<int> &tour = s->tour;

    if (pi > pj){
        int tmp = pi;
        pi = pj;
        pj = tmp;
    }
    bool flag = true;
    if (delta == 0) flag = false;

    if (!flag){
        delta = -(getDistance(tour[pi - 1], tour[pi]) + getDistance(tour[pj], tour[pj + 1]));
    }

    reverse(s, pi, pj);

    if (!flag){
        delta += (getDistance(tour[pi - 1], tour[pi]) + getDistance(tour[pj], tour[pj + 1]));
    }

    s->distance += delta;
}

void SolverTSP::reverse(SolutionTSP *s, int pi, int pj){
    vector<int> &tour = s->tour;

    if (pi > pj){
        int tmp = pi;
        pi = pj;
        pj = tmp;
    }

    for (int k = 0; k <= (pj - pi) / 2; ++k){
        int temp = tour[pi + k];
        tour[pi + k] = tour[pj - k];
        tour[pj - k] = temp;
    }
}

void SolverTSP::do_relocate(SolutionTSP *s, int nodePos, int newPos, double delta){
    int node = s->tour[nodePos];
    if (newPos > nodePos){
        s->tour.insert(s->tour.begin() + newPos, node);
        s->tour.erase(s->tour.begin() + nodePos);
    } else {
        s->tour.erase(s->tour.begin() + nodePos);
        s->tour.insert(s->tour.begin() + newPos, node);
    }
    s->distance += delta;
}

void SolverTSP::check(SolutionTSP *s){
    double d = 0;
    for (int i = 0; i < s->tour.size() - 1; ++i){
        d += getDistance(s->tour[i], s->tour[i + 1]);
    }
    assert(abs(d - s->distance) < 0.00001);
}

void SolverTSP::mutate(SolutionTSP *s){
    int numSwap = Utils::integer_random_generator(1, 4);
    for (int i = 0; i < numSwap; ++i){
        int p1, p2;
        do {
            p1 = Utils::integer_random_generator(1, s->tour.size() - 1);
            p2 = Utils::integer_random_generator(1, s->tour.size() - 1);
        } while (p1 == p2);

        swap_two_nodes(s, p1, p2);
        check(s);
    }
}

void SolverTSP::nearestHeuristic(SolutionTSP *s){
    unordered_set<int> NodeList;
    for (int i = 1; i < instance->nodes; ++i)
        NodeList.insert(i);

    int last = 0;
    while (!NodeList.empty()){
        int nearest = find_nearest(NodeList, last);
        insertNode(s, nearest, s->tour.size() - 1);
        NodeList.erase(nearest);
        s->print();
        last = nearest;
    }
}

void SolverTSP::cheapestHeuristic(SolutionTSP *s){
    unordered_set<int> NodeList;
    for (int i = 1; i < instance->nodes; ++i)
        NodeList.insert(i);

    while (!NodeList.empty()){
        int bestNode = -1;
        int bestPosition = -1;
        double bestCost = 1e5;

        for (int node : NodeList){
            for (int position = 1; position < s->tour.size(); ++position){
                double cost_gain = getDistance(s->tour[position - 1], node) + getDistance(node, s->tour[position]) - getDistance(s->tour[position - 1], s->tour[position]);
                if (cost_gain < bestCost){
                    bestNode = node;
                    bestPosition = position;
                    bestCost = cost_gain;
                }
            }
        }
        insertNode(s, bestNode, bestPosition);
        NodeList.erase(bestNode);
    }
}

void SolverTSP::cheapestHeuristic_random(SolutionTSP *s){
    vector<int> NodeList;
    for (int i = 1; i < instance->nodes; ++i)
        NodeList.push_back(i);
    Utils::shuffle(NodeList);

    for (int node : NodeList){
        int bestPosition = -1;
        double bestCost = 1e5;
        for (int position = 1; position < s->tour.size(); ++position) {
            double cost_gain = getDistance(s->tour[position - 1], node) + getDistance(node, s->tour[position]) - getDistance(s->tour[position - 1], s->tour[position]);
            if (cost_gain < bestCost){
                bestPosition = position;
                bestCost = cost_gain;
            }
        }
        insertNode(s, node, bestPosition);
    }
}

double SolverTSP::getDistance(int i, int j){
    return instance->getDistance(i, j);
}

int SolverTSP::find_nearest(unordered_set<int> &list, int i){
    double bestDis = 999999;
    int nearest = -1;
    for (int node : list){
        double d = instance->getDistance(i, node);
        if (d < bestDis){
            bestDis = d;
            nearest = node;
        }
    }
    return nearest;
}

void SolverTSP::insertNode(SolutionTSP *s, int node, int position){
    s->distance += instance->getDistance(s->tour[position - 1], node) + instance->getDistance(s->tour[position], node) - instance->getDistance(s->tour[position - 1], s->tour[position]);
    s->tour.insert(s->tour.begin() + position, node);
}

double const SolverTSP::getDistanceMinimumTour(){
    return bestSol->getDistance();
}