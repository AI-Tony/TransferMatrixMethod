# pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>
#include <vector>
#include "Parser.h"
#include "Signal.h"
#include "MTM.h"

#define WIDTH 7
#define DURATION 2

using namespace std;

class Interface 
{
    public:
        Interface(Parser &parser) : parser(parser) {}
        void start();

    private:
        int idx;
        Parser &parser;
        string index();
        string prompt();
        bool booleanInput(bool);
        int numericalInput(int, int);
        void header();
        void viewDesigns();
        void printMaterialOptions();
        void printDesign(vector<double>, vector<material>);

        void mainMenu();
        void setModes(mode);
        void setDiscretisation();
        void setLowerSpectralLimit();
        void setUpperSpectralLimit();
        void setLowerAngularLimit();
        void setUpperAngularLimit();
        void setIncidentMedium();
        void setTransmittedMedium();
        void setDesigns();
        void manualDesign();
        // void target();
        // void random();
        // void unitCell();
        // void parameterised();
};