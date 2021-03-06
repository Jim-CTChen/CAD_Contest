#ifndef GLOBAL_FUNC_H
#define GLOBAL_FUNC_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Steiner_pts;

void init();
void clear();

// Readin
void readMaxCellMove();
void readGGridBoundaryIdx();
void readLayer();
void readNumNonDefaultSupplyGGrid();
void readMasterCell();
void readNeighborCellExtraDemand();
void readCellInst();
void readNets();
void readRoutes();  //pts_pts照net分開讀 在個別做find_root build_route

void build_route(vector<pair<Steiner_pts*,Steiner_pts*>> , Steiner_pts* ); 
void find_root(vector<pair<Steiner_pts*,Steiner_pts*>>  , string );
bool between_two_st_pts(Steiner_pts* , Steiner_pts* , Steiner_pts* ); //判斷第三點是否在前兩點之間

// Output
void routing_len();
bool store_output(string);
void store_demand_pic(string, int);
void store_cell_pic(string);


void netlistBFS();








#endif // GLOBAL_FUNC_H