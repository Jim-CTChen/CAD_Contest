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
#include <placement.h>
#include <queue>


// Global variables
// const string file_path = "../test/case1.txt";
int maxCellMove = 0;
int row_of_gGrid = 0;
int column_of_gGrid = 0;
int layer_of_gGrid = 0;
int route_len = 0;
int num_routes = 0; 
string output = "";
vector<string> net;

int8_t DEMANDFLAG = 0;
string file_path = "test/case2.txt";

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

unordered_map <string, Cell*> cells;

vector< vector<float> > cvalues_x;

vector< vector<float> > cvalues_y;

vector<float> c0values;

vector<float> d_x;

vector<float> d_y;

vector<Cell*> movable_cells;

vector<Cell*> moved_cells; 

Grid** model = 0;
D_Manager demand_manager; // [row][column][layer]

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;

using namespace std;
// using Eigen::MatrixXd;

int main(int argc, char** argv)
{
    // MatrixXd t(2, 2);
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

    cout << movable_cells.size();
    placement_init();
    for(auto& it : netlists) {
        // cout << "Netlist: " << it.first << endl;
        it.second->B2B_weight_x();
        // cout << "==================" << endl;
    }
    cout << "C value: " << endl;
    for(size_t i = 0; i < cvalues_x.size(); ++i) {
        for(size_t j = 0; j < cvalues_x.size(); ++j) {
            cout << cvalues_x[i][j] << " ";
        }
        cout << endl;
    }
    // cout << endl;
    // for(size_t i = 0; i < d_x.size(); i++) {
    //     cout << d_x[i] << " ";
    // }
    // cout << endl; 
    solveInitialMatrix_x();
    


    // netlistBFS();
    demand_manager.countDemand(true);
    demand_manager.printDemand();
    // demand_manager.printSupply();
    demand_manager.printResult();
    // routing_len();
    // store_output(output_path);
}