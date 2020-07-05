#include <iostream>
#include <unordered_map>
#include <string>
#include "eigen3/Eigen/Dense"
#include "global_func.h"
#include "base_DS.h"
#include "advance_DS.h"
#include "definition.h"
#include "placement.h"
#include <queue>
#include <algorithm>
#include <stdio.h>  
#include <math.h>

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
extern vector<float> phi_x;
extern vector<float> phi_y;



// init cvalues_x & cvalues_y & d_x & d_y
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

}

// init C0 
// do it after at least one time of initial placement!
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

bool cmp_value(pair<Cell*, float> a, pair<Cell*, float> b){ //use for displacement sorting
    return a.second > b.second;
}

void calculateCvalue_x()
{
    for(auto& it : netlists) {
        it.second->B2B_weight_x();
    }
}

void calculateCvalue_y()
{
    for(auto& it : netlists) {
        it.second->B2B_weight_y();
    }
}

void choose_movable(){
    int numOfCells = movable_cells.size();
    vector<pair<Cell*, float>> displacement;
    MatrixXf C_x(numOfCells, numOfCells), C_y(numOfCells, numOfCells);
    VectorXf D_x(numOfCells), result_x(numOfCells), D_y(numOfCells), result_y(numOfCells), result_total(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        D_x(i, 0) = -d_x[i];
        D_y(i, 0) = -d_y[i];
        for(int j = 0; j < numOfCells; ++j) {
            C_x(i, j) = cvalues_x[i][j];
            C_y(i, j) = cvalues_y[i][j];
        }
    }
    result_x = C_x.colPivHouseholderQr().solve(D_x); 
    result_y = C_y.colPivHouseholderQr().solve(D_y);
    if(numOfCells > maxCellMove){    
        result_total = result_x + result_y;
        for(int i = 0; i < numOfCells; ++i) { // sort placement
            displacement.push_back(pair<Cell*, float>(movable_cells[i],
            abs(result_total[i]-movable_cells[i]->get_coord().first-movable_cells[i]->get_coord().second)));
            displacement[i].first->set_former_index(i);
        }
        sort(displacement.begin(),displacement.end(),cmp_value);
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
        numOfCells = movable_cells.size();
        VectorXf temp_x(numOfCells), temp_y(numOfCells);
        for(int i = 0; i < numOfCells; ++i) {
            temp_x[i] = result_x[displacement[i].first->get_former_index()];
            temp_y[i] = result_y[displacement[i].first->get_former_index()];
        }
        for(int i = 0; i < numOfCells; ++i) { // change position
            int x = (movable_cells[i]->get_coord().first + 
                ((int(result_x[i]) - movable_cells[i]->get_coord().first)/INITIAL_DISTANCE_RATE));
            if(x > row_of_gGrid)    x = row_of_gGrid;
            if(x < 1) x = 1;
            movable_cells[i]->set_X(x);
            int y = movable_cells[i]->get_coord().second + 
                ((int(result_y[i]) - movable_cells[i]->get_coord().second)/INITIAL_DISTANCE_RATE);
            if(y > column_of_gGrid) y = column_of_gGrid;
            if(y < 1) y = 1;
            movable_cells[i]->set_Y(y);
        }
        displacement.clear();
        cvalues_x.clear();
        cvalues_y.clear();
        d_x.clear();
        d_y.clear();
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
    }
    else{
        for(int i = 0; i < numOfCells; ++i) { // change position
            int x = (movable_cells[i]->get_coord().first + 
                ((int(result_x[i]) - movable_cells[i]->get_coord().first)/INITIAL_DISTANCE_RATE));
            if(x > row_of_gGrid)    x = row_of_gGrid;
            if(x < 1) x = 1;
            movable_cells[i]->set_X(x);
            int y = movable_cells[i]->get_coord().second + 
                ((int(result_y[i]) - movable_cells[i]->get_coord().second)/INITIAL_DISTANCE_RATE);
            if(y > column_of_gGrid) y = column_of_gGrid;
            if(y < 1) y = 1;
            movable_cells[i]->set_Y(y);
        }
    }   
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
    // cerr << "building matrices..." << endl;
    MatrixXf C_x(numOfCells, numOfCells);
    VectorXf D_x(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        D_x(i, 0) = -d_x[i];
        for(int j = 0; j < numOfCells; ++j) {
            C_x(i, j) = cvalues_x[i][j];
        }
    }
    // cerr << "end of building." << endl << "calculating initial x..." << endl;
    result = C_x.colPivHouseholderQr().solve(D_x); // new x position for every cell
    // cerr << "finish" << endl;


    // select cell to move for first initial movement
    // cerr << "end of selecting" << endl;

    // for(int i = 0; i < numOfCells; ++i) { // printing result
    //     cerr << movable_cells[i]->get_name() << ": " << endl;
    //     cerr << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")"
    //          << " >> " << "(" << result[i] << ", " << movable_cells[i]->get_coord().second << ")" << endl;
    // }

    // cerr << "moving cells..." << endl;
    for(int i = 0; i < numOfCells; ++i) { // change position
        int x = (movable_cells[i]->get_coord().first + 
            ((int(result[i]) - movable_cells[i]->get_coord().first)/INITIAL_DISTANCE_RATE));
        if(x > row_of_gGrid)    x = row_of_gGrid;
        if(x < 1) x = 1;
        movable_cells[i]->set_X(x);
    }
 
    // cerr << "end of moving" << endl;
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

    result = C_y.colPivHouseholderQr().solve(D_y);

    // for(int i = 0; i < numOfCells; ++i) {
    //     cout << movable_cells[i]->get_name() << ": "<< endl;
    //     cout << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")"
    //          << " >> " << "(" << movable_cells[i]->get_coord().second << ", " << result[i] << ")" << endl;
    // }
    for(int i = 0; i < numOfCells; ++i) { // change position
        int y = movable_cells[i]->get_coord().second + 
                        ((int(result[i]) - movable_cells[i]->get_coord().second)/INITIAL_DISTANCE_RATE);
        if(y > column_of_gGrid) y = column_of_gGrid;
        if(y < 1) y = 1;
        movable_cells[i]->set_Y(y);
    }
}

