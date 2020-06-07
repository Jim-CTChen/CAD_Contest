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
    ifstream file;
    string line;
    string tok ;
    int pos = 0;
    file.open(file_path);
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "MaxCellMove"){
            pos = myStrGetTok(line,tok,pos,' ');
            maxCellMove = stoi(tok);
        }
    }

    
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
    int str2Int_1 ,str2Int_2 ,str2Int_3;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumMasterCell"){
            myStrGetTok(line,tok,pos,' ');
            myStr2Int(tok, num);
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = 0;
                for(int j=0 ;j<4 ;j++){
                    pos = myStrGetTok(line,tok,pos,' ');
                    temp[j] = tok;     
                }
                myStr2Int(temp[2] , str2Int_1);
                myStr2Int(temp[3] , str2Int_2);
                mastercells[temp[1]] = new MasterCell(temp[1],str2Int_1,str2Int_2);
                for(int j=0 ;j<str2Int_1 ;j++){
                    getline(file,line);
                    pos = 0;
                    for(int k=0 ;k<3 ;k++){
                        pos = myStrGetTok(line,tok,pos,' ');
                        if(k == 1){
                            tempPin[0] = tok;
                        }
                        else if(k == 2){
                            tempPin[1] = tok;
                        }
                    } 
                    mastercells[temp[1]]->set_pin(tempPin[0],tempPin[1],0); // FIXME
                }
                for(int j=0 ;j<str2Int_2 ;j++){
                    getline(file,line);
                    pos = 0;
                    for(int k=0 ;k<4 ;k++){
                        pos = myStrGetTok(line,tok,pos,' ');
                        if(k == 1){
                            tempBlkg[0] = tok;
                        }
                        else if(k == 2){
                            tempBlkg[1] = tok;
                        }
                        else if(k == 3){
                            tempBlkg[2] = tok;
                        }
                        
                    }  
                    myStr2Int(tempBlkg[2] , str2Int_3);
                    mastercells[temp[1]]->set_blockage(tempBlkg[0],tempBlkg[1], str2Int_3);
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
    int str2Int_1 ,str2Int_2;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNeighborCellExtraDemand"){
            myStrGetTok(line,tok,pos,' ');
            myStr2Int(tok, num); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<5 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                myStr2Int(temp[3], str2Int_1);
                myStr2Int(temp[4], str2Int_2);
                if(temp[0] == "sameGGrid"){
                    sameGGrids.push_back(SameGGrid(temp[1],temp[2],str2Int_1,str2Int_2));
                }
                else{
                    adjGGrids.push_back(AdjHGGrid(temp[1],temp[2],str2Int_1,str2Int_2));
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
    int str2Int_1 ,str2Int_2;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumCellInst"){
            myStrGetTok(line,tok,pos,' ');
            myStr2Int(tok, num);
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<6 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                myStr2Int(temp[3], str2Int_1);
                myStr2Int(temp[4], str2Int_2);
                cells.insert(pair<string,Cell*>(temp[1],new Cell(temp[1],temp[2],str2Int_1,str2Int_2,temp[5])));
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
    int str2Int_1 ,str2Int_2;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNets"){
            myStrGetTok(line,tok,pos,' ');
            myStr2Int(tok, num); 
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int j=0 ;j<4 ;j++){
                    temp[j] = tok;  
                    pos = myStrGetTok(line,tok,pos,' ');     
                }
                netlists.insert(pair<string, Netlist*>(temp[1],new Netlist()));
                myStr2Int(temp[2], str2Int_1); 
                for(int j=0 ;j<str2Int_1 ;j++){
                    getline(file,line);
                    pos = myStrGetTok(line,tok,0,' ');
                    pos = myStrGetTok(line,tok,pos+1,'/');
                    tempCell_Pin[0] = tok;
                    pos = myStrGetTok(line,tok,pos+1,' ');
                    tempCell_Pin[1] = tok;
                    for(int i=0 ;i < (cells[tempCell_Pin[0]]->get_pins()).size(); i++){
                        if((cells[tempCell_Pin[0]]->get_pins())[i].get_name() == tempCell_Pin[1]){
                            netlists[temp[1]]->add_pin(&(cells[tempCell_Pin[0]]->get_pins()[i]));
                        }
                    }
                }
            }
            break;
        }
    }

};

void build_route(vector<pair<Steiner_pts*,Steiner_pts*>> pts_to_pts , Steiner_pts* root){
    vector<pair<Steiner_pts*,Steiner_pts*>>::iterator it1;
    Steiner_pts* temp = root;
    int k = 0;
    while(true){
        for(auto it = pts_to_pts.begin(); it != pts_to_pts.end(); it++){
            if(it->first == temp){   
                temp->set_fanout(it->second);
                it->second->set_fanin(temp);
                it1 = find(pts_to_pts.begin(), pts_to_pts.end(),pair<Steiner_pts*,Steiner_pts*>(it->first,it->second)); 
                pts_to_pts.erase(it1);
                if(pts_to_pts.size() == 0){
                    break;
                }
                build_route(pts_to_pts,it->second);
            }
            else if(it->second == temp){
                temp->set_fanout(it->first);
                it->first->set_fanin(temp);
                it1 = find(pts_to_pts.begin(), pts_to_pts.end(),pair<Steiner_pts*,Steiner_pts*>(it->first,it->second)); 
                pts_to_pts.erase(it1);
                if(pts_to_pts.size() == 0){
                    break;
                }
                build_route(pts_to_pts,it->first);
            }
        }
        break;
    }
}

