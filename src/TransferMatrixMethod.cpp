#include "TransferMatrixMethod.h"
#include <cstdlib>
#include <iostream>

TransferMatrixMethod::TransferMatrixMethod(MTM mtm, signal sig) : mtm(mtm), sig(sig) 
{
    // materialProperties();
}

void TransferMatrixMethod::calculateRTM() 
{
    for (auto &k : sig.k0) {
        k0 = k;
        kx = k0 * sin(sig.theta); 
        omega = 3e8 * k0;
        complex<double> ni = mtm.refractiveIndex(mtm.materials.front(), omega); 
        complex<double> nt = mtm.refractiveIndex(mtm.materials.back(), omega); 
        complex<double> bi = sqrt(k0*k0*ni*ni - kx*kx); 
        complex<double> bt = sqrt(k0*k0*nt*nt - kx*kx); 
        totalMatrix(TM);
        invertMatrix();
        complex<double> rtm = abs(-( IM[0]*bi*nt*nt - IM[3]*bt*ni*ni + 1i*(IM[2]*ni*ni*nt*nt + IM[1]*bi*bt ))/
                                   ( IM[0]*bi*nt*nt + IM[3]*bt*ni*ni - 1i*(IM[2]*ni*ni*nt*nt - IM[1]*bi*bt )));
        reflectionTM.push_back(rtm);
    }
}

void TransferMatrixMethod::totalMatrix(polarisation pol)
{
    M.clear();
    M.push_back(1.0), M.push_back(0.0), M.push_back(0.0), M.push_back(1.0);
    for (int i = mtm.layers; i > 0; i--) {
        switch (pol) {
            case(TM): submatixTM(i);
            case(TE): submatixTE(i);
        }
        MatMul(M, m);
    }
}

void TransferMatrixMethod::submatixTM(int i)
{
    double d = mtm.thicknesses[i]; 
    material mat = mtm.materials[i];
    complex<double> n = mtm.refractiveIndex(mat, omega); 
    complex<double> beta = sqrt(k0*k0*n*n - kx*kx);
    m.clear();
    m.push_back( cos(beta*d));
    m.push_back( sin(beta*d)*n*n/beta);
    m.push_back(-sin(beta*d)*beta/(n*n));
    m.push_back( cos(beta*d));
    cout << n << " " << beta <<  endl;
}

void TransferMatrixMethod::submatixTE(int l)
{
    // double d = mtm.thicknesses[l]; 
    // complex<double> b = beta[l+1]; 
    // m[0] =  cos(b*d);
    // m[1] =  sin(b*d)/b;
    // m[2] = -sin(b*d)*b;
    // m[3] =  cos(b*d);
}

void TransferMatrixMethod::MatMul(vector<complex<double>> M, vector<complex<double>> m) 
{
    complex<double> M1 = M[0], M2 = M[1], M3 = M[2], M4 = M[3]; 
    complex<double> m1 = m[0], m2 = m[1], m3 = m[2], m4 = m[3]; 
    M.clear(); 
    M.push_back(M1*m1 + M2*m3); 
    M.push_back(M1*m2 + M2*m4); 
    M.push_back(M3*m1 + M4*m3); 
    M.push_back(M3*m2 + M4*m4); 
}

void TransferMatrixMethod::invertMatrix()
{
    complex<double> det = M[0]*M[3] - M[1]*M[2] + 1e-15;
    IM.clear();
    IM.push_back( M[3] / det);
    IM.push_back(-M[1] / det);
    IM.push_back(-M[2] / det);
    IM.push_back( M[0] / det);
}

// void TransferMatrixMethod::materialProperties() {
//     set<material> unique_materials;
//     for (auto &mat : mtm.materials) { unique_materials.insert(mat); }
//     for (auto &mat : unique_materials) {
//         vector<complex<double>> refractiveIndices;
//         vector<complex<double>> propagationConstants;
//         for (int i = 0; i < sig.omega.size(); i++) {
//             complex<double> n = mtm.refractiveIndex(mat, sig.omega[i], sig.theta);
//             complex<double> beta = sqrt(sig.k0[i]*sig.k0[i]*n*n - sig.kx[i]*sig.kx[i]);
//             refractiveIndices.push_back(n);
//             propagationConstants.push_back(beta);
//         }
//         refractive_indicies[mat] = refractiveIndices;
//         propagation_constants[mat] = propagationConstants;
//     }
// }