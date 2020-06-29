#include <iostream>
#include <vector>
#include "advance_DS.h"
#include "base_DS.h"

using namespace std;

void Netlist::count_Cvalue()
{
    for(auto &it : pins){
        for(auto &iit : pins)
            it->get_cell()->addCvalues(iit->get_cell(), 0);
    }
}