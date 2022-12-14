#include "TransferMatrixMethod.h"
#include <iostream>


void TransferMatrixMethod::reflection(polarisation pol) 
{
    setProperties(pol);
    for (int i = 0; i < sig.omega.size(); i++) {
        ni = incidentRefractiveIndices[i];
        nt = transmittedRefractiveIndices[i];
        bi = incidentPropagationConstants[i];
        bt = transmittedPropagationConstants[i];
        switch (pol) {
            case (TM):
                IM = inverseMatricesTM[i];
                rtm =  -( IM[0]*bi*nt*nt - IM[3]*bt*ni*ni + 1i*(IM[2]*ni*ni*nt*nt + IM[1]*bi*bt )) /
                        ( IM[0]*bi*nt*nt + IM[3]*bt*ni*ni - 1i*(IM[2]*ni*ni*nt*nt - IM[1]*bi*bt ));
                complexParts[RTM].emplace_back( rtm );
                coefficients[RTM].emplace_back( abs(rtm)*abs(rtm) );
                break;

            case (TE):
                IM = inverseMatricesTE[i];
                rte =  -( IM[0]*bi - IM[3]*bt + 1i*(IM[2] + IM[1]*bi*bt )) /
                        ( IM[0]*bi + IM[3]*bt - 1i*(IM[2] - IM[1]*bi*bt ));
                complexParts[RTE].emplace_back( rte );
                coefficients[RTE].emplace_back( abs(rte)*abs(rte) );
                break;
        }
    }
}

void TransferMatrixMethod::transmission(polarisation pol) 
{
    setProperties(pol);
    for (int i = 0; i < sig.omega.size(); i++) {
        ni = incidentRefractiveIndices[i];
        nt = transmittedRefractiveIndices[i];
        bi = incidentPropagationConstants[i];
        bt = transmittedPropagationConstants[i];
        switch (pol) {
            case (TM):
                IM = inverseMatricesTM[i];
                ttm = -2.0*ni*nt*bi / ( IM[0]*bi*nt*nt + IM[3]*bt*ni*ni - 1i*(IM[2]*ni*ni*nt*nt - IM[1]*bi*bt ));
                complexParts[TTM].emplace_back( ttm );
                coefficients[TTM].emplace_back( abs(ttm)*abs(ttm)*(bt/bi).real() );
                break;

            case (TE):
                IM = inverseMatricesTE[i];
                tte = -2.0*bi / ( IM[0]*bi + IM[3]*bt - 1i*(IM[2] - IM[1]*bi*bt ));
                complexParts[TTE].emplace_back( tte );
                coefficients[TTE].emplace_back( abs(tte)*abs(tte)*(bt/bi).real() );
                break;
        }
    }
}

void TransferMatrixMethod::setProperties(polarisation pol) {
    if (inverseMatricesTM.empty() || inverseMatricesTE.empty() || incidentRefractiveIndices.empty() || incidentPropagationConstants.empty() || transmittedRefractiveIndices.empty() || transmittedPropagationConstants.empty()) {
        for (int i = 0; i < sig.omega.size(); i++) {
            omega = sig.omega[i]; 
            k0 = sig.k0[i]; 
            kx = sig.kx[i]; 
            if (incidentRefractiveIndices.size() < sig.omega.size()) {
                ni = mtm.refractiveIndex(mtm.incident_medium, omega); 
                bi = sqrt(k0*k0*ni*ni - kx*kx); 
                incidentRefractiveIndices.emplace_back(ni);
                incidentPropagationConstants.emplace_back(bi);
            }
            if (transmittedRefractiveIndices.size() < sig.omega.size()) {
                nt = mtm.refractiveIndex(mtm.transmited_medium, omega); 
                bt = sqrt(k0*k0*nt*nt - kx*kx); 
                transmittedRefractiveIndices.emplace_back(nt);
                transmittedPropagationConstants.emplace_back(bt);
            }
            switch (pol) {
                case (TM):
                    if (inverseMatricesTM.size() < sig.omega.size()) {
                        totalMatrix(TM); 
                        invertMatrix();
                        inverseMatricesTM.emplace_back(IM);
                    }
                    break;
                    
                case (TE):
                    if (inverseMatricesTE.size() < sig.omega.size()) {
                        totalMatrix(TE); 
                        invertMatrix();
                        inverseMatricesTE.emplace_back(IM);
                    }
                    break;
            }
        }
    }
}

void TransferMatrixMethod::totalMatrix(polarisation pol) 
{
    M.clear();
    M.emplace_back(1.0), M.emplace_back(0.0), M.emplace_back(0.0), M.emplace_back(1.0); 
    for (int layer = mtm.layers-1; layer >= 0; layer--) { 
        subMatrix(pol, layer);
        MatMul();
    }
}

void TransferMatrixMethod::subMatrix(polarisation pol, int layer) 
{
    m.clear();
    double d = mtm.thicknesses[layer]; 
    material mat = mtm.materials[layer+1]; 
    n = mtm.refractiveIndex(mat, omega); 
    beta = sqrt(k0*k0*n*n - kx*kx);
    switch (pol) {
        case (TM): 
            m.emplace_back( cos(beta*d));
            m.emplace_back( sin(beta*d)*n*n/beta);
            m.emplace_back(-sin(beta*d)*beta/(n*n));
            m.emplace_back( cos(beta*d));
            break;

        case (TE): 
            m.emplace_back( cos(beta*d));
            m.emplace_back( sin(beta*d)/beta);
            m.emplace_back(-sin(beta*d)*beta);
            m.emplace_back( cos(beta*d));
            break;
    }
}

void TransferMatrixMethod::MatMul() 
{
    complex<double> M1 = M[0], M2 = M[1], M3 = M[2], M4 = M[3]; 
    complex<double> m1 = m[0], m2 = m[1], m3 = m[2], m4 = m[3]; 
    M.clear(); 
    M.emplace_back(M1*m1 + M2*m3); 
    M.emplace_back(M1*m2 + M2*m4); 
    M.emplace_back(M3*m1 + M4*m3); 
    M.emplace_back(M3*m2 + M4*m4); 
}

void TransferMatrixMethod::invertMatrix()
{
    complex<double> det = M[0]*M[3] - M[1]*M[2] + 1e-99;
    IM.clear();
    IM.emplace_back( M[3] / det);
    IM.emplace_back(-M[1] / det);
    IM.emplace_back(-M[2] / det);
    IM.emplace_back( M[0] / det);
}
