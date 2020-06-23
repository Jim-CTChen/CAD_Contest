#include <iostream>
#include <string>
#include <utility>
#include <base_DS.h>
#include <advance_DS.h>
#include <stdlib.h>
#include <algorithm>
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
    else { cerr << "Not in same line!!!" << endl; return '0'; }
}

int Steiner_pts::getDistance(Steiner_pts* s, char d = '0')
{
    if (d == '0') {
        if      (coord.first != s->coord.first) d = 'x';
        else if (coord.second != s->coord.second) d = 'y';
        else if (layer != s->layer)  d = 'z';
        else { cerr << "Not in same line!!!" << endl; return 0; }
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

void Steiner_pts::addDemand()
{
    char d; // direction
    int s;  // distance
    bool p; // positivity of distance
    if(fanout.empty())  return; // leaf case
    for(auto it : fanout)
    {
        d = checkDirection(it);
        s = getDistance(it, d);
        if (s < 0) p = false;
        else p = true;
        
        // debug
        // cerr << *this << " " << *it << " " << d << " " << s << endl;

        for(int i = 0; i < abs(s)+1; ++i) // +1 for head and end both need to add demand
        {
            switch (d)
            {
            case 'x':
                if(p) {
                    Demand& tmp = all_demand[coord.first+i-1][coord.second-1][layer-1];
                    if( !tmp.checkFlag() ){ // haven't addDemand in this netlist
                        // cerr << '\t' << coord.first+i << " " << coord.second << " " << layer << " +1" << endl;
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }
                else  {
                    Demand& tmp = all_demand[coord.first-i-1][coord.second-1][layer-1];
                    if( !tmp.checkFlag() ){
                        // cerr << '\t' << coord.first-i << " " << coord.second << " " << layer << " +1" << endl;
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }
                break;
            
            case 'y':
                if(p) {
                    Demand& tmp = all_demand[coord.first-1][coord.second+i-1][layer-1];
                    if( !tmp.checkFlag() ){
                        // cerr << '\t' << coord.first << " " << coord.second+i << " " << layer << " +1" << endl;
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }   
                else  {
                    Demand& tmp = all_demand[coord.first-1][coord.second-i-1][layer-1];
                    if( !tmp.checkFlag() ){
                        // cerr << '\t' << coord.first << " " << coord.second-i << " " << layer << " +1" << endl;
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }
                break;

            case 'z':
                if(p) {
                    Demand& tmp = all_demand[coord.first-1][coord.second-1][layer+i-1];
                    if( !tmp.checkFlag() ){
                        // cerr << '\t' << coord.first << " " << coord.second << " " << layer+i << " +1" << endl;
                        tmp.addDemand(1);
                        tmp.setFlag();
                    }
                }   
                else  {
                    Demand& tmp = all_demand[coord.first-1][coord.second-1][layer-i-1];
                    if( !tmp.checkFlag() ){
                        // cerr << '\t' << coord.first << " " << coord.second << " " << layer-i << " +1" << endl;
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

Cell::Cell(string n, string m, int x, int y, string move){
    name = n;
    mc = mastercells[m];
    coord = pair<int, int>(x, y);
    if(move == "Fixed") movable = false;
    else movable = true;
    pins = mc->pins;
    for(auto it = pins.begin(); it != pins.end(); it++){
        it->set_steiner_pts(x, y);
        it->cell = this;
    }
}

bool Steiner_pts:: operator== (Steiner_pts& pts){
    return (pts.get_coord() == (*this).get_coord()) && (pts.get_layer() == (*this).get_layer());
}

bool Steiner_pts:: is_fanout(Steiner_pts* s){
    for(auto it = fanout.begin(); it != fanout.end(); it++){
        if(*it == s){
            return true;
        }
    }
    return false;
}

void Steiner_pts:: del_fanout(Steiner_pts* s){
    vector<Steiner_pts*>::iterator it;
    it = find (fanout.begin(), fanout.end(), s);
    fanout.erase(it);
}