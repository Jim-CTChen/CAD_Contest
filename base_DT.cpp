#include <iostream>
#include <string>
#include <utility>
#include <base_DT.h>
using namespace std;
extern unordered_map <string, MasterCell> mastercells;

void MasterCell::set_pin(string n, string l){
    pair<string, Pin> p (n, Pin(n, l));
    pins.insert(p);
}
void MasterCell::set_blockage(string n, string l, int d){
    blockages.push_back(Blockage(n, l, d));
}


Cell::Cell(string n, string m, int x, int y, string move){
    name = n;
    mc = &mastercells[m];
    coord = pair<int, int>(x, y);
    if(move == "Fixed") movable = false;
    else movable = true;
    pins = mc->get_pins();
    for(auto it = pins.begin(); it != pins.end(); it++){
        it->second.set_steiner_pts(x, y);
    }
}