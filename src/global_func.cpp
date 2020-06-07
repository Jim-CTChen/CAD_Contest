#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <global_func.h>
#include <base_DS.h>
#include <advance_DS.h>
#include <utility.h>
#include <definition.h>
#include <queue>
#include <algorithm>
using namespace std;

// extern const string file_path;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern int layer_of_gGrid;
extern int maxCellMove;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
extern unordered_map <string, Netlist*> netlists;
extern unordered_map <string, Cell*> cells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;
extern Grid** model;
extern Demand*** all_demand;


void init()
{
    // form a 2-D model for Grids
    model = new Grid* [row_of_gGrid];
    for(int i = 0; i < row_of_gGrid; ++i)
    {
        model[i] = new Grid [column_of_gGrid];
    }

    // form a 3-D model for demands
    all_demand = new Demand** [row_of_gGrid];
    for(int i = 0; i < row_of_gGrid; ++i)
    {
        all_demand[i] = new Demand* [column_of_gGrid];
        for(int j = 0; j < column_of_gGrid; ++j){
            all_demand[i][j] = new Demand [layer_of_gGrid];
        }
    }
}


void clear()
{
    // delete layers & mastercells & netlists & cells
    for(auto& it : layers)      delete it.second; layers.clear();
    for(auto& it : mastercells) delete it.second; mastercells.clear();  
    for(auto& it : netlists)    delete it.second; netlists.clear();
    for(auto& it : cells)       delete it.second; cells.clear();

    // delete model
    for(int i = 0; i < row_of_gGrid;){
        delete[] model[i];
    }
    delete[] model;

    // delete all_demand
    for(int i = 0; i < row_of_gGrid;){
        for(int j = 0; j < column_of_gGrid; ++j){
            delete[] all_demand[i][j];
        }
    }
    delete[] all_demand;
}



void readMaxCellMove(){
    ifstream file(file_path);
    string tempstr;
    int temp;
    file >> tempstr;
    file >> temp;
    maxCellMove = temp;
    file.close();
    
}

void readGGridBoundaryIdx(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    int temp[4];
    int pos = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "GGridBoundaryIdx"){
            for(int i=0 ;i<4 ;i++){
                pos = myStrGetTok(line,tok,pos,' ');
                temp[i] = stoi(tok);
            }
            row_of_gGrid = temp[2];
            column_of_gGrid = temp[3];
            break;
        } 
    }
};

void readLayer(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[5];
    int pos = 0 ,num = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumLayer"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<5 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                layers.insert(pair<string,Layer*>(temp[1],new Layer(temp[1],stoi(temp[2]),temp[3],stoi(temp[4]))));
            }
            break;
        }
    }

};
void readNumNonDefaultSupplyGGrid(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    int temp[4];
    int pos = 0 ,num = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNonDefaultSupplyGGrid"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<4 ;i++){
                    temp[i] = stoi(tok);
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                //model[tmep[2]][temp[0]][temp[1]].demand+=[temp[3]];
            }
            break;
        }
    }
};

void readMasterCell(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[4],tempPin[2],tempBlkg[3];
    int pos = 0 ,num = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumMasterCell"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                for(int j=0 ;j<4 ;j++){
                    pos = myStrGetTok(line,tok,0,' ');
                    temp[i] = stoi(tok);     
                }
                mastercells.insert(pair<string,MasterCell*>(temp[1],new MasterCell(temp[1],stoi(temp[2]),stoi(temp[3]))));
                for(int j=0 ;j<stoi(temp[2]) ;j++){
                    file.getline(line,100);
                    for(int k=0 ;k<3 ;k++){
                        pos = myStrGetTok(line,tok,0,' ');
                        if(k = 1){
                            tempPin[0] = tok;
                        }
                        else if(k = 2){
                            tempPin[1] = tok;
                        }
                        // mastercells[temp[0]]->set_pin(tempPin[0],tempPin[1]); // FIXME
                    }  
                }
                for(int j=0 ;j<stoi(temp[3]) ;j++){
                    file.getline(line,100);
                    for(int k=0 ;k<4 ;k++){
                        pos = myStrGetTok(line,tok,0,' ');
                        if(k = 1){
                            tempBlkg[0] = tok;
                        }
                        else if(k = 2){
                            tempBlkg[1] = tok;
                        }
                        else if(k = 3){
                            tempBlkg[2] = tok;
                        }
                        mastercells[temp[0]]->set_blockage(tempBlkg[1],tempBlkg[2],stoi(tempBlkg[3]));
                    }  
                }
                
            }
            break;
        }
    }
};

