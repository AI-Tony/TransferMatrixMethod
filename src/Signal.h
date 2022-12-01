# pragma once

#include <iostream> 
#include <vector>

using namespace std;

enum polarisation{TM, TE};
enum modes {RTM, RTE, TTM, TTE};

struct signal
{
    double theta;
    vector<double> k0;
    vector<double> kx;
    vector<double> omega;
};