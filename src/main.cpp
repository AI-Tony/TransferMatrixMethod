#include <iostream>
#include <fstream>
#include "Parser.h"
#include "TransferMatrixMethod.h"

int main() 
{
    Parser parser; 

    for ( auto &mtm : parser.MTMs ) {
        
        for ( auto &sig : parser.signals ) {

            TransferMatrixMethod tmm(mtm, sig);
            
            if (parser.mode[RTM]==true) tmm.reflection(TM);
            if (parser.mode[RTE]==true) tmm.reflection(TE);
            if (parser.mode[TTM]==true) tmm.transmission(TM);
            if (parser.mode[TTE]==true) tmm.transmission(TE);

            // for (auto &value : tmm.reflectionTM) {
            //     cout << value << endl;
            // }
        }
    }
}