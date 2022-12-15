#include "MTM.h"

string MTM::convert(material mat) {
    if(mat==air) return "air";
    else if (mat==tio2) return "tio2";
    else if (mat==sio2) return "sio2";
    else if (mat==ito) return "ito";
    else printf("Error: invalid material!"); exit(1);
}

material MTM::convert(string str) {
    if(str=="air") return air;
    else if (str=="tio2") return tio2;
    else if (str=="sio2") return sio2;
    else if (str=="ito") return ito;
    else printf("Error: invalid material!"); exit(1);
}

complex<double> MTM::refractiveIndex(material mat, double omega)
{
    switch (mat) {
        case air: return 1.0; break; 
        case tio2: return 2.45; break; 
        case sio2: return 1.45; break; 
        case ito: return ITO(omega); break; 
        default: printf("Error: invalid material!"); exit(1); break;
    }
}

complex<double> MTM::ITO(double omega) {
    complex<double> gamma = 2.05e14, Einf = 3.91, wp2 = pow(2.65e15, 2), w = omega;
    complex<double> eps = Einf - wp2 / ( w*w + 1i*gamma*w );
    double n = sqrt(  real(eps) + sqrt( pow(real(eps), 2) + pow(imag(eps), 2) ) ) / sqrt(2);
    double k = sqrt( -real(eps) + sqrt( pow(real(eps), 2) + pow(imag(eps), 2) ) ) / sqrt(2);
    return n + 1i*k;
}
