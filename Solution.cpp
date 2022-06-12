#include "Solution.h"
#include "Instance.h"
#include "TSP.h"
#include<bits/stdc++.h>

using namespace std;

Solution::Solution(){
    Tours = {};
    index_MaxTour = 0;
    distance_MaxTour = 0;
    distance_SubTours.resize(100);
}

Solution::Solution(const Solution &s){
    Tours = s.Tours;
    index_MaxTour = s.index_MaxTour;
    distance_MaxTour = s.distance_MaxTour;
    serviceDetails = s.serviceDetails;
}

Solution &Solution::operator=(const Solution &s){
    Tours.clear();
    Tours = s.Tours;
    index_MaxTour = s.index_MaxTour;
    distance_MaxTour = s.distance_MaxTour;
    distance_SubTours = s.distance_SubTours;
    serviceDetails = s.serviceDetails;
    return *this;
}

double Solution::getMaxDistanceTour(){
    return distance_MaxTour;
}

void Solution::update(Instance& instance, int update_iter){
    if (distance_SubTours.size() != Tours.size()){
        distance_SubTours.resize(Tours.size());
    }
    if (serviceDetails.size() != (instance.nodes)){
        serviceDetails.resize(instance.nodes);
    }
    index_MaxTour = 0;
    distance_MaxTour = 0;

    double distance;
    for (int index_SubTours = 0; index_SubTours < Tours.size(); ++index_SubTours){
        cout << "++++++++++++++++++++++++++++++++\n";
        cout << "TSP Tour " << index_SubTours + 1 << ":\n";
        vector<int> tour_i = Tours[index_SubTours];
        TSP tsp_;
        tsp_.Run_TSP(getDistanceMatrixSubTour(index_SubTours, instance), update_iter);
        distance = tsp_.min_distance;
        vector<int> new_tour_i = vector<int>();
        for (auto ind_client : tsp_.Tour){
            new_tour_i.emplace_back(tour_i[ind_client]);
        }
        Tours[index_SubTours] = new_tour_i;
        distance_SubTours[index_SubTours] = distance;
        if (distance > distance_MaxTour){
            index_MaxTour = index_SubTours;
            distance_MaxTour = distance;
        }
    }
}

void Solution::updateMaxTour(){
    for (int i = 0; i < Tours.size(); ++i){
        if (distance_SubTours[i] > distance_MaxTour){
            index_MaxTour = i;
            distance_MaxTour = distance_SubTours[i];
        }
    }
}

vector<vector<double> > Solution::getDistanceMatrixSubTour(int idSubTour, Instance& instance){
    vector<vector<double> > distance_Matrix = vector<vector<double> >();
    for (int i = 0; i < Tours[idSubTour].size() - 1; ++i){
        distance_Matrix.push_back({});
        for (int j = 0; j < Tours[idSubTour].size() - 1; ++j){
            distance_Matrix[i].emplace_back(instance.getDistance(Tours[idSubTour][i], Tours[idSubTour][j]));
        }
    }
    return distance_Matrix;
}

void Solution::print(){
    int index = 0;

    for (auto &i : Tours){
        cout << "Tour " << index + 1 << ": \t" << distance_SubTours[index] << "\t\t";
        for (auto &j : i) {
            printf("%-3d-> ", j);
        }
        cout << "|||\n";
        ++index;
    }

    cout << "\n=> Max tour " << index_MaxTour + 1 << ": " << distance_MaxTour << "\n";
    cout << endl;
}