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
#include <queue>

// Global variables
// const string file_path = "../test/case3.txt";
int maxCellMove = 0;
int row_of_gGrid = 0;
int column_of_gGrid = 0;
int layer_of_gGrid = 3;
int8_t DEMANDFLAG = 0;

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

unordered_map <string, Cell*> cells;

Grid** model = 0;
Demand*** all_demand = 0; // [row][column][layer]

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;

using namespace std;
int main()
{
    readMaxCellMove();
    readGGridBoundaryIdx();
    readLayer();

    init(); // after reading in row, column, layer, init first

    readNumNonDefaultSupplyGGrid();
    readMasterCell();
    readNeighborCellExtraDemand();
    readCellInst();
    readNets();
    readRoutes();
    // cout << (netlists["N1"]->get_root()->get_fanout()).size();

    // for(auto it : mastercells["MC1"]->get_blkgs())  {
    //     cerr << it.get_layer() << " " << it.get_extra_demand() << endl;
    // }
    // cerr << mastercells["MC1"]->get_blkgs()[0].get_layer << " "
    // if(netlists["N1536"]->get_root() != 0) cerr << *netlists["N1536"]->get_root() << endl;
    // netlistBFS();
    // countDemand();
    // printDemand();
    // printSupply();
}