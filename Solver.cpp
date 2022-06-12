#include "Solver.h"
#include<bits/stdc++.h>

using namespace std;

Solver::Solver(Instance *instance, int timeLimit, int iter) : instance(instance), timeLimit(timeLimit), iter(iter){
    cooling_rate = 0.996;
    threshold = 0.2;
}

void Solver::solution_construction(){
    bestSol = new Solution();
    currentSol = new Solution();
    nearestHeuristic(bestSol);
    *currentSol = *bestSol;
}

void Solver::solve(int iter){
    solution_construction();
    double min_dist = bestSol->getMaxDistanceTour();
    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < iter; ++i){
        auto now = chrono::high_resolution_clock::now();
        double runtime = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
        runtime = runtime / 1000;
        if (runtime > timeLimit) break;

        Solution *s = new Solution();
        *s = *currentSol;
        s->print();
        if (i > 0) {
            int n = rand() % 4;
            if (n == 0) nearestHeuristic(s);
            else mutate(s);
        }

        local_search(s);
        if ((s->getMaxDistanceTour() - currentSol->getMaxDistanceTour()) / currentSol->getMaxDistanceTour() < threshold){
            *currentSol = *s;
            if (currentSol->getMaxDistanceTour() - bestSol->getMaxDistanceTour() < -0.0001) {
                *bestSol = *currentSol;
                cout << i << " - Found improvement " << bestSol->getMaxDistanceTour() << endl;
            }
        }
        threshold = threshold * cooling_rate;
    }
    cout << "Min : " << bestSol->getMaxDistanceTour() << "\n";
    cout << "Done C-VRP Local Search!\n";
}

void Solver::local_search(Solution *s){
    bool improved = true;
    while (improved) {
        improved = false;
        LS_relocate(s, improved);
        LS_swap(s, improved);
    }
}

void Solver::LS_swap(Solution *s, bool &improved){
here:
    int step = 1;
    int index_MaxTour = s->index_MaxTour;
    vector<int> Tour_max = s->Tours[index_MaxTour];
    vector<int> indexTour;
    for (int i = 0; i < s->Tours.size(); i++){
        if (i == index_MaxTour) continue;
        indexTour.emplace_back(i);
    }

    Utils::shuffle(indexTour);

    for (auto i : indexTour){
        vector<int> tour_i = s->Tours[i];
        for (auto &j : tour_i){
            if (j == 0) continue;
            for (auto &k : Tour_max){
                if (step == 250) return;
                if (k == 0) continue;
                swap(j, k);
                TSP tsp_max, tsp_i;
                cout << "\x1B[2K\rStep:" << step++ << "\r\t\t\tSwap "
                     << "\r\t\t\t\t\t\t";
                if (step % 4 == 0)
                    cout << ".";
                else if (step % 4 == 1)
                    cout << "..";
                else if (step % 4 == 2)
                    cout << "...";
                else
                    cout << "....";
                tsp_max.Run_TSP(getDistanceMatrixTour(Tour_max), 2000);
                tsp_i.Run_TSP(getDistanceMatrixTour(tour_i), 2000);
                if (tsp_max.min_distance + 0.1 < s->getMaxDistanceTour() && tsp_i.min_distance + 0.1 < s->getMaxDistanceTour()){
                    vector<int> new_tour_i = vector<int>();
                    for (auto ind_client : tsp_i.Tour) {
                        new_tour_i.emplace_back(tour_i[ind_client]);
                    }
                    vector<int> new_tour_max = vector<int>();
                    for (auto ind_client : tsp_max.Tour) {
                        new_tour_max.emplace_back(Tour_max[ind_client]);
                    }
                    s->Tours[i].clear();
                    s->Tours[i] = new_tour_i;
                    s->distance_SubTours[i] = tsp_i.min_distance;
                    s->Tours[index_MaxTour].clear();
                    s->Tours[index_MaxTour] = new_tour_max;
                    s->distance_SubTours[index_MaxTour] = tsp_max.min_distance;
                    s->distance_MaxTour = tsp_max.min_distance;
                    s->updateMaxTour();
                    cout << "\tMin i: " << tsp_i.min_distance << "\t Min max: " << tsp_max.min_distance << "\t";
                    cout << "Max afer Swap: " << s->getMaxDistanceTour() << "\n\n";
                    s->print();
                    cout << "===============================\n\n";
                    improved = true;
                    goto here;
                }
                swap(j, k);
            }
        }
    }
    cout << "\n";
}

