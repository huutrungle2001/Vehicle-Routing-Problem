#include "Config.h"
#include "Instance.h"
#include "Solver.h"
#include<bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[]){
    try{
        Config config(argc, argv);
        Instance instance(config.s);
        Solver solver(&instance, config.timelimit, config.iter);
        solver.solve(config.iter);

    }catch (const string& e){
        std::cerr << "EXCEPTION | " << e << "\n";
    }

    return 0;
}