#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <global_func.h>
#include <base_DS.h>
#include <advance_DS.h>
#include <utility.h>
#include <definition.h>
#include <queue>
#include <algorithm>
using namespace std;

// extern const string file_path;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern int layer_of_gGrid;
extern int maxCellMove;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern int route_len;
extern int num_routes;
extern string output;
extern vector<string> net;
extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
extern unordered_map <string, Netlist*> netlists;
extern unordered_map <string, Cell*> cells;
extern vector<Cell* > moved_cells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;
extern Grid** model;
extern Demand*** all_demand;
