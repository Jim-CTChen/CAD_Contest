#ifndef BASE_DT_H
#define BASE_DT_H
//  Layer, Steiner_pts, Pin, Blockage, SameGGrid, AdjHGGrid, MasterCell, Cell

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std; 
extern unordered_map <string, Layer*> layers;
extern unordered_map <string, MasterCell*> mastercells;
class Netlist;
class Grid;

class Layer{ 
    public:
        Layer(string n, int i, char d, int s):name(n), index(i), direction(d), supply(s){}
        string get_name()       { return name;}
        char   get_direction()  { return direction; }
        int    get_index()      { return index; }
        int    get_supply()     { return supply; }

    private:
        // data member
        string name;
        char direction;
        int index = 0;
        int supply = 0;
};

class Steiner_pts{
    friend class Pin;
    public:
        Steiner_pts(int x, int y, int z):layer(z){
            coord = pair<int, int> (x, y);
        }
        ~Steiner_pts();
        void set_fanin(Steiner_pts* s)  { fanin = s; }
        void set_fanout(Steiner_pts* s) { fanout.push_back(s); }
        void set_netlist(Netlist* n)    { nlist = n; }
        void reset_fanin()              { fanin = 0; }
        void reset_fanout()             { fanout.clear(); }
        // pair<int, int> get_coord()      { return coord; }
        // int get_layer()                 { return layer; }
    private:
        // data member
        pair<int, int>          coord;
        int                     layer;
        Steiner_pts*            fanin = 0;
        vector<Steiner_pts*>    fanout;
        Netlist*                nlist = 0;
};

class Pin{
    friend class cell;
    public:
        Pin(string n, string l):name(n) { layer = layers[l]->get_index(); }
        ~Pin()                          { delete steiner_pts; }
        void set_steiner_pts(int x, int y){
            steiner_pts = new Steiner_pts(x, y, layer);
        }
    private:
        // data member
        int          layer;
        string       name;
        Steiner_pts* steiner_pts = 0;
};

class Blockage{
    friend class cell;
    public:
        Blockage(string n, string l, int d):name(n), extra_demand(d){
            layer = layers[l]->get_index();
        }
        // int get_layer()         { return layer;}
        // int get_extra_demand()  { return extra_demand; }
    private:
        // data member
        string  name;
        int     layer;
        int     extra_demand;
};

class SameGGrid{
    friend class Grid;
    public:
        SameGGrid(string m1, string m2, int l, int ex):mc1(m1), mc2(m2), layer(l), extra_demand(ex){}
        ~SameGGrid();
        string get_oppenent(const string& s) {
            if(mc1 == s)    return mc2;
            else return mc1;
        }
        // int get_layer()         { return layer; }
        // int get_extra_demand()  { return extra_demand; }
    private:
        // data member
        string  mc1, mc2;
        int     layer;
        int     extra_demand;
};

class AdjHGGrid{
    friend class Grid;
    public:
        AdjHGGrid(string m1, string m2, int l, int ex):mc1(m1), mc2(m2), layer(l), extra_demand(ex);
        ~AdjHGGrid();
        string get_oppenent(const string& s) {
            if(mc1 == s)    return mc2;
            else return mc1;
        }
        // int get_layer()         { return layer; }
        // int get_extra_demand()  { return extra_demand; }
    private:
        // data member
        string  mc1, mc2;
        int     layer;
        int     extra_demand;
};

class MasterCell{
    friend class Cell;
    public:
        MasterCell(string n, int p, int b):name(n){
            pins.reserve(p);
            blockages.reserve(b);
        }
        void set_pin(string n, string l)    { pins.push_back(Pin(n, l)); }
        void set_blockage(string n, string l, int d) 
                                            { blockages.push_back(Blockage(n, l, d)); }
        void add_sGGrid(SameGGrid* s)       { sGGrid.push_back(s); }
        void add_aGGrid(AdjHGGrid* a)       { aGGrid.push_back(a); }
        // vector<Pin>& get_pins()             { return pins; }
    private:    
        // data member
        string                      name;
        vector<Pin>                 pins;
        vector<Blockage>            blockages;
        vector<SameGGrid*>          sGGrid;
        vector<AdjHGGrid*>          aGGrid;
};

class Cell{
    public:
        Cell(string, string, int, int, string);
        ~Cell();
        void moveTo(int, int); // cell movement
        vector<Pin>& get_pins()  { return pins; }
    private:
        // data member
        MasterCell*                 mc = 0;
        vector<Pin>                 pins;
        pair<int, int>              coord;
        bool                        movable;
        string                      name;
};






#endif  //BASE_DT_H