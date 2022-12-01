#pragma once

#include <iostream> 
#include <complex>
#include <vector>

using namespace std;

enum material{air, tio2, sio2, ito};

class MTM
{
  public:
    int layers;
    vector<double> thicknesses; 
    vector<material> materials;
    static material convert(string &);
    complex<double> refractiveIndex(material, double);

  private:
    complex<double> ITO(double);
};