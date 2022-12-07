#include "Parser.h"
#include <cmath>

Parser::Parser()
{
    try {
        parseMode();
        parseDiscretisation();
        parseMaterials();
        parseThicknesses();
        parseSpectralWindow();
        parseAngularWindow();
        setSignals();
        setMTMs();
    } catch (char const* msg) {
        cerr << msg << endl;
    }
}

void Parser::parseMode() {
    file.open(filename);
    while (getline(file, line)) {
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="RTM") lineStream >> mode[RTM]; 
        else if (key=="RTE") lineStream >> mode[RTE];
        else if (key=="TTM") lineStream >> mode[TTM];
        else if (key=="TTE") lineStream >> mode[TTE]; 
    }
    file.close();
}

void Parser::parseDiscretisation()
{
    found = false;
    file.open(filename);
    while(getline(file, line))
    {
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="discretisation") 
        {
            lineStream >> value;
            discretisation = stoi(value);
            found = true;
            break;
        }
    }
    file.close();
    if (found==false){ throw "Missing required data from 'tmm.conf'!\n"; }
}

void Parser::parseSpectralWindow()
{
    found = false;
    file.open(filename);
    while(getline(file, line))
    {
        string value1;
        string value2;
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="spectral_window") 
        {
            for ( int i = 0; i < 2; i++ ) {
                lineStream >> value1;
                lineStream >> value2;
                if ( stoi(value1) > stoi(value2) ) {
                    spectral_window.push_back( stoi(value1) );
                    spectral_window.push_back( stoi(value2) );
                } else {
                    spectral_window.push_back( stoi(value2) );
                    spectral_window.push_back( stoi(value1) );
                }
            }
            found = true;
            break;
        }
    }
    file.close();
    if (found==false){ throw "Missing required data from 'tmm.conf'!\n"; }
}

void Parser::parseAngularWindow() {
    found = false;
    file.open(filename);
    while(getline(file, line))
    {
        string value1;
        string value2;
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="angular_window") 
        {
            for ( int i = 0; i < 2; i++ ) {
                lineStream >> value1;
                lineStream >> value2;
                if ( stoi(value1) < stoi(value2) ) {
                    angular_window.push_back( stoi(value1) );
                    angular_window.push_back( stoi(value2) );
                } else {
                    angular_window.push_back( stoi(value2) );
                    angular_window.push_back( stoi(value1) );
                }
            }
            found = true;
            break;
        }
    }
    file.close();
    if (found==false){ throw "Missing required data from 'tmm.conf'!\n"; }
}

void Parser::parseMaterials()
{
    found = false;
    file.open(filename);
    while(getline(file, line))
    {
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="materials") 
        {
            while ( lineStream >> value ) {
                materials.push_back( MTM::convert(value) );
            }
            found = true;
            break;
        }
    }
    file.close();
    if (found==false){ throw "Missing required data from 'tmm.conf'!\n"; }
}

void Parser::parseThicknesses()
{
    file.open("thicknesses");
    while(getline(file, line))
    {
        istringstream lineStream(line);
        while ( lineStream >> value ) {
            thicknesses.push_back( stod(value) * 1e-9 );
        }
        layers.push_back(thicknesses.size());
        thicknesses_all.push_back(thicknesses);
        thicknesses.clear();
    }
    file.close();
}

void Parser::setSignals() {
    double step = double( spectral_window[0] - spectral_window[1] ) / discretisation ;
    for ( int theta = angular_window[0]; theta < angular_window[1]; theta++ ) {
        signal sig;
        sig.theta = theta*M_PI/180;
        sig.lower = spectral_window[1];
        sig.upper = spectral_window[0];
        for (double lambda = spectral_window[0]; lambda > spectral_window[1]; lambda -= step) {
            double k0 = 2.0*M_PI / ( lambda*1e-9 );
            sig.k0.push_back( k0 );
            sig.kx.push_back( k0*sin(theta) );
            sig.omega.push_back( 3e8 * k0 ); 
        }
        signals.push_back(sig);
    }
}

void Parser::setMTMs()
{
    for ( int i = 0; i < thicknesses_all.size(); i++ ) { 
        MTM mtm;
        mtm.layers = layers[i];
        mtm.materials = materials;
        mtm.thicknesses = thicknesses_all[i];
        MTMs.push_back(mtm);
    }
}
