#ifndef ROUTER_H
#define ROUTER_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Netlist;
class Pin;
class Steiner_pts;

extern unordered_map <string, Netlist*> netlists;

class Router{
    public:
        Router(unordered_map <string, Netlist*> n): netlists_r(n){}
        ~Router() {}
        void netConnection(string);
        
    private:
        vector<Pin*> pins;
        unordered_map <string, Netlist*> netlists_r;

};





#endif // ROUTER_H