#include <iostream>
#include <unordered_map>
#include <string>
#include "router.h"
#include "advance_DS.h"

using namespace std;

class Netlist;
class Pin;
class Steiner_pts;


extern unordered_map <string, Netlist*> netlists;

void Router::netConnection(string n = ""){
    if(n != ""){ // connect specific netlist
        if(netlists.find(n) == netlists.end()) {
            cerr << "No netlist named " << n << " exists!!" << endl;
            return;
        }
        Netlist* net = netlists[n];
        const vector<Pin*>& pins = net->get_pins();
        
    }
}