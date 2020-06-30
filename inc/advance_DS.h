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
        void add_root(Steiner_pts* s) { root = s; }
        void add_pin(Pin* p) { pins.push_back(p); }
        Steiner_pts*& get_root()  {return root;}
        vector<Pin*>& get_pins()  {return pins;} 
        int get_pins_size()  {return pins.size();}
        vector<Steiner_pts*>& get_st_pts() {return steiner_pts;} 
        void add_st_pts( Steiner_pts* s) {steiner_pts.push_back(s);}
        short B2B_weight( Pin*, Pin*); 
        void erase_st_pts(vector<Steiner_pts*>::iterator it) {steiner_pts.erase(it);}
        void count_Cvalue();
    private:
        Steiner_pts*    root = 0;
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
        void add_cell(Cell* c)      { cells.push_back(c); }
        vector<Cell*> get_cells()   { return cells; }
    private:
        vector<Cell*> cells;
        int demand; // netlist 初始化的時候有經過的grid要加
};

class Demand{
    public:
        Demand(){  }
        ~Demand(){}
        // public member function
        void addDemand(int i)   { demand += i; }
        void resetDemand()      { demand = 0; }
        int  getDemand()        { return demand; }
        int  getSupply()        { return supply; }
        void setSupply(int s)   { supply = s;}
        void setFlag()          { flag = DEMANDFLAG; }
        bool checkFlag()        { return (flag == DEMANDFLAG); }
        bool checkValidity()    { return supply >= demand; }
        friend ostream& operator<<(ostream& os, const Demand& dt) { os << dt.demand; }
        // public member variable

    private:
        int demand = 0;
        int supply = 0;
        int8_t flag = 0;
};

#endif // ADVANCE_DS_H