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
    vector<double> reflectionCoefficientsTM;
    vector<double> reflectionCoefficientsTE;
    vector<double> transmissionCoefficientsTM;
    vector<double> transmissionCoefficientsTE;
    vector<complex<double>> reflectionTM;
    vector<complex<double>> reflectionTE;
    vector<complex<double>> transmissionTM;
    vector<complex<double>> transmissionTE;

  private:
    MTM mtm;
    signal sig;
    double k0, kx, omega, theta;
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
