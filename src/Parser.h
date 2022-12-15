#pragma once

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <map>
#include "Signal.h"
#include "MTM.h"

using namespace std;

class Parser
{
    public:
        Parser();
        void refresh();
        void setModes(mode, bool);
        void setDisctretisation(int);
        void setLowerAngularLimit(int);
        void setUpperAngularLimit(int);
        void setLowerSpectralLimit(int);
        void setUpperSpectralLimit(int);
        void setIncidentMedium(material);
        void setTransmittedMedium(material);
        void setDesigns(vector<double>, vector<material>);
        void deleteDesigns(int);

        vector<MTM> getMTMs() { return MTMs; }
        vector<signal> getSignals() { return signals; }
        bool getModes(mode mode) { return modes[mode]; }
        int getDisctretisation() { return discretisation; }
        int getLowerAngularLimit() { return angle_lower_limit; }
        string getUpperAngularLimit() { 
            if (angle_upper_limit<=angle_lower_limit) { return "NULL"; }
            return to_string(angle_upper_limit); 
        }
        int getLowerSpectralLimit() { return spectrum_lower_limit; }
        int getUpperSpectralLimit() { return spectrum_upper_limit; }
        material getIncidentMedium() { return incident_medium; }
        material getTransmittedMedium() { return transmited_medium; }
  
    private:
        string key;
        string line;
        string value;
        string CONF_FILE = "tmm.conf";
        string MATERIAL_FILE = "materials";
        string THICKNESS_FILE = "thicknesses";
        vector<MTM> MTMs;
        vector<signal> signals;
        map<mode, bool> modes;
        vector<vector<double>> all_thicknesses; 
        vector<vector<material>> all_materials;
        material incident_medium;
        material transmited_medium;
        int discretisation;
        int spectrum_lower_limit;
        int spectrum_upper_limit;
        int angle_lower_limit;
        int angle_upper_limit;
        void writeConfigurationFile();
        void writeDesignsFiles();
        void readConfigurationFile();
        void readDesignsFiles();
        void updateSignals();
        void updateDesigns();


        // void setSignals();
        // void setMTMs();
        // vector<int> layers;
        // unique_ptr<vector<vector<double>>> all_thicknesses{new vector<vector<double>>};
        // vector<int> angular_window;
        // vector<int> spectral_window;
        // vector<double> thicknesses;
        // vector<material> materials;
        // vector<vector<double>> _thicknesses_all;
        // vector<vector<material>> _materials_all;
        // void parseMode();
        // void parseDiscretisation();
        // void parseSpectralWindow();
        // void parseAngularWindow();
        // void parseMaterials();
        // void parseThicknesses();
};