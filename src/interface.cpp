#include "interface.h"
#include "MTM.h"
#include "Parser.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <unistd.h>


void Interface::start() {
    mainMenu();
    cout << prompt() << "Edit setup before continuing? (yes/no) ";
    if ( booleanInput(true) ) { 
        while (true) {

            cout << prompt() << "Select an property to edit: ";
            switch ( numericalInput(1, idx) ) {
                case (1): setModes(RTM); break;
                case (2): setModes(RTE); break;
                case (3): setModes(TTM); break;
                case (4): setModes(TTE); break;
                case (5): setDiscretisation(); break;
                case (6): setLowerAngularLimit(); break;
                case (7): setUpperAngularLimit(); break;
                case (8): setLowerSpectralLimit(); break;
                case (9): setUpperSpectralLimit(); break;
                case (10): setIncidentMedium(); break;
                case (11): setTransmittedMedium(); break;
                case (12): setDesigns(); break;
                case (13): break;
            }
            
            mainMenu();
            cout << prompt() << "Finished editing? (yes/no) ";
            if ( booleanInput(true) ) { 
                parser.refresh(); 
                cout << prompt() << "Writing changes to disk" << endl;
                sleep(DURATION) ;
                break;
            }
        }
    } 
    cout << endl;
}

void Interface::mainMenu() {
    system("clear");
    header();
    idx = 0;
    cout << boolalpha;
    cout << "  Properties menu" << endl;
    cout << "  ---------------" << endl;
    cout << "  Index   Description             Value   Unit" << endl;
    cout << " ----------------------------------------------" << endl;
    cout << index() << "Reflection (TM):      " << setw(WIDTH) << parser.getModes(RTM) << endl;
    cout << index() << "Reflection (TE):      " << setw(WIDTH) << parser.getModes(RTE) << endl;
    cout << index() << "Transmission (TM):    " << setw(WIDTH) << parser.getModes(TTM) << endl;
    cout << index() << "Transmission (TE):    " << setw(WIDTH) << parser.getModes(TTE) << endl;
    cout << index() << "Discretisation:       " << setw(WIDTH) << parser.getDisctretisation()    << endl;
    cout << index() << "Lower angular limit:  " << setw(WIDTH) << parser.getLowerAngularLimit()  << setw(6) << " deg" << endl;
    cout << index() << "Upper angular limit:  " << setw(WIDTH) << parser.getUpperAngularLimit()  << setw(6) << " deg" << endl;
    cout << index() << "Lower spectral limit: " << setw(WIDTH) << parser.getLowerSpectralLimit() << setw(6) << " nm" << endl;
    cout << index() << "Upper spectral limit: " << setw(WIDTH) << parser.getUpperSpectralLimit() << setw(6) << " nm" << endl;
    cout << index() << "Incident medium:      " << setw(WIDTH) << MTM::convert(parser.getIncidentMedium()) << endl;
    cout << index() << "Transmitted medium:   " << setw(WIDTH) << MTM::convert(parser.getTransmittedMedium()) << endl;
    cout << index() << "Number of designs:    " << setw(WIDTH) << parser.getMTMs().size() << endl;
    cout << endl;
}

void Interface::setDesigns() {
    while (true) {
        int num;
        idx = 0;
        system("clear");
        header();
        cout << "  Metamaterial design settings" << endl; 
        cout << "  ----------------------------" << endl;
        cout << index() << "Return to main menu" << endl;
        cout << index() << "View current designs " << endl;
        cout << index() << "Delete all designs" << endl;
        cout << index() << "Delete specific design" << endl;
        cout << index() << "Add new design manually" << endl;
        cout << endl;
        cout << prompt() << "Select an option: ";
        switch ( numericalInput(0, idx) ) {
            case (1): return;
            case (2): viewDesigns(); break;
            case (3): parser.deleteDesigns(0); break;
            case (4): cout << prompt() << "Select a design to delete: ";
                      num = numericalInput(1, idx);
                      parser.deleteDesigns( num ); 
                      cout << prompt() << "Design " << num << " deleted" << endl;
                      sleep(DURATION); break;
            case (5): manualDesign(); break;
        }
    }
}

