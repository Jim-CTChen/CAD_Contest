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
int layer_of_gGrid = 0;
int route_len = 0;
int num_routes = 0; 
string output = "";
vector<string> net;

int8_t DEMANDFLAG = 0;
string file_path = "test/case3.txt";

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

unordered_map <string, Cell*> cells;

Grid** model = 0;
Demand*** all_demand = 0; // [row][column][layer]

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;

using namespace std;
int main(int argc, char** argv)
{
    if(argc == 2) file_path = argv[1];
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


    // netlistBFS();
    // countDemand();
    // printDemand();
    // printSupply();
    routing_len();
    store_output(output_path);
}