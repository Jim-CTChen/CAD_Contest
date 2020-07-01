#include <iostream>
#include <unordered_map>
#include <string>
#include <eigen3/Eigen/Dense>
#include "global_func.h"
#include "base_DS.h"
#include "advance_DS.h"
#include "definition.h"
#include "placement.h"
#include <queue>
#include <algorithm>

using namespace std;
using namespace Eigen;

// extern const string file_path;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern int layer_of_gGrid;
extern int maxCellMove;
extern string output;
extern vector<string> net;
extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
extern unordered_map <string, Netlist*> netlists;
extern unordered_map <string, Cell*> cells;
extern vector<Cell*> movable_cells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;
extern Grid** model;
extern D_Manager demand_manager;
extern vector< vector<float> > cvalues_x;
extern vector< vector<float> > cvalues_y;
extern vector<float> c0values;
extern vector<float> d_x;
extern vector<float> d_y;

void countC0() {  // P_i/P_avg*(1/numOfCells)
    float numOfPins, avgPins;
    float numOfCells = movable_cells.size();
    for(auto &it : movable_cells) {
        numOfPins += it->get_pins().size();
    }
    avgPins = numOfPins/numOfCells;
    for(auto &it : movable_cells) {
        c0values.push_back(float(it->get_pins().size()) / avgPins * (1/numOfCells));
    }
}

void placement_init() {
    for(size_t i = 0; i < movable_cells.size(); ++i) {
        vector<float> tmp;
        for(size_t j = 0; j < movable_cells.size(); ++j) {
            tmp.push_back(0);
        }
        cvalues_x.push_back(tmp);
        cvalues_y.push_back(tmp);
    }

    for(size_t i = 0; i < movable_cells.size(); ++i) {
        d_x.push_back(0);
        d_y.push_back(0);
    }
    countC0();
    cout << "C0: " << endl;
    for(size_t i = 0; i < movable_cells.size(); ++i) {
        cout << c0values[i] << " " << endl;
    }
    cout << "==============" << endl;
}

// Only Call countC0 for one time!!

void solveInitialMatrix_x() {
    int numOfCells = movable_cells.size();
    MatrixXf C_x(numOfCells, numOfCells);
    VectorXf D_x(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        D_x(i, 0) = -d_x[i];
        for(int j = 0; j < numOfCells; ++j) {
            C_x(i, j) = cvalues_x[i][j];
        }
    }
    result = C_x.colPivHouseholderQr().solve(D_x); // new x position for every cell
    for(int i = 0; i < numOfCells; ++i) {
        cout << movable_cells[i]->get_name() << ": " << endl;
        cout << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")"
             << " >> " << "(" << result[i] << ", " << movable_cells[i]->get_coord().second << ")" << endl;
    }
}

void solveInitialMatrix_y() {
    int numOfCells = movable_cells.size();
    MatrixXf C_y(numOfCells, numOfCells);
    VectorXf D_y(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        D_y(i, 0) = d_y[i];
        for(int j = 0; j < numOfCells; ++j) {
            C_y(i, j) = cvalues_y[i][j];
        }
    }
    result = C_y.colPivHouseholderQr().solve(D_y); // new y position for every cell
    for(int i = 0; i < numOfCells; ++i) {
        cout << movable_cells[i]->get_name() << ": "<< endl;
        cout << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")"
             << " >> " << "(" << movable_cells[i]->get_coord().second << ", " << result[i] << ")" << endl;
    }
}