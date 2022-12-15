# pragma once

#include <iostream> 
#include <vector>

using namespace std;

enum polarisation{TM, TE};
enum mode{RTM, RTE, TTM, TTE}; 

struct signal
{
    double theta;
    int lower, upper;
    vector<double> k0;
    vector<double> kx;
    vector<double> omega;
};