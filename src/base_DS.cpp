#include <iostream>
#include <string>
#include <utility>
#include <base_DS.h>
#include <global_var.h>
using namespace std;
extern unordered_map <string, MasterCell*> mastercells;


Cell::Cell(string n, string m, int x, int y, string move){
    name = n;
    mc = mastercells[m];
    coord = pair<int, int>(x, y);
    if(move == "Fixed") movable = false;
    else movable = true;
    pins = mc->pins;
    for(auto it = pins.begin(); it != pins.end(); it++){
        it->set_steiner_pts(x, y);
    }
}