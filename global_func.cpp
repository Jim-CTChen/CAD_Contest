#include <iostream>
#include <unordered_map>
#include <string>
#include <global_func.h>
#include <base_DT.h>
using namespace std;

extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;

void clear()
{
    for(auto it = layers.begin(); it!= layers.end(); it++){
        delete it->second;
    }
    for(auto it = mastercells.begin(); it!= mastercells.end(); it++){
        delete it->second;
    }
}

void add_sameGGrid(string m1, string m2, int l, int ex)
{
    sameGGrids.push_back(SameGGrid(m1, m2, l, ex));
    layers[m1]->add_sGGrid(&sameGGrids.back());
    layers[m2]->add_sGGrid(&sameGGrids.back());
}

void add_adjhGGrid(string m1, string m2, int l, int ex)
{
    adjGGrids.push_back(AdjHGGrid(m1, m2, l, ex));
    layers[m1]->add_aGGrid(&adjGGrids.back());
    layers[m2]->add_aGGrid(&adjGGrids.back());
}