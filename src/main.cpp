#include <iostream>
#include <fstream>
#include "Parser.h"
#include "TransferMatrixMethod.h"

int main() 
{
    Parser parser; 

    // cout << parser.angular_window[0] << endl;
    // cout << parser.angular_window[1] << endl;

    // cout << parser.MTMs.size() << endl;
    // cout << parser.signals.size() << endl;

    for ( auto &mtm : parser.MTMs ) {

        // cout << mtm.layers << endl;
        // for (auto &t : mtm.thicknesses) {
        //     cout << t << " ";
        // }
        // cout << endl;
        // for (auto &m : mtm.materials) {
        //     cout << m << " ";
        // }
        // cout << endl;
        // cout << endl;
        
        for ( auto &sig : parser.signals ) {

            // cout << sig.theta << endl;
            // cout << sig.k0.size() << endl;
            // cout << sig.kx.size() << endl;
            // cout << sig.omega.size() << endl;

            TransferMatrixMethod tmm(mtm, sig);
            
            if (parser.mode[RTM]==true) tmm.calculateRTM();
            // if (mode_map[RTE]==true) calculateReflection(TE);
            // if (mode_map[TTM]==true) calculateTransmission(TM);
            // if (mode_map[TTE]==true) calculateTransmission(TE);

            // for (auto &value : tmm.reflectionTM) {
            //     cout << value << endl;
            // }
        }
    }
}