void solveGlobalMatrix_x() {
    int numOfCells = movable_cells.size();
    if(phi_x.size() != numOfCells) {
        cerr << "size of phi_x does not match!!" << endl;
        return;
    }
    if(c0values.size() != numOfCells) {
        cerr << "size of c0values does not match!!" << endl;
        return;
    }
    if(cvalues_x.size() != numOfCells) {
        cerr << "size of C_x does not match!!" << endl;
        return;
    }

    MatrixXf C(numOfCells, numOfCells);
    VectorXf P(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        for(int j = 0; j < numOfCells; ++j) {
            C(i, j) = cvalues_x[i][j];
            if(i == j) C(i, j) += c0values[i];
        }
        P(i) = -c0values[i]*phi_x[i];
    }
    // cout << "phi" << endl;
    // cout << P << endl;
    // cout << "calculating global x..." << endl;
    result = C.colPivHouseholderQr().solve(P);
    // cout << "finish" << endl;


    phi_x.clear();
    vector <int> previous_pos;
    for(int i = 0; i < numOfCells; ++i) {
        previous_pos.push_back(movable_cells[i]->get_coord().first);
    }

    

    // moving...
    // cerr << "moving cells..." << endl;
    for(int i = 0; i < numOfCells; ++i) { // change position if out of range then move to boundary
        if((int(result[i])+movable_cells[i]->get_coord().first) > row_of_gGrid) movable_cells[i]->set_X(row_of_gGrid);
        else if((int(result[i])+movable_cells[i]->get_coord().first) <= 0 ) movable_cells[i]->set_X(1);
        else   movable_cells[i]->set_X((int(result[i]) + movable_cells[i]->get_coord().first));
    }

    // for(int i = 0; i < numOfCells; ++i) { // print movement
    //     cerr << movable_cells[i]->get_name() << ": " << endl;
    //     cerr << "(" << previous_pos[i] << ", " << movable_cells[i]->get_coord().second << ")"
    //          << " >> " << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")" << endl;
    // }

    // cerr << "end of moving" << endl;
}

