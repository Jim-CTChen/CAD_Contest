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
// extern vector<pair<Cell*, float>> displacement;

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
        d_x.push_back(0);
        d_y.push_back(0);
    }

    countC0();
    // cout << "C0: " << endl;
    // for(size_t i = 0; i < movable_cells.size(); ++i) {
    //     cout << c0values[i] << " " << endl;
    // }
    // cout << "==============" << endl;
}

// Only Call countC0 for one time!!

bool cmp_value(pair<Cell*, float> a, pair<Cell*, float> b){ //use for displacement sorting
    return a.second >b.second;
}

void solveInitialMatrix_x() {
    int numOfCells = movable_cells.size();
    if(cvalues_x.size() != numOfCells) {
        cerr << "cvalues_x size does not match!";
        return;
    } 
    if(d_x.size() != numOfCells) {
        cerr << "d_x size does not match!";
        return;
    }
    cout << "building matrices..." << endl;
    MatrixXf C_x(numOfCells, numOfCells);
    VectorXf D_x(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        D_x(i, 0) = -d_x[i];
        for(int j = 0; j < numOfCells; ++j) {
            C_x(i, j) = cvalues_x[i][j];
        }
    }
    cout << "end of building." << endl << "calculating" << endl;
    result = C_x.colPivHouseholderQr().solve(D_x); // new x position for every cell
    cout << "finish" << endl;


    // select cell to move for first initial movement
    if(movable_cells.size() > maxCellMove){
        cout << "selecting cells..." << endl;
        vector<pair<Cell*, float>> displacement;
        for(int i = 0; i < numOfCells; ++i) { // sort placement
            displacement.push_back(pair<Cell*, float>(movable_cells[i],abs(result[i]-movable_cells[i]->get_coord().first)));
            sort(displacement.begin(),displacement.end(),cmp_value);
        }
        for(int i = 0; i < numOfCells; ++i){
            if(i < maxCellMove){
                displacement[i].first->set_index(i);
            }
            else{
                displacement[i].first->set_index(-1);
                displacement[i].first->set_movable(false);
            }
        }
        movable_cells.clear();
        for(int i = 0; i < maxCellMove; ++i){ //construct new movable_cells
            movable_cells.push_back(displacement[i].first);
        }
        displacement.clear();

        cvalues_x.clear();
        d_x.clear();
        for(size_t i = 0; i < movable_cells.size(); ++i) {
            vector<float> tmp;
            for(size_t j = 0; j < movable_cells.size(); ++j) {
                tmp.push_back(0);
            }
            cvalues_x.push_back(tmp);
            d_x.push_back(0);
        }
    cout << "end of selecting" << endl;
    }

    cout << "moving cells..." << endl;
    for(int i = 0; i < numOfCells; ++i) { // change position
        movable_cells[i]->set_X(int(result[i]));
    }
 
    for(int i = 0; i < numOfCells; ++i) {
        cout << movable_cells[i]->get_name() << ": " << endl;
        cout << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")"
             << " >> " << "(" << result[i] << ", " << movable_cells[i]->get_coord().second << ")" << endl;
    }
    cout << "end of moving" << endl;
}

void solveInitialMatrix_y() {
    int numOfCells = movable_cells.size();
    if(cvalues_y.size() != numOfCells) {
        cerr << "cvalues_y size does not match!";
        return;
    } 
    if(d_y.size() != numOfCells) {
        cerr << "d_y size does not match!";
        return;
    } 
    MatrixXf C_y(numOfCells, numOfCells);
    VectorXf D_y(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        D_y(i, 0) = -d_y[i];
        for(int j = 0; j < numOfCells; ++j) {
            C_y(i, j) = cvalues_y[i][j];
        }
    }

    // select cell to move for first initial movement
    vector<pair<Cell*, float>> displacement;
    if(movable_cells.size() > maxCellMove){
        for(int i = 0; i < numOfCells; ++i) { // sort placement
            displacement.push_back(pair<Cell*, float>(movable_cells[i],abs(result[i]-movable_cells[i]->get_coord().second)));
            sort(displacement.begin(),displacement.end(),cmp_value);
        }
        for(int i = 0; i < numOfCells; ++i){
            if(i < maxCellMove){
                displacement[i].first->set_index(i);
            }
            else{
                displacement[i].first->set_index(-1);
                displacement[i].first->set_movable(false);
            }
        }
        movable_cells.clear();
        for(int i = 0; i < maxCellMove; ++i){ //construct new movable_cells
            movable_cells.push_back(displacement[i].first);
        }
        displacement.clear();

        cvalues_x.clear();
        d_y.clear();
        for(size_t i = 0; i < movable_cells.size(); ++i) {
            vector<float> tmp;
            for(size_t j = 0; j < movable_cells.size(); ++j) {
                tmp.push_back(0);
            }
            cvalues_y.push_back(tmp);
            d_y.push_back(0);
        }
    }

    

    
    result = C_y.colPivHouseholderQr().solve(D_y); // new y position for every cell
    for(int i = 0; i < numOfCells; ++i) {
        cout << movable_cells[i]->get_name() << ": "<< endl;
        cout << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")"
             << " >> " << "(" << movable_cells[i]->get_coord().second << ", " << result[i] << ")" << endl;
    }
}
