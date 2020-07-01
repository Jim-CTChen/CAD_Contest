#include <iostream>
#include <vector>
#include "advance_DS.h"
#include "base_DS.h"
#include <queue>

using namespace std;

extern int row_of_gGrid;
extern int column_of_gGrid;
extern int layer_of_gGrid;
extern unordered_map <string, Netlist*> netlists;
extern unordered_map <string, Cell*> cells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;
extern Grid** model;
extern vector< vector<float> > cvalues_x;
extern vector< vector<float> > cvalues_y;
extern vector<float> c0values;
extern vector<float> d_x;
extern vector<float> d_y;

const int BIG_CONST = 2000;

using namespace std;

void Netlist::B2B_weight_x(){
    int max = pins[0]->get_cell()->get_coord().first , min = pins[0]->get_cell()->get_coord().first ;
    float w = 0;
    Pin* p_r = pins[0];
    Pin* p_l = pins[0];
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it)->get_cell()->get_coord().first > max){
            p_r = *it;
            max = (*it)->get_cell()->get_coord().first;
        }
        else if((*it)->get_cell()->get_coord().first < min){
            p_l = *it;
            min = (*it)->get_cell()->get_coord().first;
        }
    }
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it) != p_r){
            if(p_r->get_cell()->get_coord().first == (*it)->get_cell()->get_coord().first){
                w = BIG_CONST;
                cout <<  p_r->get_cell()->get_name() << " & " << (*it)->get_cell()->get_name() << ": " << w << endl; // DEBUG
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_r->get_cell()->get_coord().first-(*it)->get_cell()->get_coord().first));
                cout <<  p_r->get_cell()->get_name() << " & " << (*it)->get_cell()->get_name() << ": " << w << endl; // DEBUG
            }
            if((p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_x[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                cvalues_x[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues_x[p_r->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues_x[(*it)->get_cell()->get_index()][p_r->get_cell()->get_index()] -= w;
            }
            else if(!(p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_x[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_x[(*it)->get_cell()->get_index()] -= w*p_r->get_cell()->get_coord().first;
            }
            else if((p_r->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues_x[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                d_x[p_r->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().first;
            }
        }

        if((*it) != p_l && (*it) != p_r){
            if(p_l->get_cell()->get_coord().first == (*it)->get_cell()->get_coord().first){
                w = BIG_CONST;
                cout <<  p_r->get_cell()->get_name() << " & " << (*it)->get_cell()->get_name() << ": " << w << endl; // DEBUG
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_l->get_cell()->get_coord().first-(*it)->get_cell()->get_coord().first));
                cout <<  p_r->get_cell()->get_name() << " & " << (*it)->get_cell()->get_name() << ": " << w << endl; // DEBUG
            }
            if((p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_x[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                cvalues_x[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues_x[p_l->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues_x[(*it)->get_cell()->get_index()][p_l->get_cell()->get_index()] -= w;
            }
            else if(!(p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_x[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_x[(*it)->get_cell()->get_index()] -= w*p_l->get_cell()->get_coord().first;
            }
            else if((p_l->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues_x[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                d_x[p_l->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().first;
            } 
        }
    }
}

void Netlist::B2B_weight_y(){
    int max = pins[0]->get_cell()->get_coord().second , min = pins[0]->get_cell()->get_coord().second  ;
    float w = 0;
    Pin* p_r = pins[0];
    Pin* p_l = pins[0];
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it)->get_cell()->get_coord().second > max){
            p_r = *it;
            max = (*it)->get_cell()->get_coord().second;
        }
        else if((*it)->get_cell()->get_coord().second < min){
            p_l = *it;
            min = (*it)->get_cell()->get_coord().second;
        }
    }
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it) != p_r){
            if(p_r->get_cell()->get_coord().second == (*it)->get_cell()->get_coord().second){
                w = BIG_CONST;
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_r->get_cell()->get_coord().second-(*it)->get_cell()->get_coord().second));
            }
            if((p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_y[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                cvalues_y[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues_y[p_r->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues_y[(*it)->get_cell()->get_index()][p_r->get_cell()->get_index()] -= w;
            }
            else if(!(p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_y[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_y[(*it)->get_cell()->get_index()] -= w*p_r->get_cell()->get_coord().second;
            }
            else if((p_r->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues_y[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                d_y[p_r->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().second;
            }
        }

        if((*it) != p_l && (*it) != p_r){
            if(p_l->get_cell()->get_coord().second == (*it)->get_cell()->get_coord().second){
                w = BIG_CONST;
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_l->get_cell()->get_coord().second-(*it)->get_cell()->get_coord().second));
            }
            if((p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_y[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                cvalues_y[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues_y[p_l->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues_y[(*it)->get_cell()->get_index()][p_l->get_cell()->get_index()] -= w;
            }
            else if(!(p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues_y[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_y[(*it)->get_cell()->get_index()] -= w*p_l->get_cell()->get_coord().second;
            }
            else if((p_l->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues_y[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                d_y[p_l->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().second;
            } 
        }     
    }  
}


D_Manager::~D_Manager(){
    for(int i = 0; i < row_of_gGrid;){
        for(int j = 0; j < column_of_gGrid; ++j){
            delete[] demands[i][j];
        }
    }
    delete[] demands;
}

void D_Manager::init() {
    demands = new Demand** [row_of_gGrid];
    int* supply = new int[layer_of_gGrid];
    for(auto &it : layers)  supply[it.second->get_index()-1] = it.second->get_supply();
    for(int i = 0; i < row_of_gGrid; ++i)
    {
        demands[i] = new Demand* [column_of_gGrid];
        for(int j = 0; j < column_of_gGrid; ++j){
            demands[i][j] = new Demand [layer_of_gGrid];
            for(int k = 0; k < layer_of_gGrid; ++k){
                demands[i][j][k].setSupply(supply[k]);
            }
        }
    }
}

void D_Manager::clearDemand()
{
    for(int k = 0; k < layer_of_gGrid; ++k) {
        cout << "Layer " << k << endl;
        for(int j = 0; j < row_of_gGrid; ++j) {
            for(int i = 0; i < column_of_gGrid; ++i) {
                demands[j][i][k].setDemand(0);
            }
        }
    }
}

void D_Manager::countDemand(bool initial = false)
{
    // counting routing by iteration for netlist
    if(!initial) {
        for(auto &it : netlists) // for every netlist, count demand of routing
        {
            // cerr << "===========================================" << endl; // debug
            // cerr << "Netlist " << it.first << endl; // debug
            DEMANDFLAG++;
            queue<Steiner_pts*> q; // for BFS
            Steiner_pts* tmp = 0;
            if(it.second->get_root() == 0) { // in case all pins in the netlist are in the same grid
                int x, y, z;
                it.second->get_pins()[0]->get_coord(x, y, z);
                demands[x-1][y-1][z-1].addDemand(1);
                // cerr << '\t' << '\t' << "(" << x << ", " << y << ", " << z << ") +1" << endl; // debug
                continue;
            }
            q.push(it.second->get_root());
            while(!q.empty())
            {
                tmp = q.front();
                tmp->addDemand();
                for(auto &iit : tmp->get_fanout()){
                    q.push(iit);
                }
                q.pop();
            }
        }
    }


    // counting blockage demand
    for(auto &it : cells) // for every cell
    {
        Cell* tmp = it.second;
        for(auto &iit : tmp->get_mc()->get_blkgs()){ // for every blockage in one cell
            demands[tmp->get_coord().first-1][tmp->get_coord().second-1][iit.get_layer()-1].addDemand(iit.get_extra_demand());
        }
    }

    // counting extra_demand by iteration for grids
    for (int i = 0; i < row_of_gGrid; ++i)
    {
        for(int j = 0; j < column_of_gGrid; ++j)
        {
            // counting MCs in one grid
            unordered_map<string, int> countPreMC;
            unordered_map<string, int> countCurMC;
            unordered_map<string, int> countNxtMC;
            for(auto& it : model[i][j].get_cells()){
                countCurMC[it->get_mc()->get_name()]++;
            }
            if(j != 0){
                for(auto& it : model[i][j-1].get_cells()){
                    countPreMC[it->get_mc()->get_name()]++;
                }
            }
            if(j != column_of_gGrid-1){
                for(auto& it : model[i][j+1].get_cells()){
                    countNxtMC[it->get_mc()->get_name()]++;
                }
            }

            // calculating sameGGrids
            for(auto& it : sameGGrids){
                int minimum = min(countCurMC[it.get_mc1()], countCurMC[it.get_mc2()]);
                demands[i][j][it.get_layer()-1].addDemand(it.get_extra_demand()*minimum);
            }

            // calculating adjGGrids
            for(auto& it : adjGGrids)
            {
                int pairCurPre = 0;
                int pairCurNxt = 0;
                if(it.get_mc1() == it.get_mc2()){
                    pairCurPre = min(countPreMC[it.get_mc1()],countCurMC[it.get_mc1()]);
                    pairCurNxt = min(countCurMC[it.get_mc1()],countNxtMC[it.get_mc1()]);
                }
                else{
                    pairCurPre = min(countPreMC[it.get_mc1()], countCurMC[it.get_mc2()]) + min(countPreMC[it.get_mc2()], countCurMC[it.get_mc1()]);
                    pairCurNxt = min(countCurMC[it.get_mc1()], countNxtMC[it.get_mc2()]) + min(countCurMC[it.get_mc2()], countNxtMC[it.get_mc1()]);
                }
                demands[i][j][it.get_layer()-1].addDemand(it.get_extra_demand()*(pairCurPre+pairCurNxt));
            }
        }
    }
}

void D_Manager::printDemand()
{
    cout << "demand:" << endl;
    cout << "======================" << endl;
    for(int k = 0; k < layer_of_gGrid; ++k) {
        cout << "Layer " << k << endl;
        for(int j = 0; j < row_of_gGrid; ++j) {
            for(int i = 0; i < column_of_gGrid; ++i) {
                cout << demands[j][i][k] << " ";
            }
            cout << endl;
        }
        cout << "======================" << endl;
    }
}

void D_Manager::printSupply()
{
    cout << "supply:" << endl;
    cout << "======================" << endl;
    for(int k = 0; k < layer_of_gGrid; ++k) {
        cout << "Layer " << k << endl;
        for(int j = 0; j < row_of_gGrid; ++j) {
            for(int i = 0; i < column_of_gGrid; ++i) {
                cout << demands[j][i][k].getSupply() << " ";
            }
            cout << endl;
        }
        cout << "======================" << endl;
    }
}

void D_Manager::printResult()
{
    cout << "result" << endl;
    cout << "======================" << endl;
    for(int k = 0; k < layer_of_gGrid; ++k) {
        cout << "Layer " << k << endl;
        for(int j = 0; j < row_of_gGrid; ++j) {
            for(int i = 0; i < column_of_gGrid; ++i) {
                cout << demands[j][i][k].getSupply()-demands[j][i][k].getDemand() << " ";
            }
            cout << endl;
        }
        cout << "======================" << endl;
    }
}
