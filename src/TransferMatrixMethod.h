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
    TransferMatrixMethod(MTM mtm, signal sig) : mtm(mtm), sig(sig) {}
    void reflection(polarisation);
    void transmission(polarisation);
    map<mode, vector<double>> coefficients;
    map<mode, vector<complex<double>>> complexParts; 

  private:
    MTM mtm;
    signal sig;
    double k0, kx, omega;
    vector<complex<double>> IM, M, m;
    complex<double> rtm, rte, ttm, tte;
    complex<double> ni, nt, bi, bt, n, beta;
    vector<vector<complex<double>>> inverseMatricesTM, inverseMatricesTE;
    vector<complex<double>> incidentRefractiveIndices;
    vector<complex<double>> incidentPropagationConstants;
    vector<complex<double>> transmittedRefractiveIndices;
    vector<complex<double>> transmittedPropagationConstants;
    void setProperties(polarisation);
    void totalMatrix(polarisation);
    void subMatrix(polarisation, int);
    void invertMatrix();
    void MatMul();
};
