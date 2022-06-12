#include "SolutionTSP.h"
#include "InstanceTSP.h"
#include<bits/stdc++.h>


SolutionTSP::SolutionTSP(){
    distance = 0;
    tour.reserve(200);
    tour = {0, 0};
}

SolutionTSP::SolutionTSP(const SolutionTSP& s){
    distance = s.distance;
    tour = s.tour;
}

SolutionTSP &SolutionTSP::operator=(const SolutionTSP& s){
    tour.clear();
    distance = s.distance;
    tour = s.tour;
    return *this;
}

double SolutionTSP::getDistance(){
    return distance;
}

void SolutionTSP::print(){
    for (int i : tour)
        cout << i << " ";
    cout << endl;
}
