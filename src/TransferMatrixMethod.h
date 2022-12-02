# pragma once

#include <iostream> 
#include <complex>
#include <vector>
#include <set>
#include <map>
#include "MTM.h"
#include "Signal.h"

using namespace std;

class TransferMatrixMethod 
{
  public: 
    TransferMatrixMethod(MTM, signal);
    void calculateRTM();
    void calculateRTE();
    void calculateTTM();
    void calculateTTE();
    vector<complex<double>> reflectionTM;
    vector<complex<double>> reflectionTE;
    vector<complex<double>> transmissionTM;
    vector<complex<double>> transmissionTE;

  private:
    MTM mtm;
    signal sig;
    double k0, kx, omega;
    // map<material, vector<complex<double>>> refractive_indicies; 
    // map<material, vector<complex<double>>> propagation_constants; 
    vector<complex<double>> IM, M, m;
    void materialProperties();
    void totalMatrix(polarisation);
    void submatixTM(int);
    void submatixTE(int);
    void invertMatrix();
    void MatMul();
};