void solveGlobalMatrix_y() {
    int numOfCells = movable_cells.size();
    if(phi_y.size() != numOfCells) {
        cerr << "size of phi_y does not match!!" << endl;
        return;
    }
    if(c0values.size() != numOfCells) {
        
        cerr << "size of c0values does not match!!" << endl;
        return;
    }
    if(cvalues_y.size() != numOfCells) {
        cerr << "size of C_y does not match!!" << endl;
        return;
    }

    MatrixXf C(numOfCells, numOfCells);
    VectorXf P(numOfCells), result(numOfCells);
    for(int i = 0; i < numOfCells; ++i) {
        for(int j = 0; j < numOfCells; ++j) {
            C(i, j) = cvalues_y[i][j];
            if(i == j) C(i, j) += c0values[i];
        }
        P(i) = -c0values[i]*phi_y[i];
    }
    // cout << "calculating global y..." << endl;
    result = C.colPivHouseholderQr().solve(P); // solve pos y
    // cout << "finish" << endl;

    phi_y.clear();
    vector<int> previous_pos;
    for(size_t i = 0; i < numOfCells; ++i) {
        previous_pos.push_back(movable_cells[i]->get_coord().second);
    }

    // cerr << "moving cells..." << endl;
    for(int i = 0; i < numOfCells; ++i) { // cchange position if out of range then move to boundary
        if((int(result[i]) + movable_cells[i]->get_coord().second) > column_of_gGrid)   movable_cells[i]->set_Y(column_of_gGrid);
        else if((int(result[i]) + movable_cells[i]->get_coord().second) <= 0) movable_cells[i]->set_Y(1);
        else movable_cells[i]->set_Y((int(result[i]) + movable_cells[i]->get_coord().second));
    }

    
 
    // for(int i = 0; i < numOfCells; ++i) { // print movement
    //     cerr << movable_cells[i]->get_name() << ": " << endl;
    //     cerr << "(" << movable_cells[i]->get_coord().first << ", " << previous_pos[i] << ")"
    //          << " >> " << "(" << movable_cells[i]->get_coord().first << ", " << movable_cells[i]->get_coord().second << ")" << endl;
    // }
    // cerr << "end of moving" << endl;
}

