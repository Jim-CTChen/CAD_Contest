#ifndef BASE_DT_H
#define BASE_DT_H
//  Contain Layer, Pin, Blockage, MasterCell, Cell

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std; 
extern unordered_map <string, Layer*> layers;
extern unordered_map <string, MasterCell> mastercells;
class Netlist;

class Layer{
    public:
        Layer(string n, int i, char d, int s):name(n), index(i), direction(d), supply(s){}
        string get_name()      { return name;}
        char   get_direction() { return direction; }
        int    get_index()     { return index; }
        int    get_supply()    { return supply; }

    private:
        string name;
        char direction;
        int index = 0;
        int supply = 0;
};

class Steiner_pts{
    public:
        Steiner_pts(int x, int y, int z):layer(z){
            coord = pair<int, int> (x, y);
        }
        ~Steiner_pts();
        void set_fanin(Steiner_pts* s){ fanin = s; }
        void set_fanout(Steiner_pts* s){ fanout.push_back(s); }
        void set_netlist(Netlist* n) { nlist = n; }
        void reset_fanout(){ fanout.clear(); }
    private:
        pair<int, int> coord;
        int layer;
        Steiner_pts* fanin;
        vector<Steiner_pts*> fanout;
        Netlist* nlist;
};

class Pin{
    public:
        Pin(string n, string l):name(n){ layer = layers[l].get_index(); }
        ~Pin(){ delete steiner_pts; }
        void set_steiner_pts(int x, int y){
            steiner_pts = new Steiner_pts(x, y, layer);
        }
    private:
        int layer;
        string name;
        Steiner_pts* steiner_pts = 0;
};

class Blockage{
    public:
        Blockage(string n, string l, int d):name(n), extra_demand(d){
            layer = layers[l].get_index();
        }
    private:
        string name;
        int layer;
        int extra_demand;
};

class MasterCell{
    public:
        MasterCell(string n, int p, int b):name(n){
            pins.reserve(p);
            blockages.reserve(b);
        }
    void set_pin(string, string);
    void set_blockage(string, string, int);
    unordered_map<string, Pin>& get_pins() { return pins; }
    private:
        string name;
        unordered_map<string, Pin> pins;
        vector<Blockage> blockages;
};

class Cell{
    public:
        Cell(string, string, int, int, string);
        ~Cell();
        void moveTo(int, int); // cell movement
    private:
        MasterCell* mc;
        unordered_map<string, Pin> pins;
        pair<int, int> coord;
        bool movable;
        string name;
};











#endif  //BASE_DT_H