void Interface::viewDesigns() {
    string input;
    system("clear");
    header();
    idx = 0;
    cout << "  There are a total of " << parser.getMTMs().size() << " designs" << endl << endl;
    for ( auto &mtm : parser.getMTMs() ) {
        printDesign(mtm.thicknesses, mtm.materials);
    }
    cout << prompt() << "Press 'q' and enter to go back: ";
    while (true) {
        cin >> input;
        if (input == "q") { break; }
    }
}

void Interface::manualDesign() {
    vector<double> thicknesses;
    vector<material> materials;
    system("clear");
    header();
    cout << prompt() << "Enter desired number of metamaterial layers: ";
    int layers = numericalInput(1, numeric_limits<int>::max());
    cout << layers << endl;
    for (int i = 0; i < layers; i++) {
        system("clear");
        header();
        idx = 0;
        if (materials.size() > 0) { 
            printDesign(thicknesses, materials); 
        }
        printMaterialOptions();
        cout << prompt() << "Enter material for layer " << i+1 << ": "; 
        materials.emplace_back( static_cast<material>(numericalInput(1, idx)-1) );
        cout << prompt() << "Enter thickness for layer " << i+1 << " (nm): ";
        thicknesses.emplace_back( numericalInput(1, numeric_limits<int>::max()) * 1e-9 );
    }
    parser.setDesigns(thicknesses, materials);
    parser.refresh();
    system("clear");
    header();
    idx = 0;
    printDesign(thicknesses, materials);
    cout << prompt() << "New design added" << endl;
    sleep(DURATION);
}

void Interface::setModes(mode mode) {
    string mode_string;
    if (mode==RTM) { mode_string = "RTM"; }
    else if (mode==RTE) { mode_string = "RTE"; }
    else if (mode==TTM) { mode_string = "TTM"; }
    else if (mode==TTE) { mode_string = "TTE"; }
    cout << prompt() << "Would you like to calculate " << mode_string << "? (yes/no) ";
    if ( booleanInput(true) ) { 
        parser.setModes(mode, 1); 
        cout << prompt() << mode_string << " set to true " << endl;
        sleep(DURATION); 
    } else { 
        parser.setModes(mode, 0); 
        cout << prompt() << mode_string << " set to false " << endl;
        sleep(DURATION); 
    } 
}

void Interface::setDiscretisation() {
    cout << prompt() << "The discretisation is currently set to " << parser.getDisctretisation() << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setDisctretisation( numericalInput(1, numeric_limits<int>::max()) );
    cout << prompt() << "Discretisation set to " << parser.getDisctretisation() << endl;
    sleep(DURATION);
}

void Interface::setLowerAngularLimit() {
    cout << prompt() << "The starting angle is currently set to " << parser.getLowerAngularLimit() << " deg" << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setLowerAngularLimit( numericalInput(0, 90) ); 
    cout << prompt() << "Starting angle set to " << parser.getLowerAngularLimit() << endl;
    sleep(DURATION);
}

void Interface::setUpperAngularLimit() {
    cout << prompt() << "The upper anglular limit is currently set to " << parser.getUpperAngularLimit() << " deg" << endl;
    cout << prompt() << "To use a single incident angle, set upper bound " << endl;
    cout << prompt() << "lower than or equal to the lower bound" << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setUpperAngularLimit( numericalInput(0, 90) ); 
    cout << prompt() << "Angle upper limit set to " << parser.getUpperAngularLimit() << endl;
    sleep(DURATION);
}

void Interface::setLowerSpectralLimit() {
    cout << prompt() << "The lower spectral limit is currently set to " << parser.getLowerSpectralLimit() << " nm" << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setLowerSpectralLimit( numericalInput(1, parser.getUpperSpectralLimit()) ); 
    cout << prompt() << "Lower spectral limit set to " << parser.getLowerSpectralLimit() << endl;
    sleep(DURATION);
}

