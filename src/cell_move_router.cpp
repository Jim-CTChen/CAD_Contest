#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <utility.h>
#include <base_DS.h>
#include <advance_DS.h>
#include <global_func.h>
#include <definition.h>

// Global variables
// const string file_path = "../test/case3.txt";
int maxCellMove = 0;
int row_of_gGrid = 0;
int column_of_gGrid = 0;
int layer_of_gGrid = 0;
int8_t DEMANDFLAG = 0;

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

unordered_map <string, Cell*> cells;

Grid** model = 0;
Demand*** all_demand = 0;

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;

using namespace std;
int main()
{
    init();
    readMaxCellMove();
    readGGridBoundaryIdx();
    readLayer();
    readNumNonDefaultSupplyGGrid();
    readMasterCell();
    readNeighborCellExtraDemand();
    readCellInst();
    readNets();
    readRoutes();
    cout << (netlists["N1"]->get_root()->get_fanout()).size();
}