void calculate_phi_x () {
    int phi[row_of_gGrid][column_of_gGrid];
    int x_max, x_min, y_max, y_min;
    // float k = GLOBAL_K_CONST;
    // float total = cells.size();
    // float maxcellmove = maxCellMove;
    // int r = (maxcellmove/total)*100 - BASE_MOVABLE_RATIO;
    // if(r > 0) k *= r;
    // else k /= r;
    for(int i = 0; i < row_of_gGrid; i++){
        for(int j = 0; j < column_of_gGrid; j++){
            phi[i][j] = 0; 
        }
    }
    for(auto it = movable_cells.begin(); it != movable_cells.end(); it++){
        if((*it)->get_coord().first <= row_of_gGrid*RANGE_OF_PHI/100){
            if((*it)->get_coord().second <= column_of_gGrid*RANGE_OF_PHI/100){
                x_max = 2*row_of_gGrid*RANGE_OF_PHI/100;
                x_min = 0;
                y_max = 2*column_of_gGrid*RANGE_OF_PHI/100;
                y_min = 0;
            }
            else if((*it)->get_coord().second >= column_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().second <= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = 2*row_of_gGrid*RANGE_OF_PHI/100;
                x_min = 0;
                y_max = (*it)->get_coord().second + column_of_gGrid*RANGE_OF_PHI/100;
                y_min = (*it)->get_coord().second - column_of_gGrid*RANGE_OF_PHI/100;
            }
            else if((*it)->get_coord().second >= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = 2*row_of_gGrid*RANGE_OF_PHI/100;
                x_min = 0;
                y_max = column_of_gGrid;
                y_min = column_of_gGrid - 2*column_of_gGrid*RANGE_OF_PHI/100;
            }
        }

        else if((*it)->get_coord().first >= row_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().first <= (row_of_gGrid - row_of_gGrid*RANGE_OF_PHI/100)){
            if((*it)->get_coord().second <= column_of_gGrid*RANGE_OF_PHI/100){
                x_max = (*it)->get_coord().first + row_of_gGrid*RANGE_OF_PHI/100;
                x_min = (*it)->get_coord().first - row_of_gGrid*RANGE_OF_PHI/100;
                y_max = 2*column_of_gGrid*RANGE_OF_PHI/100;
                y_min = 0;
            }
            else if((*it)->get_coord().second >= column_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().second <= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = (*it)->get_coord().first + row_of_gGrid*RANGE_OF_PHI/100;
                x_min = (*it)->get_coord().first - row_of_gGrid*RANGE_OF_PHI/100;
                y_max = (*it)->get_coord().second + column_of_gGrid*RANGE_OF_PHI/100;
                y_min = (*it)->get_coord().second - column_of_gGrid*RANGE_OF_PHI/100;
            }
            else if((*it)->get_coord().second >= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = (*it)->get_coord().first + row_of_gGrid*RANGE_OF_PHI/100;
                x_min = (*it)->get_coord().first - row_of_gGrid*RANGE_OF_PHI/100;
                y_max = column_of_gGrid;
                y_min = column_of_gGrid - 2*column_of_gGrid*RANGE_OF_PHI/100;
            }
        }

        else if((*it)->get_coord().first >= (row_of_gGrid - row_of_gGrid*RANGE_OF_PHI/100)){
            if((*it)->get_coord().second <= column_of_gGrid*RANGE_OF_PHI/100){
                x_max = row_of_gGrid;
                x_min = row_of_gGrid - 2*row_of_gGrid*RANGE_OF_PHI/100;
                y_max = 2*column_of_gGrid*RANGE_OF_PHI/100;
                y_min = 0;
            }
            else if((*it)->get_coord().second >= column_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().second <= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = row_of_gGrid;
                x_min = row_of_gGrid - 2*row_of_gGrid*RANGE_OF_PHI/100;
                y_max = (*it)->get_coord().second + column_of_gGrid*RANGE_OF_PHI/100;
                y_min = (*it)->get_coord().second - column_of_gGrid*RANGE_OF_PHI/100;
            }
            else if((*it)->get_coord().second >= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = row_of_gGrid;
                x_min = row_of_gGrid - 2*row_of_gGrid*RANGE_OF_PHI/100;
                y_max = column_of_gGrid;
                y_min = column_of_gGrid - 2*column_of_gGrid*RANGE_OF_PHI/100;
            }
        }

        for(int i = x_min; i < x_max; i++){
            for(int j = y_min; j < y_max; j++){
                for(int k = 0; k < layer_of_gGrid; k++){
                    if((*it)->get_coord().first != i){
                        phi[(*it)->get_coord().first-1][(*it)->get_coord().second-1] += 
                        demand_manager.demands[i][j][k].getDemand()*GLOBAL_K_CONST*(-((*it)->get_coord().first-i)) /
                            pow((pow(((*it)->get_coord().first-i),2)+pow(((*it)->get_coord().second-j),2)),(3/2));
                    }
                }
                
            }
        }
    }
    for(auto it = movable_cells.begin(); it != movable_cells.end(); it++){
        phi_x.push_back(phi[(*it)->get_coord().first][(*it)->get_coord().second]);	
    }
	
}

