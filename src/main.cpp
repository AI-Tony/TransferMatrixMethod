#include <iostream>
#include <fstream>
#include <ostream>
#include <time.h>
#include <filesystem>
#include "Parser.h"
#include "TransferMatrixMethod.h"
#include "interface.h"

#define MAX_DATE 20

string get_date();
void data_to_file(ofstream &, string, vector<double> &, vector<double> &);
void data_to_file(ofstream &, string, vector<double> &, vector<complex<double>> &);
void data_to_file(ofstream &, string, signal &, MTM &);

int main() 
{
    Parser parser; 
    Interface interface(parser);

    interface.start();



    // string DIR_PATH = "data/" + get_date();
    // filesystem::create_directory(DIR_PATH);

    // ofstream metadata;
    // ofstream RTM_complexparts;
    // ofstream RTE_complexparts;
    // ofstream TTM_complexparts;
    // ofstream TTE_complexparts;
    // ofstream RTM_coefficients;
    // ofstream RTE_coefficients;
    // ofstream TTM_coefficients;
    // ofstream TTE_coefficients;

    // for ( auto &mtm : parser.MTMs ) {
        
    //     for ( auto &sig : parser.signals ) {

    //         TransferMatrixMethod tmm(mtm, sig);

    //         data_to_file(metadata, DIR_PATH+"metadata.csv", sig, mtm);
            
    //         if (parser.getModes(RTM)==true) {
    //             tmm.reflection(TM);
    //             data_to_file(RTM_complexparts, DIR_PATH+"RTM_complexparts.csv", sig.omega, tmm.complexParts[RTM]);
    //             data_to_file(RTM_coefficients, DIR_PATH+"RTM_coefficients.csv", sig.omega, tmm.coefficients[RTM]);
    //         }
    //         if (parser.getModes(RTE)==true) {
    //             tmm.reflection(TE);
    //             data_to_file(RTE_complexparts, DIR_PATH+"RTE_complexparts.csv", sig.omega, tmm.complexParts[RTE]);
    //             data_to_file(RTE_coefficients, DIR_PATH+"RTE_coefficients.csv", sig.omega, tmm.coefficients[RTE]);
    //         }
    //         if (parser.getModes(TTM)==true) {
    //             tmm.transmission(TM);
    //             data_to_file(TTM_complexparts, DIR_PATH+"TTM_complexparts.csv", sig.omega, tmm.complexParts[TTM]);
    //             data_to_file(TTM_coefficients, DIR_PATH+"TTM_coefficients.csv", sig.omega, tmm.coefficients[TTM]);
    //         }
    //         if (parser.getModes(TTE)==true) {
    //             tmm.transmission(TE);
    //             data_to_file(TTE_complexparts, DIR_PATH+"TTE_complexparts.csv", sig.omega, tmm.complexParts[TTE]);
    //             data_to_file(TTE_coefficients, DIR_PATH+"TTE_coefficients.csv", sig.omega, tmm.coefficients[TTE]);
    //         }
    //     }
    // }

    // metadata.close();
    // RTM_complexparts.close();
    // RTE_complexparts.close();
    // TTM_complexparts.close();
    // TTE_complexparts.close();
    // RTM_coefficients.close();
    // RTE_coefficients.close();
    // TTM_coefficients.close();
    // TTE_coefficients.close();
}


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

void data_to_file(ofstream &file, string path_to_file, vector<double> &omega, vector<double> &data) {
    if (!file.is_open()) {
        file.precision(32);
        file.open(path_to_file);
        for (int i = 0; i < omega.size(); i++) {
            file << omega[i];
            if ( i < omega.size()-1 ) { file << ','; }
        }
        file << endl;
        file.precision(6);
    }
    for (int i = 0; i < data.size(); i++) {
        file << data[i];
        if ( i < data.size()-1 ) { file << ','; }
    }
    file << endl;
} 

void data_to_file(ofstream &file, string path_to_file, vector<double> &omega, vector<complex<double>> &data) {
    if (!file.is_open()) {
        file.precision(32);
        file.open(path_to_file);
        for (int i = 0; i < omega.size(); i++) {
            file << omega[i];
            if ( i < omega.size()-1 ) { file << ','; }
        }
        file << endl;
        file.precision(6);
    }
    for (int i = 0; i < data.size(); i++) {
        if (data[i].imag() < 0) {
            file << data[i].real() << data[i].imag() << 'j';
        } else {
            file << data[i].real() << '+' << data[i].imag() << 'j';
        }
        if ( i < data.size()-1 ) { file << ','; }
    }
    file << endl;
} 

void data_to_file(ofstream &file, string path_to_file, signal &sig, MTM &mtm) {
    if (!file.is_open()) {
        file.open(path_to_file);
        file << "theta," << "lower," << "upper," << "materials," << "thicknesses" << endl;
    }
    file << sig.theta << ',' << sig.lower << ',' << sig.upper << ',';
    for (auto &mat : mtm.materials) {
        file << ' ' << MTM::convert(mat);
    } 
    file << ',';
    for (auto &t : mtm.thicknesses) {
        file << ' ' << t;
    }
    file << endl;
}