void Interface::setUpperSpectralLimit() {
    cout << prompt() << "The Upper spectral limit is currently set to " << parser.getUpperSpectralLimit() << " nm" << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setUpperSpectralLimit( numericalInput(parser.getLowerSpectralLimit(), numeric_limits<int>::max()) ); 
    cout << prompt() << "Upper spectral limit set to " << parser.getUpperSpectralLimit() << endl;
    sleep(DURATION);
}

void Interface::setIncidentMedium() {
    system("clear");
    header();
    printMaterialOptions();
    cout << prompt() << "The incident medium is currently set to " << MTM::convert( parser.getIncidentMedium() ) << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setIncidentMedium( static_cast<material>(numericalInput(1, idx)-1) );
    cout << prompt() << "Incident medium set to " << MTM::convert( parser.getIncidentMedium() ) << endl;
    sleep(DURATION);
}

void Interface::setTransmittedMedium() {
    system("clear");
    header();
    printMaterialOptions();
    cout << prompt() << "The transmitted medium is currently set to " << MTM::convert( parser.getTransmittedMedium() ) << endl;
    cout << prompt() << "What would you like to set the new value to? ";
    parser.setTransmittedMedium( static_cast<material>(numericalInput(1, idx)-1) );
    cout << prompt() << "Transmitted medium set to " << MTM::convert( parser.getTransmittedMedium() ) << endl;
    sleep(DURATION);
}

string Interface::index() {
    idx++;
    string str = "   [" + to_string(idx) + "]";
    while ( str.size() < 10) { 
        str += " "; 
    }
    return str;
}

string Interface::prompt() {
    return "> ";
}

int Interface::numericalInput(int lower, int upper) {
    int input;
    cin >> input;
    while (true) {
        if (cin.fail()) {
            cout << prompt() << "Invalid input, not an integer!" << endl;
            cout << prompt() << "Try again: ";
            cin >> input;
        } else if (input < lower || input > upper) {
            cout << prompt() << "Invalid input, out of bounds!" << endl;
            cout << prompt() << "Try again: ";
            cin >> input;
        } else {
            return input;
        }
    }
}

bool Interface::booleanInput(bool Default) {
    string input;
    cin >> input;
    while (true) {
        if (input=="yes" || input=="y") { return true; }
        else if (input=="no" || input=="n") { return false; }
        else { 
            cout << prompt() << "Invalid input, enter 'yes' or 'no'." << endl; 
            cout << prompt() << "Try again: ";
            cin >> input;
        }
    }
}

void Interface::printDesign(vector<double> thicknesses, vector<material> materials) {
    cout << index();
    cout << "Materials:   ";
    for (auto &mat : materials) { cout << setw(7) << MTM::convert( mat ); }
    cout << "\n          Thicknesses: ";
    for (auto &thickness : thicknesses) { cout << setw(7) << thickness * 1e9; }
    cout << endl << endl;
}

void Interface::printMaterialOptions() {
    idx = 0;
    cout << "  Material options" << endl;
    cout << "  ----------------" << endl;
    for (int i = air; i != LAST; i++ ) {
        // material mat = static_cast<material>(i);
        cout << index() << MTM::convert( static_cast<material>(i) ) << endl;
    }
    cout << endl;
}

void Interface::header() {
    cout << "  _____                 ___            _____     _       _        _____     _   _         _  " << endl;
    cout << " |_   _|___ ___ ___ ___|  _|___ ___   |     |___| |_ ___|_|_ _   |     |___| |_| |_ ___ _| | " << endl;
    cout << "   | | |  _| .'|   |_ -|  _| -_|  _|  | | | | .'|  _|  _| |_'_|  | | | | -_|  _|   | . | . | " << endl;
    cout << "   |_| |_| |__,|_|_|___|_| |___|_|    |_|_|_|__,|_| |_| |_|_,_|  |_|_|_|___|_| |_|_|___|___| " << endl;
    cout << "                                                                                             " << endl;
    cout << "  An open-source implementation of the transfer matrix method to" << endl;
    cout << "  solve for reflection and transmission in stratified media" << endl << endl;
    cout << "  =========================================================================================" << endl << endl;
}