void readNeighborCellExtraDemand(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[5];
    int pos = 0 ,num = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNeighborCellExtraDemand"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<5 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                if(temp[0] == "samGGrid"){
                    sameGGrids.push_back(SameGGrid(temp[1],temp[2],stoi(temp[3]),stoi(temp[4])));
                }
                else{
                    adjGGrids.push_back(AdjHGGrid(temp[1],temp[2],stoi(temp[3]),stoi(temp[4])));
                }
            }
            break;
        }
    }

};

void readCellInst(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[6];
    int pos = 0 ,num = 0, layernum = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumCellInst"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<6 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                cells.insert(pair<string,Cell*>(temp[1],new Cell(temp[1],mastercells[temp[2]],stoi(temp[3]),stoi(temp[4]),temp[5])));
            }
            break;
        }
    }
};

void readNets(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[4],tempCell_Pin[2];
    int pos = 0 ,num = 0;
    pair<int, int> tempCoord = {0,0};
    unordered_map<string, MasterCell*>::iterator iter;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNets"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                for(int j=0 ;j<4 ;j++){
                    pos = myStrGetTok(line,tok,0,' ');
                    temp[i] = stoi(tok);     
                }
                netlists.insert(pair<string, Netlist*>(temp[1],new Netlist()));
                for(int j=0 ;j<stoi(temp[2]) ;j++){
                    file.getline(line,100);
                    pos = myStrGetTok(line,tok,0,' ');
                    pos = myStrGetTok(line,tok,pos,'/');
                    tempCell_Pin[0] = tok;
                    pos = myStrGetTok(line,tok,0,' ');
                    tempCell_Pin[1] = tok;
                    // FIXME
                    // tempCoord = cells[tempCell_Pin[0]]->get_coord();
                    //     templayer = cells[tempCell_Pin[0]]->get_mc()->get_pins()->
                    // if(j == 0){  
                    //     netlists[temp[1]]->add_root(Steiner_pts(tempCoord[0],tempCoord[1],templayer));
                    // }
                    // else{
                    //     netlists[temp[1]]->add_pin(Steiner_pts(tempCoord[0],tempCoord[1],templayer));
                    // }
                    
                }
            }
            break;
        }
    }

};

void readRoutes(){
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[6];
    int pos = 0 ,num = 0;
    while(file.getline(line,100)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumRoutes"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                file.getline(line,100);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<6 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                
            }
            break;
        }
    }

};



void countDemand() // routing + blockage + extra demand
{
    // counting routing by iteration for netlist
    for(auto &it : netlists) // for every netlist, count demand of routing
    {
        DEMANDFLAG++;
        queue<Steiner_pts*> q; // for BFS
        Steiner_pts* tmp = 0;
        if(it.second->get_root() == 0) { // in case all pins in the netlist are in the same grid
            tmp = it.second->get_pins()[0]->get_steiner_pts();
            all_demand[tmp->get_coord().first][tmp->get_coord().second][tmp->get_layer()].addDemand(1);
            break;
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


    // counting blockage demand
    for(auto &it : cells) // for every cell
    {
        Cell* tmp = it.second;
        for(auto &iit : tmp->get_mc()->get_blockage()){ // for every blockage in one cell
            all_demand[tmp->get_coord.first][tmp->get_coord.second][iit.get_layer].addDemand(iit.get_extra_demand);
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
                all_demand[i][j][it.get_layer()].addDemand(it.get_extra_demand()*minimum);
                // if(countCurMC[it.get_mc1()] >= countCurMC[it.get_mc2()]){
                //     all_demand[i][j][it.get_layer()].addDemand(it.get_extra_demand()*countCurMC[it.get_mc2()]);
                // }
                // else all_demand[i][j][it.get_layer()].addDemand(it.get_extra_demand()*countCurMC[it.get_mc1()]);
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
                all_demand[i][j][it.get_layer()].addDemand(it.get_extra_demand()*(pairCurPre+pairCurNxt));
            }
        }
    }
}