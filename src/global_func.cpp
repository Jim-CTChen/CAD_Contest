#include <iostream>
#include <unordered_map>
#include <string>
#include <global_var.h>
#include <global_func.h>
#include <base_DS.h>
#include <advance_DS.h>
using namespace std;

extern int row_of_gGrid;
extern int column_of_gGrid;
extern int layer_of_gGrid;
extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;
extern Grid*** model;


void init()
{
    // form a 3-D model for Grids
    model = new Grid** [row_of_gGrid];
    for(int i = 0; i < row_of_gGrid; ++i)
    {
        model[i] = new Grid* [column_of_gGrid];
        for(int j = 0; j < column_of_gGrid; ++j){
            model[i][j] = new Grid [layer_of_gGrid];
        }
    }
}

void clear()
{
    // delete layers & mastercells
    for(auto it = layers.begin(); it!= layers.end(); it++){
        delete it->second;
    }
    for(auto it = mastercells.begin(); it!= mastercells.end(); it++){
        delete it->second;
    }
    
    // delete model
    for(int i = 0; i < row_of_gGrid;){
        for(int j = 0; j < column_of_gGrid; ++j){
            delete[] model[i][j];
        }
    }
    delete[] model;
}

void add_sameGGrid(string m1, string m2, int l, int ex)
{
    sameGGrids.push_back(SameGGrid(m1, m2, l, ex));
    mastercells[m1]->add_sGGrid(&sameGGrids.back());
    mastercells[m2]->add_sGGrid(&sameGGrids.back());
}

void add_adjhGGrid(string m1, string m2, int l, int ex)
{
    adjGGrids.push_back(AdjHGGrid(m1, m2, l, ex));
    mastercells[m1]->add_aGGrid(&adjGGrids.back());
    mastercells[m2]->add_aGGrid(&adjGGrids.back());
}