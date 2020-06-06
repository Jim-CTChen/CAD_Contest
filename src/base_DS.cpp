#include <iostream>
#include <string>
#include <utility>
#include <base_DS.h>
#include <advance_DS.h>
#include <stdlib.h>
using namespace std;
extern unordered_map <string, MasterCell*> mastercells;
extern Demand*** all_demand;

// Cell::Cell(string n, string m, int x, int y, string move){
//     name = n;
//     mc = mastercells[m];
//     coord = pair<int, int>(x, y);
//     if(move == "Fixed") movable = false;
//     else movable = true;
//     pins = mc->pins;
//     for(auto it = pins.begin(); it != pins.end(); it++){
//         it->set_steiner_pts(x, y);
//     }
// }

char Steiner_pts::checkDirection(Steiner_pts* s)
{
    if      (coord.first != s->coord.first) return 'x';
    else if (coord.second != s->coord.second) return 'y';
    else if (layer != s->layer)  return 'z';
}

int Steiner_pts::getDistance(Steiner_pts* s, char d = '0')
{
    if (d == '0') {
        if      (coord.first != s->coord.first) d = 'x';
        else if (coord.second != s->coord.second) d = 'y';
        else if (layer != s->layer)  d = 'z';
    }

    switch (d) {
        case 'x':
            return (s->coord.first - coord.first);
            break;
        case 'y':
            return (s->coord.second - coord.second);
            break;
        case 'z':
            return (s->layer - layer);
            break;
        default:
            cerr << "Error in getDistance()!!" << endl;
            return 0;
    }
}

int Steiner_pts::addDemand()
{
    char d; // direction
    int s;  // distance
    bool p; // positivity of distance
    for(auto it : fanout)
    {
        d = checkDirection(it);
        s = getDistance(it, d);
        if (s < 0) p = false;
        else p = true;
        for(int i = 0; i < abs(s)+1; ++i) // +1 for head and end both need to add demand
        {
            switch (d)
            {
            case 'x':
                if(p) {
                    Demand& tmp = all_demand[coord.first+i][coord.second][layer];
                    if( !tmp.checkFlag() ){ // haven't addDemand in this netlist
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }   
                else  {
                    Demand& tmp = all_demand[coord.first-i][coord.second][layer];
                    if( !tmp.checkFlag() ){
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }
                break;
            
            case 'y':
                if(p) {
                    Demand& tmp = all_demand[coord.first][coord.second+i][layer];
                    if( !tmp.checkFlag() ){
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }   
                else  {
                    Demand& tmp = all_demand[coord.first][coord.second-i][layer];
                    if( !tmp.checkFlag() ){
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }
                break;

            case 'z':
                if(p) {
                    Demand& tmp = all_demand[coord.first][coord.second][layer+i];
                    if( !tmp.checkFlag() ){
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }   
                else  {
                    Demand& tmp = all_demand[coord.first][coord.second][layer-i];
                    if( !tmp.checkFlag() ){
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }
                break;
            default:
                cerr << "Error in adding demand!!" << endl;
                break;
            }
        }
    }
}