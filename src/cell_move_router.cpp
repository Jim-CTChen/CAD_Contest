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
    int numOfInitial = 0;
    int numOfGlobal = 0;
    for(int i = 0; i < argc; i++) {
        string str = argv[i];
        if(str == "-p") {
            file_path = argv[i+1];
        }
        if(str == "-i") {
            string istr = argv[i+1];
            int raw = stoi(istr);
            if(raw < 0) {
                cerr << "Invalid parameter for -i!" << endl;
                return 0;
            }
            numOfInitial = stoi(istr);
        }
        if(str == "-g") {
            string istr = argv[i+1];
            int raw = stoi(istr);
            if(raw < 0) {
                cerr << "Invalid parameter for -g!" << endl;
                return 0;
            }
            numOfGlobal = stoi(istr);
        }
    }
    ifstream infile;
    infile.open(file_path);
    if(!infile.is_open()) {
        cerr << "Invalid file path!" << endl;
        return 0;
    }
    infile.close();
    if(numOfInitial == 0 && numOfGlobal > 0) {
        cerr << "Must do initial placement before global placement!" << endl;
        return 0;
    }
    // cerr << "path: " << file_path << " -i: " << numOfInitial << " -g: " << numOfGlobal << endl;
    cerr << "Reading input file..." << endl;
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
    cerr << "Done reading!" << endl;

    if(numOfInitial) {
        placement_init();
        cerr << "Doing initial placement..." << endl;
        calculateCvalue_x();
        calculateCvalue_y();
        choose_movable();
        for(int i = 0; i < (numOfInitial-1); ++i) {
            calculateCvalue_x();
            solveInitialMatrix_x();
            calculateCvalue_y();
            solveInitialMatrix_y();
        }
        cerr << "Done initial placement!" << endl;
        if(numOfGlobal) {
            cerr << "Doing global placement..." << endl;
            countC0();
            for(int i = 0; i < numOfGlobal; ++i) {
                demand_manager.countDemand(true);
                calculateCvalue_x();
                calculateCvalue_y();
                calculate_phi_x();
                calculate_phi_y();
                solveGlobalMatrix_x();
                solveGlobalMatrix_y();
            }
            cerr << "Done global placement!" << endl;
        }
    }

    // output files
    
    demand_manager.countDemand(true);
    cerr << "Exporting output file test/cell.txt..." << endl;
    store_cell_pic(cell_output_path);
    cerr << "Exporting output file test/demand.txt..." << endl;
    for(int i = 0; i < layer_of_gGrid; i++) {
        store_demand_pic(demand_output_path, i);
    }
    cerr << "Done exporting!" << endl;

    
    // clear();
}