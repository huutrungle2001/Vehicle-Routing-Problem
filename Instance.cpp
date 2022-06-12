#include "Instance.h"
#include<bits/stdc++.h>

using namespace std;

Instance::Instance(string Filename){
    read_input(Filename);
    initialize();
    cout << "Done loading !" << endl;
    cout << "____________________________________________\n";
}

void Instance::read_input(const string &FileInput){
    ifstream File(FileInput);
    if (!File.is_open()){
        cout << "Unable to open file \n";
        exit(0);
    }
    string line;

    for (int i = 0; i < 4; i++){
        getline(File, line);
    }

    stringstream s(line);
    string token;
    s >> token >> token;
    s >> nodes;
    cout << "Number client: " << nodes << endl;

    getline(File, line);
    getline(File, line);

    stringstream s1(line);
    string token1;
    s1 >> token >> token;

    getline(File, line);
    double xx, yy;
    int tmp;

    for (int i = 0; i < nodes; ++i){
        getline(File, line);
        stringstream ss(line);
        ss >> tmp >> xx >> yy;
        x.push_back(xx);
        y.push_back(yy);
    }
    File.close();

    trucks = 10;
    clients = ceil((nodes - 1) / trucks) + 2;
    cout << "Number truck: " << trucks << endl;
    cout << "Max client: " << clients << endl;
}

void Instance::initialize(){
    dist_matrix.resize(nodes, {});

    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            double d = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
            dist_matrix[i].push_back(d);
        }
    }
}

void Instance::show_data(){
    cout << "________________________________\n";
    cout << "Data from file:\n";
    printf("%4s:%5s  %10s\n", "STT", "X", "Y");
    for (int i = 0; i < x.size(); ++i) {
        printf("%4d:%10.2f  %10.2f\n", i, x[i], y[i]);
    }
    cout << "________________________________\n";
}

double Instance::getDistance(int i, int j){
    return dist_matrix[i][j];
}

pair<double, double> Instance::getPoint(int id){
    double x_, y_;
    try{
        x_ = x[id];
        y_ = y[id];
    } catch (const exception &e){
        cerr << "Error function Instance::getPoint(int id): " << e.what() << '\n';
    }
    return {x_, y_};
}