void Solver::LS_relocate(Solution *s, bool &improved){
here:
    int step = 1;
    int index_MaxTour = s->index_MaxTour;
    vector<int> Tour_max = s->Tours[index_MaxTour];
    vector<int> indexTour;
    for (int i = 0; i < s->Tours.size(); i++){
        if (i == index_MaxTour) continue;
        indexTour.emplace_back(i);
    }

    Utils::shuffle(indexTour);

    for (auto i : indexTour){
        vector<int> tour_i = s->Tours[i];
        if (tour_i.size() - 2 >= instance->clients) continue;
        for (int k = 1; k < Tour_max.size() - 1; ++k){
            if (step == 250) return;

            int node_cur = Tour_max[k];
            Tour_max.erase(Tour_max.begin() + k);
            tour_i.insert(tour_i.begin() + tour_i.size() - 1, node_cur);
            TSP tsp_max, tsp_i;
            cout << "\x1B[2K\rStep:" << step++ << "\r\t\t\tRelocate\t" << i << "-" << k << "\r\t\t\t\t\t\t";
            if (step % 4 == 0)
                cout << ".";
            else if (step % 4 == 1)
                cout << "..";
            else if (step % 4 == 2)
                cout << "...";
            else
                cout << "....";
            tsp_max.Run_TSP(getDistanceMatrixTour(Tour_max), 2000);
            tsp_i.Run_TSP(getDistanceMatrixTour(tour_i), 2000);
            if (tsp_max.min_distance + 0.1 < s->getMaxDistanceTour() && tsp_i.min_distance + 0.1 < s->getMaxDistanceTour()){
                vector<int> new_tour_i = vector<int>();
                for (auto ind_client : tsp_i.Tour){
                    new_tour_i.emplace_back(tour_i[ind_client]);
                }
                vector<int> new_tour_max = vector<int>();
                for (auto ind_client : tsp_max.Tour){
                    new_tour_max.emplace_back(Tour_max[ind_client]);
                }
                s->Tours[i].clear();
                s->Tours[i] = new_tour_i;
                s->distance_SubTours[i] = tsp_i.min_distance;
                s->Tours[index_MaxTour].clear();
                s->Tours[index_MaxTour] = new_tour_max;
                s->distance_SubTours[index_MaxTour] = tsp_max.min_distance;
                s->distance_MaxTour = tsp_max.min_distance;
                s->updateMaxTour();
                cout << "\tMin i: " << tsp_i.min_distance << "\t Min max: " << tsp_max.min_distance << "\t";
                cout << "Max afer Relocate: " << s->getMaxDistanceTour() << "\n\n";
                s->print();
                cout << "===============================\n";
                improved = true;
                goto here;
            }
            Tour_max.insert(Tour_max.begin() + k, node_cur);
            tour_i.erase(tour_i.begin() + tour_i.size() - 2);
        }
    }
    cout << "\n";
}

void Solver::nearestHeuristic(Solution *s){
    vector<pair<double, int> > lists_point; 
    int id_point_milestone = rand() % (instance->nodes - 1) + 1;

    cout << "Milestone: " << id_point_milestone << endl;
    for (int i = 1; i < instance->nodes; ++i){
        double alpha = getAngleXOY(instance->getPoint(i), instance->getPoint(id_point_milestone), instance->getPoint(0));
        lists_point.push_back({alpha, i});
    }

    sort(lists_point.begin(), lists_point.end());

    vector<int> clients = vector<int>(instance->trucks, (int)(instance->nodes - 1) / instance->trucks - 2);
    int tmp = (instance->nodes - 1) % instance->trucks + 2 * instance->trucks;
    int index_id_clients;
    while (tmp > 0) {
        index_id_clients = rand() % clients.size();
        if (clients[index_id_clients] < instance->clients) {
            ++clients[index_id_clients];
            --tmp;
        }
    }

    Utils::shuffle(clients);

    for (auto i : clients){
        cout << i << " -> ";
    }
    cout << "\n";

    index_id_clients = 0;
    s->Tours.clear();
    int index_subTours = 0;
    for (auto number : clients){
        tmp = number;
        s->Tours.push_back({});
        s->Tours[index_subTours].push_back(0);
        while (tmp--) {
            s->Tours[index_subTours].emplace_back(lists_point[index_id_clients++].second);
        }
        s->Tours[index_subTours].emplace_back(0);
        ++index_subTours;
    }

    s->update(*instance, 5000);
    s->print();
    cout << "Done Nearest Heuristic !\n";
    cout << "-------------------------------\n";
}

