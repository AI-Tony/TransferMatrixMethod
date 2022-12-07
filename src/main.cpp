#include <iostream>
#include <fstream>
#include <time.h>
#include <filesystem>
#include "Parser.h"
#include "TransferMatrixMethod.h"

#define MAX_DATE 20

string get_date()
{
   time_t now;
   char the_date[MAX_DATE];
   the_date[0] = '\0';
   now = time(NULL);
   if (now != -1){
      strftime(the_date, MAX_DATE, "%Y-%m-%d_%H-%M-%S", gmtime(&now));
   }
   return string(the_date) + "/";
}

void output_data(ofstream &file, vector<double> &data) {
    for (auto &datum : data) {
        file << datum << ',';
    }
    file << endl;
} 

void output_data(ofstream &file, vector<complex<double>> &data) {
    for (auto &datum : data) {
        file << datum << ',';
    }
    file << endl;
} 

void output_data(ofstream &file, signal &sig, MTM &mtm) {
    file << "theta," << "lower," << "upper," << "materials," << "thicknesses" << endl;
    file << sig.theta << ',' << sig.lower << ',' << sig.upper << ',';
    for (auto &m : mtm.materials) {
        file << ' ' << MTM::convert(m);
    } 
    file << ',';
    for (auto &t : mtm.thicknesses) {
        file << ' ' << t;
    }
}

int main() 
{
    Parser parser; 

    string DIR_NAME = get_date();
    string DIR_PATH = "data/";
    filesystem::create_directory(DIR_PATH + DIR_NAME);

    ofstream metadata;
    ofstream RTM_complexparts;
    ofstream RTE_complexparts;
    ofstream TTM_complexparts;
    ofstream TTE_complexparts;
    ofstream RTM_coefficients;
    ofstream RTE_coefficients;
    ofstream TTM_coefficients;
    ofstream TTE_coefficients;

    metadata.open(DIR_PATH + DIR_NAME + "metadata.csv");
    RTM_complexparts.open(DIR_PATH + DIR_NAME + "RTM_complexparts.csv");
    RTE_complexparts.open(DIR_PATH + DIR_NAME + "RTE_complexparts.csv");
    TTM_complexparts.open(DIR_PATH + DIR_NAME + "TTM_complexparts.csv");
    TTE_complexparts.open(DIR_PATH + DIR_NAME + "TTE_complexparts.csv");
    RTM_coefficients.open(DIR_PATH + DIR_NAME + "RTM_coefficients.csv");
    RTE_coefficients.open(DIR_PATH + DIR_NAME + "RTE_coefficients.csv");
    TTM_coefficients.open(DIR_PATH + DIR_NAME + "TTM_coefficients.csv");
    TTE_coefficients.open(DIR_PATH + DIR_NAME + "TTE_coefficients.csv");

    for ( auto &mtm : parser.MTMs ) {
        
        for ( auto &sig : parser.signals ) {

            TransferMatrixMethod tmm(mtm, sig);
            
            if (parser.mode[RTM]==true) tmm.reflection(TM);
            if (parser.mode[RTE]==true) tmm.reflection(TE);
            if (parser.mode[TTM]==true) tmm.transmission(TM);
            if (parser.mode[TTE]==true) tmm.transmission(TE);

            output_data(metadata, sig, mtm);
            output_data(RTM_complexparts, tmm.complexParts[RTM]);
            output_data(RTE_complexparts, tmm.complexParts[RTE]);
            output_data(TTM_complexparts, tmm.complexParts[TTM]);
            output_data(TTE_complexparts, tmm.complexParts[TTE]);
            output_data(RTM_coefficients, tmm.coefficients[RTM]);
            output_data(RTE_coefficients, tmm.coefficients[RTE]);
            output_data(TTM_coefficients, tmm.coefficients[TTM]);
            output_data(TTE_coefficients, tmm.coefficients[TTE]);
        }
    }

    metadata.close();
    RTM_complexparts.close();
    RTE_complexparts.close();
    TTM_complexparts.close();
    TTE_complexparts.close();
    RTM_coefficients.close();
    RTE_coefficients.close();
    TTM_coefficients.close();
    TTE_coefficients.close();
}