void find_root(vector<pair<Steiner_pts*,Steiner_pts*>> pts_to_pts , string temp_net){
    int k = 0;
    vector<Steiner_pts*>::iterator it2;
    if(pts_to_pts.size() == 1){
        netlists[temp_net]->add_root(*(netlists[temp_net]->get_st_pts().begin()));
        it2 = find (netlists[temp_net]->get_st_pts().begin(), netlists[temp_net]->get_st_pts().end(),(netlists[temp_net]->get_root()));
        netlists[temp_net]->erase_st_pts(it2);
    }
    else if(pts_to_pts.size() == 0){
        k = 0;
    }
    else{
        for(auto it = netlists[temp_net]->get_st_pts().begin(); it != netlists[temp_net]->get_st_pts().end(); it++){
            for(int i = 0; i < netlists[temp_net]->get_pins().size(); i++){
                if((**it).get_layer() == netlists[temp_net]->get_pins()[i]->get_layer() && k == 0){
                    if((**it).get_coord() == netlists[temp_net]->get_pins()[i]->get_cell()->get_coord() && k == 0){
                        netlists[temp_net]->add_root(*it);
                        k = 1;
                        it2 = find (netlists[temp_net]->get_st_pts().begin(), netlists[temp_net]->get_st_pts().end(),(netlists[temp_net]->get_root()));
                        netlists[temp_net]->erase_st_pts(it2);
                    } 
                }   
            }     
        }
    }
    
}

void readRoutes(){
    ifstream file;
    file.open(file_path);
    string line;
    string tok;
    string temp[7],temp_net;//temp_net record the net now
    int pos = 0 ,num = 0 ,k = 0, n = 0; //n is the count of new pts_pts
    vector<string> nets;
    vector<Steiner_pts*>::iterator it;
    vector<string>::iterator it1;
    vector<Steiner_pts*>::iterator it2;
    vector<pair<Steiner_pts*,Steiner_pts*>> pts_to_pts;
    int str2Int_1,str2Int_2,str2Int_3,str2Int_4,str2Int_5,str2Int_6;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumRoutes"){
            myStrGetTok(line,tok,pos,' ');
            myStr2Int(tok, num);  
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<7 ;i++){
                    temp[i] = tok;
                    pos = myStrGetTok(line,tok,pos,' ');      
                }

                it1 = find(nets.begin(),nets.end(),temp[6]);
                if(nets.size() == 0){             
                    nets.push_back(temp[6]);
                    temp_net = temp[6];
                }
                else{
                    if(it1 == nets.end()){   
                        find_root(pts_to_pts,temp_net);
                        cout << netlists[temp_net]->get_st_pts().size() << endl;
                        //build_route(pts_to_pts,netlists[temp_net]->get_root());
                        pts_to_pts.clear();
                        nets.push_back(temp[6]);
                        temp_net = temp[6];
                        n = 0;
                    }
                    else{
                        n += 1;
                    }
                }
                myStr2Int(temp[0], str2Int_1);
                myStr2Int(temp[1], str2Int_2);
                myStr2Int(temp[2], str2Int_3);
                myStr2Int(temp[3], str2Int_4);
                myStr2Int(temp[4], str2Int_5);
                myStr2Int(temp[5], str2Int_6);
                pts_to_pts.push_back(pair<Steiner_pts*,Steiner_pts*>(new Steiner_pts(str2Int_1,str2Int_2,str2Int_3),new Steiner_pts(str2Int_4,str2Int_5,str2Int_6)));
                cout << temp_net << n << endl;
                
                if(netlists[temp_net]->get_st_pts().size() == 0){      //put pts_pts elements in st_pts
                    netlists[temp_net]->add_st_pts(pts_to_pts[n].first);
                    netlists[temp_net]->add_st_pts(pts_to_pts[n].second);
                }
                
                else{
                    k = 0;
                    for(auto it2 = netlists[temp[6]]->get_st_pts().begin(); it2 != netlists[temp[6]]->get_st_pts().end(); it2++){
                        if(**it2 == *(pts_to_pts[n].first)){
                            k = 1;
                            break;
                        } 
                    }
                    if(k == 0){
                        netlists[temp[6]]->add_st_pts(pts_to_pts[n].first); 
                    }
                    k = 0;
                    for(auto it2 = netlists[temp[6]]->get_st_pts().begin(); it2 != netlists[temp[6]]->get_st_pts().end(); it2++){
                        if(**it2 == *(pts_to_pts[n].second)){
                            k = 1;
                            break;
                        } 
                    }
                    if(k == 0){
                        netlists[temp[6]]->add_st_pts(pts_to_pts[n].second); 
                    }
                }
                if(i == num-1){                    //the route of last net
                    find_root(pts_to_pts,temp_net);
                    build_route(pts_to_pts,netlists[temp[6]]->get_root());
                    pts_to_pts.clear();
                }
            }   
                                  
            break;
        }
    }

};
 

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


