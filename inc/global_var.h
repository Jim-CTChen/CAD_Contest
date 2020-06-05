#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
class Layer;
class MasterCell;
class Netlist;
class SameGGrid;
class AdjHGGrid;
class Grid;

// information
int maxCellMove = 0;

int row_of_gGrid = 0;
int column_of_gGrid = 0;
int layer_of_gGrid = 0;


// data
unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

Grid*** model = 0;

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;






#endif // GLOBAL_VAR_H