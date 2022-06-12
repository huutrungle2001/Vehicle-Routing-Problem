#include "InstanceTSP.h"
#include<bits/stdc++.h>

using namespace std;

InstanceTSP::InstanceTSP(string instanceName){
    read_input(instanceName);
    initialize();
    cout << "Done loading !" << endl;
}

void InstanceTSP::read_input(const string &FileInput){
    ifstream File(FileInput);
    if (!File.is_open()){
        cout << "Unable to open file \n";
        exit(0);
    }
    string line;
    nodes = 0;
    while (getline(File, line)){
        stringstream ss(line);
        int id;
        double xx, yy;
        ss >> id >> xx >> yy;
        x.push_back(xx);
        y.push_back(yy);
        nodes++;
    }
    File.close();
}

InstanceTSP::InstanceTSP(vector<vector<double> > dist_matrix){
    this->dist_matrix = dist_matrix;
    this->nodes = dist_matrix.size();
}

void InstanceTSP::initialize(){
    dist_matrix.resize(nodes, {});
    for (int i = 0; i < nodes; ++i){
        for (int j = 0; j < nodes; ++j) {
            double d = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
            dist_matrix[i].push_back(d);
        }
    }
}

double InstanceTSP::getDistance(int i, int j){
    return dist_matrix[i][j];
}
