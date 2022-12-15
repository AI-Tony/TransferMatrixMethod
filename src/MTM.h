#pragma once

#include <iostream> 
#include <complex>
#include <vector>

using namespace std;

// do not change the position of air or LAST!
enum material{air, tio2, sio2, ito, LAST};

class MTM
{
  public:
    int layers=0;
    material incident_medium;
    material transmited_medium;
    vector<double> thicknesses; 
    vector<material> materials; 
    static string convert(material); 
    static material convert(string); 
    complex<double> refractiveIndex(material, double); 

  private:
    complex<double> ITO(double);
};