double Solver::getAngleXOY(pair<double, double> const &X, pair<double, double> const &Y, pair<double, double> const O){
    double d_x = (X.first - O.first) * (X.first - O.first) + (X.second - O.second) * (X.second - O.second);
    d_x = sqrt(d_x);
    double d_y = (Y.first - O.first) * (Y.first - O.first) + (Y.second - O.second) * (Y.second - O.second);
    d_y = sqrt(d_y);
    double cos_XOY = ((X.first - O.first) * (Y.first - O.first) + (X.second - O.second) * (Y.second - O.second)) / (d_x * d_y);
    if (cos_XOY < -1)
        cos_XOY += MY_EPSILON;
    else if (cos_XOY > 1)
        cos_XOY -= MY_EPSILON;
    return acos(cos_XOY);
}

void Solver::TSP_start_sol(Solution *s){
    vector<int> clients = vector<int>(instance->trucks, (int)(instance->nodes - 1) / instance->trucks - 2);
    int tmp = (instance->nodes - 1) % instance->trucks + 2 * instance->trucks;
    int index_id_clients;
    while (tmp > 0) {
        index_id_clients = rand() % clients.size();
        if (clients[index_id_clients] < instance->clients) {
            ++clients[index_id_clients];
            --tmp;
        }
    }

    Utils::shuffle(clients);

    for (auto i : clients){
        cout << i << " -> ";
    }
    cout << "\n";

    TSP tsp;
    tsp.Run_TSP(instance->dist_matrix, 3000);
    cout << "TSP: " << tsp.min_distance << "\n";
    index_id_clients = 1;
    s->Tours.clear();
    int index_subTours = 0;
    for (auto number : clients){
        tmp = number;
        s->Tours.push_back({});
        s->Tours[index_subTours].push_back(0);
        while (tmp--) {
            s->Tours[index_subTours].emplace_back(tsp.Tour[index_id_clients++]);
        }
        s->Tours[index_subTours].emplace_back(0);
        ++index_subTours;
    }

    s->update(*instance);
    s->print();
}

vector<vector<double> > Solver::getDistanceMatrixTour(vector<int> Tour){
    vector<vector<double> > distance_Matrix = vector<vector<double> >();
    for (int i = 0; i < Tour.size() - 1; ++i){
        distance_Matrix.push_back({});
        for (int j = 0; j < Tour.size() - 1; ++j){
            distance_Matrix[i].emplace_back(instance->getDistance(Tour[i], Tour[j]));
        }
    }
    return distance_Matrix;
}

void Solver::mutate(Solution *s){
    int numSwap = Utils::integer_random_generator(1, 4);
    for (int i = 0; i < numSwap; ++i){
        int p1, p2;
        do{
            p1 = Utils::integer_random_generator(0, s->Tours.size());
            p2 = Utils::integer_random_generator(0, s->Tours.size());
        } while (p1 == p2);

        swap_two_nodes(s, p1, p2);
    }
    s->update(*instance, 1000);
}

void Solver::swap_two_nodes(Solution *s, int id_tour_i, int id_tour_j){
    int numSwap = Utils::integer_random_generator(1, 2);
    for (int i = 0; i < numSwap; ++i){
        int p1, p2;
        do{
            p1 = Utils::integer_random_generator(1, s->Tours[id_tour_i].size() - 1);
            p2 = Utils::integer_random_generator(1, s->Tours[id_tour_j].size() - 1);
        } while (p1 == p2);
        swap(s->Tours[id_tour_i][p1], s->Tours[id_tour_j][p2]);
    }
}
