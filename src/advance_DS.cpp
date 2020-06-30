#include <iostream>
#include <vector>
#include "advance_DS.h"
#include "base_DS.h"

using namespace std;

extern int row_of_gGrid;
extern int column_of_gGrid;

short Netlist::B2B_weight( Pin* a, Pin* b){
    int max = 0 , min = row_of_gGrid;
    Pin* p_r = 0;
    Pin* p_l = 0;
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it)->get_cell()->get_coord().first > max){
            p_r = *it;
            max = (*it)->get_cell()->get_coord().first;
        }
        else if((*it)->get_cell()->get_coord().first < min){
            p_l = *it;
            min = (*it)->get_cell()->get_coord().first;
        }
    }
    if(a != p_r && b != p_l && a != p_l && b != p_r){
        return 0;
    }
    else{
        return 2/(pins.size()*(a->get_cell()->get_coord().first-b->get_cell()->get_coord().first));
    }
}





using namespace std;
