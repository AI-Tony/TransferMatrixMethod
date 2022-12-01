#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "Signal.h"
#include "MTM.h"

using namespace std;

class Parser
{
  public:
    Parser();
    map<modes, bool> mode;
    vector<signal> signals;
    vector<MTM> MTMs;
  
  private:
    ifstream file;
    bool found;
    string line;
    string key;
    string value;
    string filename = "tmm.conf";

    int discretisation;
    vector<int> angular_window;
    vector<int> spectral_window;

    vector<material> materials;
    vector<int> layers;
    vector<double> thicknesses;
    vector<vector<double>> thicknesses_all;

    void parseMode();
    void parseDiscretisation();
    void parseSpectralWindow();
    void parseAngularWindow();
    void parseMaterials();
    void parseThicknesses();
    void setSignals();
    void setMTMs();
};