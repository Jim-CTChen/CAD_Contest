#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <global_func.h>
#include <base_DS.h>
#include <advance_DS.h>
#include <utility.h>
#include <definition.h>
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
    // delete layers & mastercells
    for(auto it = layers.begin(); it!= layers.end(); it++){
        delete it->second;
    }
    for(auto it = mastercells.begin(); it!= mastercells.end(); it++){
        delete it->second;
    }
    
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
    string line;
    string tok;
    int temp[4];
    int pos = 0;
    while(getline(file,line)){
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
    string line;
    string tok;
    string temp[5];
    int pos = 0 ,num = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumLayer"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
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
    string line;
    string tok;
    int temp[4];
    int pos = 0 ,num = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNonDefaultSupplyGGrid"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
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
    string line;
    string tok;
    string temp[4],tempPin[2],tempBlkg[3];
    int pos = 0 ,num = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumMasterCell"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                for(int j=0 ;j<4 ;j++){
                    pos = myStrGetTok(line,tok,0,' ');
                    temp[i] = stoi(tok);     
                }
                mastercells.insert(pair<string,MasterCell*>(temp[1],new MasterCell(temp[1],stoi(temp[2]),stoi(temp[3]))));
                for(int j=0 ;j<stoi(temp[2]) ;j++){
                    getline(file,line);
                    for(int k=0 ;k<3 ;k++){
                        pos = myStrGetTok(line,tok,0,' ');
                        if(k = 1){
                            tempPin[0] = tok;
                        }
                        else if(k = 2){
                            tempPin[1] = tok;
                        }
                        mastercells[temp[0]]->set_pin(tempPin[0],tempPin[1],0); // FIXME
                    }  
                }
                for(int j=0 ;j<stoi(temp[3]) ;j++){
                    getline(file,line);
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
    string line;
    string tok;
    string temp[5];
    int pos = 0 ,num = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNeighborCellExtraDemand"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
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
    string line;
    string tok;
    string temp[6];
    int pos = 0 ,num = 0, layernum = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumCellInst"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<6 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                cells.insert(pair<string,Cell*>(temp[1],new Cell(temp[1],temp[2],stoi(temp[3]),stoi(temp[4]),temp[5])));
            }
            break;
        }
    }
};

void readNets(){
    ifstream file;
    file.open(file_path);
    string line;
    string tok;
    string temp[4],tempCell_Pin[2];
    int pos = 0 ,num = 0 ,templayer = 0;
    pair<int, int> tempCoord = {0,0};
    vector<Pin> temp_pins;
    Pin* temp_Pin;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNets"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                for(int j=0 ;j<4 ;j++){
                    pos = myStrGetTok(line,tok,0,' ');
                    temp[i] = stoi(tok);     
                }
                netlists.insert(pair<string, Netlist*>(temp[1],new Netlist()));
                for(int j=0 ;j<stoi(temp[2]) ;j++){
                    getline(file,line);
                    pos = myStrGetTok(line,tok,0,' ');
                    pos = myStrGetTok(line,tok,pos,'/');
                    tempCell_Pin[0] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');
                    for(int i=0 ;i < (cells[tempCell_Pin[1]]->get_pins()).size(); i++){
                        if((cells[tempCell_Pin[1]]->get_pins())[i].get_name() == tempCell_Pin[1]){
                            netlists[temp[1]]->add_pin(&(cells[tempCell_Pin[1]]->get_pins()[i]));
                        }
                    }
                }
            }
            break;
        }
    }

};

void readRoutes(){
    ifstream file;
    file.open(file_path);
    string line;
    string tok;
    string temp[7];
    int pos = 0 ,num = 0;
    vector<Steiner_pts*>::iterator it;
    vector<pair<Steiner_pts*,Steiner_pts*>> pts_to_pts;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumRoutes"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<7 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                pts_to_pts.push_back(pair<Steiner_pts*,Steiner_pts*>(new Steiner_pts(stoi(temp[0]),stoi(temp[1]),stoi(temp[2])),
                new Steiner_pts(stoi(temp[3]),stoi(temp[4]),stoi(temp[5]))));
                for(auto it = pts_to_pts.begin(); it != pts_to_pts.end(); it++){
                    it = find (netlists[temp[6]]->get_st_pts.begin(), netlists[temp[6]]->get_st_pts.end(), it->first);
                    if(it != netlists[temp[6]]->get_st_pts.end()){
                        netlists[temp[6]]->add_st_pts(it->first);
                    }
                    it = find (netlists[temp[6]]->get_st_pts.begin(), netlists[temp[6]]->get_st_pts.end(), it->second);
                    if(it != netlists[temp[6]]->get_st_pts.end()){
                        netlists[temp[6]]->add_st_pts(it->second);
                    }
                    netlists[temp[6]]->add_st_pts(it->second);
                    for(int i = 0; i < netlists[temp[6]]->get_pins().size(); i++){
                        if((it->first)->get_layer() == netlists[temp[6]]->get_pins()[i]->get_layer()){
                            if((it->first)->get_coord() == netlists[temp[6]]->get_pins()[i]->get_cell()->get_coord()){
                                netlists[temp[6]]->add_root(it->first);
                                break;
                            } 
                        }
                        else if((it->second)->get_layer() == netlists[temp[6]]->get_pins()[i]->get_layer()){
                            if((it->second)->get_coord() == netlists[temp[6]]->get_pins()[i]->get_cell()->get_coord()){
                                netlists[temp[6]]->add_root(it->first);
                                break;
                            }
                        }   
                    }   
                }
                it = find (netlists[temp[6]]->get_st_pts().begin(), netlists[temp[6]]->get_st_pts().end(),(netlists[temp[6]]->get_root()));
                netlists[temp[6]]->erase_st_pts(it);
                build_route(netlists[temp[6]]->get_root());
                pts_to_pts.clear();
            }
            break;
        }
    }

};

void build_route(Steiner_pts* root){
    vector<pair<Steiner_pts*,Steiner_pts*>>::iterator it1;
    Steiner_pts* temp = root;
    while(true){
        for(auto it = pts_to_pts.begin(); it != pts_to_pts.end(); it++){
            if(it->first == temp){
                temp->set_fanout(it->first);
                it->first->set_fanin(temp);
                it1 = find(pts_to_pts.begin(), pts_to_pts.end(),pair<Steiner_pts*,Steiner_pts*>(it->first,it->second)); 
                pts_to_pts.erase(it1);
                build_route(it->first);
            }
            else if(it->second == temp){
                temp->set_fanout(it->second);
                it->second->set_fanin(temp);
                it1 = find(pts_to_pts.begin(), pts_to_pts.end(),pair<Steiner_pts*,Steiner_pts*>(it->first,it->second)); 
                pts_to_pts.erase(it1);
                build_route(it->second);
            }
        }
        break;
    }
}

void add_sameGGrid(string m1, string m2, int l, int ex)
{
    sameGGrids.push_back(SameGGrid(m1, m2, l, ex));
    mastercells[m1]->add_sGGrid(&sameGGrids.back());
    mastercells[m2]->add_sGGrid(&sameGGrids.back());
}

void add_adjhGGrid(string m1, string m2, int l, int ex)
{
    adjGGrids.push_back(AdjHGGrid(m1, m2, l, ex));
    mastercells[m1]->add_aGGrid(&adjGGrids.back());
    mastercells[m2]->add_aGGrid(&adjGGrids.back());
}