void calculate_phi_y () {
    int phi[row_of_gGrid][column_of_gGrid];
    int x_max, x_min, y_max, y_min;  
    // float k = GLOBAL_K_CONST;
    // float total = cells.size();
    // float maxcellmove = maxCellMove;
    // int r = (maxcellmove/total)*100 - BASE_MOVABLE_RATIO;
    // if(r > 0) k *= r;
    // else k /= r;
    for(int i = 0; i < row_of_gGrid; i++){
        for(int j = 0; j < column_of_gGrid; j++){
            phi[i][j] = 0; 
        }
    }
    for(auto it = movable_cells.begin(); it != movable_cells.end(); it++){
        if((*it)->get_coord().first <= row_of_gGrid*RANGE_OF_PHI/100){
            if((*it)->get_coord().second <= column_of_gGrid*RANGE_OF_PHI/100){
                x_max = 2*row_of_gGrid*RANGE_OF_PHI/100;
                x_min = 0;
                y_max = 2*column_of_gGrid*RANGE_OF_PHI/100;
                y_min = 0;
            }
            else if((*it)->get_coord().second >= column_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().second <= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = 2*row_of_gGrid*RANGE_OF_PHI/100;
                x_min = 0;
                y_max = (*it)->get_coord().second + column_of_gGrid*RANGE_OF_PHI/100;
                y_min = (*it)->get_coord().second - column_of_gGrid*RANGE_OF_PHI/100;
            }
            else if((*it)->get_coord().second >= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = 2*row_of_gGrid*RANGE_OF_PHI/100;
                x_min = 0;
                y_max = column_of_gGrid;
                y_min = column_of_gGrid - 2*column_of_gGrid*RANGE_OF_PHI/100;
            }
        }

        else if((*it)->get_coord().first >= row_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().first <= (row_of_gGrid - row_of_gGrid*RANGE_OF_PHI/100)){
            if((*it)->get_coord().second <= column_of_gGrid*RANGE_OF_PHI/100){
                x_max = (*it)->get_coord().first + row_of_gGrid*RANGE_OF_PHI/100;
                x_min = (*it)->get_coord().first - row_of_gGrid*RANGE_OF_PHI/100;
                y_max = 2*column_of_gGrid*RANGE_OF_PHI/100;
                y_min = 0;
            }
            else if((*it)->get_coord().second >= column_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().second <= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = (*it)->get_coord().first + row_of_gGrid*RANGE_OF_PHI/100;
                x_min = (*it)->get_coord().first - row_of_gGrid*RANGE_OF_PHI/100;
                y_max = (*it)->get_coord().second + column_of_gGrid*RANGE_OF_PHI/100;
                y_min = (*it)->get_coord().second - column_of_gGrid*RANGE_OF_PHI/100;
            }
            else if((*it)->get_coord().second >= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = (*it)->get_coord().first + row_of_gGrid*RANGE_OF_PHI/100;
                x_min = (*it)->get_coord().first - row_of_gGrid*RANGE_OF_PHI/100;
                y_max = column_of_gGrid;
                y_min = column_of_gGrid - 2*column_of_gGrid*RANGE_OF_PHI/100;
            }
        }

        else if((*it)->get_coord().first >= (row_of_gGrid - row_of_gGrid*RANGE_OF_PHI/100)){
            if((*it)->get_coord().second <= column_of_gGrid*RANGE_OF_PHI/100){
                x_max = row_of_gGrid;
                x_min = row_of_gGrid - 2*row_of_gGrid*RANGE_OF_PHI/100;
                y_max = 2*column_of_gGrid*RANGE_OF_PHI/100;
                y_min = 0;
            }
            else if((*it)->get_coord().second >= column_of_gGrid*RANGE_OF_PHI/100 && (*it)->get_coord().second <= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = row_of_gGrid;
                x_min = row_of_gGrid - 2*row_of_gGrid*RANGE_OF_PHI/100;
                y_max = (*it)->get_coord().second + column_of_gGrid*RANGE_OF_PHI/100;
                y_min = (*it)->get_coord().second - column_of_gGrid*RANGE_OF_PHI/100;
            }
            else if((*it)->get_coord().second >= (column_of_gGrid - column_of_gGrid*RANGE_OF_PHI/100)){
                x_max = row_of_gGrid;
                x_min = row_of_gGrid - 2*row_of_gGrid*RANGE_OF_PHI/100;
                y_max = column_of_gGrid;
                y_min = column_of_gGrid - 2*column_of_gGrid*RANGE_OF_PHI/100;
            }
        }

        for(int i = x_min; i < x_max; i++){
            for(int j = y_min; j < y_max; j++){
                for(int k = 0; k < layer_of_gGrid; k++){
                    if((*it)->get_coord().second != j){
                        phi[(*it)->get_coord().first-1][(*it)->get_coord().second-1] += 
                        demand_manager.demands[i][j][k].getDemand()*GLOBAL_K_CONST*(-((*it)->get_coord().second-j)) /
                         pow((pow(((*it)->get_coord().first-i),2)+pow(((*it)->get_coord().second-j),2)),(3/2));
                    }
                }
            }
        }
    }
    for(auto it = movable_cells.begin(); it != movable_cells.end(); it++){
        phi_y.push_back(phi[(*it)->get_coord().first][(*it)->get_coord().second]);	
    }	
}