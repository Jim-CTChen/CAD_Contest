#ifndef ADVANCE_DS_H
#define ADVANCE_DS_H

#include <string>
#include <vector>

using namespace std;
class Cell;
class Steiner_pts;
class Pin;

class Netlist{
    public:
        Netlist(){}
        ~Netlist(){}
        void add_root( Steiner_pts* s) { root = s; }
        void add_pin(Pin* p) { pins.push_back(p); }
        Steiner_pts*& get_root()  {return root;}
        vector<Pin*>& get_pins() {return pins;} 
        vector<Steiner_pts*>& get_st_pts() {return steiner_pts;} 
        void add_st_pts( Steiner_pts* s) {steiner_pts.push_back(s);}
        void erase_st_pts(vector<Steiner_pts*>::iterator it) {steiner_pts.erase(it);}
    private:
        Steiner_pts*    root;
        vector<Pin*>    pins;
        vector<Steiner_pts*>    steiner_pts;
};                                                              



class Grid{
    public:
        Grid(){}
        ~Grid() { 
            for(int i = 0; i < cells.size(); ++i){
                delete cells[i];
            }
        }
        void add_cell(Cell* c) { cells.push_back(c); }
    private:
        vector<Cell*> cells;
        int demand; // netlist 初始化的時候有經過的grid要加
};

class Demand{
    public:
        Demand(){}
        ~Demand(){}
    private:
        int demand;
};

#endif // ADVANCE_DS_H