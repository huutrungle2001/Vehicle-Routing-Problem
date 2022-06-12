#pragma once
#include<bits/stdc++.h>

using namespace std;

class Config {
public:
    string s;
    bool check = false;
    double timelimit = 30.0;
    bool checktime = false;
    int iter = 100;
    bool checkiter = false;

    Config(int argc, char* argv[]){
        for (int i = 1; i < argc; ++i){
            string key = argv[i];

            if (key == "-i"){
                string value = argv[++i];
                s = value;
                check = true;
            }else if (key == "-t"){
                string value = argv[++i];
                timelimit = stof(value);
                checktime = true;
            }else if (key == "-it"){
                string value = argv[++i];
                iter = stoi(value);
                checkiter = true;
            }else{
                cerr << "Invalid argument !!!\n";
                exit(0);
            }
        }

        if (!check){
            cerr << "ERROR: Input is required !!\n";
            exit(0);
        }
        if (!checktime){
            cout << "Warning: Time limit default = 30.0s\n";
        }
        if (!checkiter){
            cout << "Warning: Max iteration default = 50,000 iterations\n";
        }

        cout << flush;
    }
};
