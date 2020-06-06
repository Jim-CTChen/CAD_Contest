#ifndef ADVANCE_DS_H
#define ADVANCE_DS_H

#include <string>
#include <vector>

using namespace std;
class Cell;
class Steiner_pts;
class Pin;

extern int8_t DEMANDFLAG;

class Netlist{
    public:
        Netlist(){}
        ~Netlist(){}
        void add_root( Steiner_pts* s) { root = s; }
        void add_pin(Pin* p)    { pins.push_back(p); }
        Steiner_pts* get_root() { return root; }
        vector<Pin*> get_pins() { return pins; }
    private:
        Steiner_pts*    root;
        vector<Pin*>    pins;
};                                                              



class Grid{
    public:
        Grid(){}
        ~Grid() { 
            for(int i = 0; i < cells.size(); ++i){
                delete cells[i];
            }
        }
        void add_cell(Cell* c)      { cells.push_back(c); }
        vector<Cell*> get_cells()   { return cells; }
    private:
        vector<Cell*> cells;
        int demand; // netlist 初始化的時候有經過的grid要加
};

class Demand{
    public:
        Demand(){}
        ~Demand(){}
        // public member function
        void addDemand(int i)   { demand += i; }
        void resetDemand()      { demand = 0; }
        int  getDemand()        { return demand; }
        void setFlag()          { flag = DEMANDFLAG; }
        bool checkFlag()        { return (flag == DEMANDFLAG); }
        // public member variable

    private:
        int demand;
        int8_t flag;
};

#endif // ADVANCE_DS_H