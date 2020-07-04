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

vector<float> phi_x;

vector<float> phi_y;

// vector<pair<Cell*, float>> displacement;

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
    
    // cout << "1" << endl;
    readNumNonDefaultSupplyGGrid();
    // cout << "2" << endl;
    readMasterCell();
    // cout << "3" << endl;
    readNeighborCellExtraDemand();
    // cout << "4" << endl;
    readCellInst();
    // cout << "5" << endl;
    readNets();
    // readRoutes();

    // cout << movable_cells.size();
    // placement_init();
    // for(auto& it : netlists) {
    //     // cout << "Netlist: " << it.first << endl;
        // it.second->B2B_weight_x();
    //     // cout << "==================" << endl;
    // }
    // cout << "C value: " << endl;
    // for(size_t i = 0; i < cvalues_x.size(); ++i) {
    //     for(size_t j = 0; j < cvalues_x.size(); ++j) {
    //         cout << cvalues_x[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    // for(size_t i = 0; i < d_x.size(); i++) {
    //     cout << d_x[i] << " ";
    // }
    // cout << endl; 
    // solveInitialMatrix_x();
    


    // netlistBFS();
    // demand_manager.countDemand(true);
    // demand_manager.printDemand();
    // demand_manager.printSupply();

    
    // cout << "intial result: " << endl;
    // demand_manager.countDemand(true);
    // demand_manager.printDemand();
    // demand_manager.printResult();
    // cout << "==============================================" << endl;

    placement_init();
    calculateCvalue_x();
    solveInitialMatrix_x();
    calculateCvalue_x();
    solveInitialMatrix_x();
    calculateCvalue_x();
    solveInitialMatrix_x();
    // cout << "C510 1" << endl;
    countC0();
    // cout << "C0" << endl;
    // cout << cells["C510"]->get_coord().first << endl;
    // for(int i = 0; i < c0values.size(); ++i) cout << c0values[i] << " ";
    // cout << endl;
    // cout << "C510 2" << endl;
    // cout << cells["C510"]->get_coord().first << endl;
    // cout << "count demand" << endl;
    demand_manager.countDemand(true);
    // cerr << "counting phi_x" << endl;
    // cout << "C510 3" << endl;
    // cout << cells["C510"]->get_coord().first << endl;
    calculateCvalue_x();
    // cout << "Cvalue" << endl;
    // for(int i = 0; i < cvalues_x.size(); ++i) {
    //     for(int j = 0; j < cvalues_x.size(); ++j){
    //         cout << cvalues_x[i][j] << " ";
    //     }
    //     cout << endl;
    // }

        
    // cout << endl;

    calculate_phi_x();
    solveGlobalMatrix_x();
    // cout << "count demand" << endl;
    demand_manager.countDemand(true);
    // cerr << "counting phi_x" << endl;
    calculateCvalue_x();
    calculate_phi_x();
    solveGlobalMatrix_x();

    // demand_manager.countDemand(true);
    cerr << "exporting...." << endl;
    store_cell_pic(cell_output_path);
    cerr << "finish!!!!!!!!!!" << endl;
    
    // cout << "first movement: " << endl; 
    // demand_manager.countDemand(true);
    // demand_manager.printDemand();
    // demand_manager.printResult();
    // cout << "==============================================" << endl;

    // calculateCvalue_x();
    // solveInitialMatrix_x();
    // cout << "second movement: " << endl;
    // demand_manager.countDemand(true);
    // demand_manager.printDemand();
    // demand_manager.printResult();
    // // routing_len();
    // // store_output(output_path);
    // cout << "counting C0" << endl;
    // countC0();

    // cout << "counting phi_x" << endl;
    // calculate_phi_x();
    // cout << "solveGlobalMatrix_x" << endl;
    // solveGlobalMatrix_x();
    // return 0;
}