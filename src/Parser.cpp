#include "Parser.h"
#include "MTM.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

Parser::Parser()
{
    readConfigurationFile();
    readDesignsFiles();
}

void Parser::readConfigurationFile() {
    ifstream file;
    file.open(CONF_FILE);
    while ( getline(file, line) ) {
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="RTM") lineStream >> boolalpha >> modes[RTM]; 
        else if (key=="RTE") lineStream >> boolalpha >> modes[RTE];
        else if (key=="TTM") lineStream >> boolalpha >> modes[TTM];
        else if (key=="TTE") lineStream >> boolalpha >> modes[TTE]; 
        else if (key=="discretisation") { lineStream >> value; discretisation = stoi(value); } 
        else if (key=="spectrum_lower_limit") {lineStream >> value; spectrum_lower_limit = stoi(value); }
        else if (key=="spectrum_upper_limit") {lineStream >> value; spectrum_upper_limit = stoi(value); }
        else if (key=="angle_lower_limit") {lineStream >> value; angle_lower_limit = stoi(value); }
        else if (key=="angle_upper_limit") {lineStream >> value; angle_upper_limit = stoi(value); }
        else if (key=="incident_medium") {lineStream >> value; incident_medium = MTM::convert(value); }
        else if (key=="transmitted_medium") {lineStream >> value; transmited_medium = MTM::convert(value); }
    }
    file.close();
    updateSignals();
}

void Parser::writeConfigurationFile() {
    ifstream read_file;
    ofstream write_file;
    vector<string> lines;
    read_file.open(CONF_FILE);
    while( getline(read_file, line) ) { lines.emplace_back(line); }
    read_file.close();
    write_file.open(CONF_FILE);
    write_file << boolalpha;
    for (auto &line : lines) {
        istringstream lineStream(line);
        lineStream >> key;
        if (key=="RTM") { write_file << key << " " << modes[RTM] << endl; }
        else if (key=="RTE") { write_file << key << " " << modes[RTE] << endl; }
        else if (key=="TTM") { write_file << key << " " << modes[TTM] << endl; }
        else if (key=="TTE") { write_file << key << " " << modes[TTE] << endl; }
        else if (key=="discretisation") { write_file << key << " " << discretisation << endl; }
        else if (key=="spectrum_lower_limit") {write_file << key << " " << spectrum_lower_limit << endl; }
        else if (key=="spectrum_upper_limit") {write_file << key << " " << spectrum_upper_limit << endl; }
        else if (key=="angle_lower_limit") {write_file << key << " " << angle_lower_limit << endl; }
        else if (key=="angle_upper_limit") {write_file << key << " " << angle_upper_limit << endl; }
        else if (key=="incident_medium") {write_file << key << " " << MTM::convert(incident_medium) << endl; }
        else if (key=="transmitted_medium") {write_file << key << " " << MTM::convert(transmited_medium) << endl; }
        else { write_file << line << endl; }
    }
    write_file.close();
}

void Parser::readDesignsFiles() {
    ifstream material_file;
    ifstream thickness_file;
    material_file.open(MATERIAL_FILE);
    thickness_file.open(THICKNESS_FILE);
    while ( getline(material_file, line) ) {
        vector<material> materials;
        istringstream lineStream(line);
        while ( lineStream >> value ) {
            materials.emplace_back( MTM::convert(value) );
        }
        all_materials.emplace_back( materials );
    }
    while ( getline(thickness_file, line) ) {
        vector<double> thicknesses;
        istringstream lineStream(line);
        while ( lineStream >> value ) {
            thicknesses.emplace_back( stod(value) * 1e-9 );
        }
        all_thicknesses.emplace_back( thicknesses );
    }
    material_file.close();
    thickness_file.close();
    updateDesigns();
}

void Parser::writeDesignsFiles() {
    ofstream material_file;
    ofstream thickness_file;
    material_file.open(MATERIAL_FILE);
    thickness_file.open(THICKNESS_FILE);
    for ( auto &mtm : MTMs ) {
        for ( auto &mat : mtm.materials ) { material_file << MTM::convert(mat) << " "; }
        for ( auto &thickness : mtm.thicknesses ) { thickness_file << thickness * 1e9 << " "; }
        material_file << endl;
        thickness_file << endl;
    }
    material_file.close();
    thickness_file.close();
}

void Parser::setModes(mode mode, bool boolean) {
    modes[mode] = boolean;
}

void Parser::setDisctretisation(int num) {
    discretisation = num;
}

void Parser::setLowerSpectralLimit(int num) {
    spectrum_lower_limit = num;
}

void Parser::setUpperSpectralLimit(int num) {
    spectrum_upper_limit = num;
}

void Parser::setLowerAngularLimit(int num) {
    angle_lower_limit = num;
}

void Parser::setUpperAngularLimit(int num) {
    angle_upper_limit = num;
}

void Parser::setIncidentMedium(material mat) {
    incident_medium = mat;
} 

void Parser::setTransmittedMedium(material mat) {
    transmited_medium = mat;
}

void Parser::setDesigns(vector<double> thicknesses, vector<material> materials) {
    all_materials.emplace_back( materials );
    all_thicknesses.emplace_back( thicknesses );
}

void Parser::deleteDesigns(int num) {
    if ( num == 0 ) {
        MTMs.clear();
    } else {
        MTMs.erase( MTMs.begin() + num - 1 );
    }
}

void Parser::updateSignals() {
    signals.clear();
    double step = double( spectrum_upper_limit - spectrum_lower_limit ) / discretisation ;
    for ( int theta = angle_lower_limit; theta < angle_upper_limit; theta++ ) {
        signal sig;
        sig.theta = theta*M_PI/180;
        sig.lower = spectrum_lower_limit;
        sig.upper = spectrum_upper_limit;
        for (double lambda = spectrum_upper_limit; lambda > spectrum_lower_limit; lambda -= step) {
            double k0 = 2.0*M_PI / ( lambda*1e-9 );
            sig.k0.emplace_back( k0 );
            sig.kx.emplace_back( k0*sin(sig.theta) );
            sig.omega.emplace_back( 3e8 * k0 ); 
        }
        signals.emplace_back(sig);
    }
}

void Parser::updateDesigns() {
    MTMs.clear();
    for (int i = 0; i < all_materials.size(); i++) {
        MTM mtm;
        mtm.incident_medium = incident_medium;
        mtm.transmited_medium = transmited_medium;
        mtm.materials = all_materials[i];
        mtm.thicknesses = all_thicknesses[i];
        mtm.layers = mtm.materials.size();
        MTMs.emplace_back( mtm );
    }
}

void Parser::refresh() {
    updateDesigns();
    updateSignals();
    writeDesignsFiles();
    writeConfigurationFile();
}