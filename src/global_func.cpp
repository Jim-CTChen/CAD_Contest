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
#include <stack>
#include <algorithm>
using namespace std;

// extern const string file_path;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern int layer_of_gGrid;
extern int maxCellMove;
extern int row_of_gGrid;
extern int column_of_gGrid;
extern string file_path;
extern int route_len;
extern int num_routes;
extern string output;
extern vector<string> net;
extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
extern unordered_map <string, Netlist*> netlists;
extern unordered_map <string, Cell*> cells;
extern vector<Cell* > all_cells;
extern vector<Cell* > moved_cells;
extern vector<Cell* > movable_cells;
extern vector<SameGGrid> sameGGrids;
extern vector<AdjHGGrid> adjGGrids;
extern Grid** model;
extern D_Manager demand_manager;


void init() // init 2-D model, 3-D all_demand, extra_supply
{
    // form a 2-D model for Grids
    model = new Grid* [row_of_gGrid];
    for(int i = 0; i < row_of_gGrid; ++i)
    {
        model[i] = new Grid [column_of_gGrid];
    }
    
    int* supply = new int[layer_of_gGrid];
    for(auto &it : layers)  supply[it.second->get_index()-1] = it.second->get_supply();

    // form a 3-D model for demands & set supply
    demand_manager.init()
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
    string temp[5]; // Lay M1 1 H 10
    int pos = 0 ,num = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumLayer"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok); 
            layer_of_gGrid = num;
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
    int temp[4]; // 2 2 1 +3
    int pos = 0 ,num = 0, extra = 0;
    while(getline(file,line)){
        pos = myStrGetTok(line,tok,0,' ');
        if(tok == "NumNonDefaultSupplyGGrid"){
            myStrGetTok(line,tok,pos,' ');
            num = stoi(tok);
            for(int i=0 ;i<num ;i++){
                getline(file,line);
                pos = myStrGetTok(line,tok,0,' ');
                for(int i=0 ;i<3 ;i++){
                    temp[i] = stoi(tok);
                    pos = myStrGetTok(line,tok,pos,' ');      
                }
                myStr2Int(tok, extra);
                demand_manager.demands[temp[0]-1][temp[1]-1][temp[2]-1].setSupply(demand_manager.demands[temp[0]-1][temp[1]-1][temp[2]-1].getSupply() + extra);
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
    string temp[4],tempPin[2],tempBlkg[3]; // MasterCell MC3 3 0
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
    string temp[5]; // adjHGGrid MC1 MC2 M1 1
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
                for(auto &it : layers){
                    if(it.first == temp[3]) str2Int_1 = it.second->get_index();
                }
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
    string temp[6]; // CellInst C1 MC1 1 1 Movable
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
                Cell* new_cell = new Cell(temp[1],temp[2],str2Int_1,str2Int_2,temp[5]);
                cells.insert(pair<string,Cell*>(temp[1], new_cell));
                if(temp[5] == "Movable"){
                    movable_cells.push_back(cells[temp[1]]);
                    cells[temp[1]]->set_index(movable_cells.size()-1);
                }
                model[str2Int_1-1][str2Int_2-1].add_cell(new_cell);
                all_cells.push_back(cells[temp[1]]);
                cells[temp[1]]->set_index(all_cells.size()-1);
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
                net.push_back(temp[1]);
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

bool between_two_st_pts(Steiner_pts* a, Steiner_pts* b, Steiner_pts* x){
    if(x->get_layer() == a->get_layer()  && x->get_layer() == b->get_layer()) {
        if(x->get_coord().first == a->get_coord().first && x->get_coord().first == b->get_coord().first){
            if(x->get_coord().second < a->get_coord().second && x->get_coord().second > b->get_coord().second){
                
                return true;
            }
            else if(x->get_coord().second > a->get_coord().second && x->get_coord().second < b->get_coord().second){
                return true;
            }
        }
        else if(x->get_coord().second == a->get_coord().second && x->get_coord().second == b->get_coord().second){
            
            if(x->get_coord().first < a->get_coord().first && x->get_coord().first > b->get_coord().first){
                return true;
            }
            else if(x->get_coord().first > a->get_coord().first && x->get_coord().first < b->get_coord().first){
                return true;
            }
        }
    }
    else{
        if(x->get_coord().first == a->get_coord().first  && x->get_coord() == b->get_coord()){
            if(x->get_layer() < a->get_layer()  && x->get_layer()  > b->get_layer() ){
                return true;
            }
            else if(x->get_layer() > a->get_layer() && x->get_layer() < b->get_layer() ){
                return true;
            }
        }
    }
    
    
    
    return false;

}

void build_route(vector<pair<Steiner_pts*,Steiner_pts*>> pts_to_pts , Steiner_pts* root){
    vector<Steiner_pts*>::iterator it1, it3;
    Steiner_pts* temp = root ;
    vector<Steiner_pts*> temp1 ,temp2 ,temp3 ;//root in temp1,leaf in temp2,finished root in temp3 
    temp1.push_back(root);
    while(pts_to_pts.size() > 0){
        if(temp1.size() == 0 && temp2.size() == 0){
            for(auto it2 = pts_to_pts.begin(); it2 != pts_to_pts.end(); it2++){
                for(auto it4 = temp3.begin(); it4 != temp3.end()-1; it4++){
                    for(auto it5 = it4+1; it5 != temp3.end(); it5++){
                        if(between_two_st_pts(*it4,*it5,it2->first)){
                            if((*it4)->is_fanout(*it5)){
                                (*it4)->del_fanout(*it5);
                                (*it5)->reset_fanin();
                                (*it4)->set_fanout(it2->first);
                                (it2->first)->set_fanin(*it4);
                                (it2->first)->set_fanout(*it5);
                                (*it5)->set_fanin(it2->first);
                                temp2.push_back(it2->first);
                                goto stop;
                            }
                            else if((*it5)->is_fanout(*it4)){
                                (*it5)->del_fanout(*it4);
                                (*it4)->reset_fanin();
                                (*it5)->set_fanout(it2->first);
                                (it2->first)->set_fanin(*it5);
                                (it2->first)->set_fanout(*it4);
                                (*it4)->set_fanin(it2->first);
                                temp2.push_back(it2->first);                            
                                goto stop;
                            }     
                        }
                        else if(between_two_st_pts(*it4,*it5,it2->second)){
                            if((*it4)->is_fanout(*it5)){
                                (*it4)->del_fanout(*it5);
                                (*it5)->reset_fanin();
                                (*it4)->set_fanout(it2->second);
                                (it2->second)->set_fanin(*it4);
                                (it2->second)->set_fanout(*it5);
                                (*it5)->set_fanin(it2->second);
                                temp2.push_back(it2->second);
                                goto stop;
                            }
                            else if((*it5)->is_fanout(*it4)){
                                (*it5)->del_fanout(*it4);
                                (*it4)->reset_fanin();
                                (*it5)->set_fanout(it2->second);                            
                                (it2->second)->set_fanin(*it5);
                                (it2->second)->set_fanout(*it4);
                                (*it4)->set_fanin(it2->second);
                                temp2.push_back(it2->second);
                                goto stop;
                            } 
                        }
                    }
                } 
                stop: ; 
            }
        }
        for(auto it = temp1.begin(); it != temp1.end(); it++){
            for(auto it2 = pts_to_pts.begin(); it2 != pts_to_pts.end(); it2++){
                if(*(it2->first) == **it){
                    it1 = find (temp3.begin(), temp3.end(), (it2->second));
                    it3 = find (temp2.begin(), temp2.end(), (it2->second));
                    if(it1 != temp3.end() | it3 != temp2.end()){
                        pts_to_pts.erase(it2);
                        if(pts_to_pts.size() == 0){
                            return ;
                        }
                        it2 -= 1;
                    }     
                    else{
                        (*it)->set_fanout(it2->second);
                        (it2->second)->set_fanin(*it);
                        temp2.push_back(it2->second);
                        pts_to_pts.erase(it2);            
                        if(pts_to_pts.size() == 0){
                            return ;
                        }
                        it2 -= 1;
                    }
                }
                else if(*(it2->second) == **it){
                    it1 = find (temp3.begin(), temp3.end(), (it2->first));
                    it3 = find (temp2.begin(), temp2.end(), (it2->first));
                    if(it1 != temp3.end() | it3 != temp2.end()){
                        pts_to_pts.erase(it2);
                        if(pts_to_pts.size() == 0){
                            return ;
                        }
                        it2 -= 1;

                    }
                    else{
                        (*it)->set_fanout(it2->first);
                        (it2->first)->set_fanin(*it);
                        temp2.push_back(it2->first);
                        pts_to_pts.erase(it2);
                        if(pts_to_pts.size() == 0){
                            return ;
                        }
                        it2 -= 1;
                    }  
                }    
            }
        }
        for(auto it = temp1.begin(); it != temp1.end(); it++){ 
            temp3.push_back(*(it));
        }
        temp1.clear();
        for(auto it = temp2.begin(); it != temp2.end(); it++){
            temp1.push_back(*(it));
        }
        temp2.clear();
    }
}

void find_root(vector<pair<Steiner_pts*,Steiner_pts*>> pts_to_pts , string temp_net){
    int k = 0;
    vector<Steiner_pts*>::iterator it2;
    if(pts_to_pts.size() == 1){
        netlists[temp_net]->add_root(*(netlists[temp_net]->get_st_pts().begin()));
        it2 = find (netlists[temp_net]->get_st_pts().begin(), netlists[temp_net]->get_st_pts().end(),(netlists[temp_net]->get_root()));
        netlists[temp_net]->erase_st_pts(it2);
        // for(auto it3 = netlists[temp_net]->get_st_pts().begin(); it3 != netlists[temp_net]->get_st_pts().end(); it3++){
        //     (*it3)->hi();
        // }
        // netlists[temp_net]->get_root()->hi();   
        // cout << "=====================" << endl;
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
                        it2 = find (netlists[temp_net]->get_st_pts().begin(), netlists[temp_net]->get_st_pts().end(),(netlists[temp_net]->get_root())); //cancel root in st_pts
                        netlists[temp_net]->erase_st_pts(it2);
                        // for(auto it3 = netlists[temp_net]->get_st_pts().begin(); it3 != netlists[temp_net]->get_st_pts().end(); it3++){
                        //     (*it3)->hi();
                        // }
                        // netlists[temp_net]->get_root()->hi();   
                        // cout << "=====================" << endl;
                        return ;
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
                        build_route(pts_to_pts,netlists[temp_net]->get_root());
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
                // cout << temp_net <<  " " << n << endl;
                
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
                if(i == num-1){                    //the routing of last net
                    find_root(pts_to_pts,temp_net);
                    build_route(pts_to_pts,netlists[temp[6]]->get_root());
                    pts_to_pts.clear();
                }
            }   
                                  
            break;
        }
    }
};
 

int two_pts_distance(Steiner_pts* a, Steiner_pts* b){
    if(a->get_layer() == b->get_layer()){
        return abs(a->get_coord().first - b->get_coord().first) + abs(a->get_coord().second - b->get_coord().second);
    }
    else{
        return abs(a->get_layer() - b->get_layer());
    }
    return 0;
}

void cal_len(Steiner_pts* s){
    while(true){
        if((s->get_fanout()).size() == 0){  
            return ;
        }
        else{
            for(auto it2 = s->get_fanout().begin(); it2 != s->get_fanout().end(); it2++){
                route_len += two_pts_distance(s,*it2);
                cal_len(*it2);
            }
        }  
        return;
    }
}

void routing_len(){
    Steiner_pts* temp;
    for(auto it = netlists.begin(); it !=netlists.end(); it++){
        if(it->second->get_root() != 0){
            temp = it->second->get_root();
            cal_len(temp);
            // cout << route_len << endl;
        }  
    }
}



void netlistBFS()
{
    for(auto &it : netlists) {
        queue<Steiner_pts*> bfs;
        cout << "Netlist \"" << it.first << "\"" << endl;
        if(it.second->get_root() == 0) {
            int x, y, z;
            it.second->get_pins()[0]->get_coord(x, y, z);
            cout << "(" << x << ", " << y << ", " << z << ")"<< endl;
            cout << "=============" << endl; 
            continue; 
        } 
        bfs.push(it.second->get_root());
        while(!bfs.empty()) {
            cout << *bfs.front() << endl;
            for(auto &it : bfs.front()->get_fanout()){
                bfs.push(it);
            }
            bfs.pop();
        }
        cout << "=============" << endl;
    }
}

void write_output(Steiner_pts* s, string path, string net_name){
    ofstream fout(path.c_str(), ios::out );
    while(true){
        if((s->get_fanout()).size() == 0){  
            return ;
        }
        else{
            for(auto it2 = s->get_fanout().begin(); it2 != s->get_fanout().end(); it2++){
                num_routes += 1;
                output = output + ((s)->print_inf() + " " + (*it2)->print_inf() + " " + net_name + "\n");
                write_output(*it2, path, net_name);
            }
        }  
        return;
    }
}

bool store_output(string path){
    ofstream fout(path.c_str(), ios::out);
    Steiner_pts* temp;

    
    
    for(auto it = net.begin(); it != net.end(); it++){
        for(auto it2 = netlists.begin(); it2 != netlists.end(); it2++){
            if(it2->second->get_root() != 0 && *it == it2->first){
                temp = it2->second->get_root();
                write_output(temp, path, it2->first);
            }
        }
    }
    output = "NumRoutes " + to_string(num_routes) + "\n" + output;

    moved_cells.push_back(cells["C1"]);
    moved_cells.push_back(cells["C2"]);
    for(int i = 0; i < moved_cells.size(); i++){
        output = "CellInst "+moved_cells[moved_cells.size()-1-i]->get_name() + " " + to_string(moved_cells[moved_cells.size()-1-i]->get_coord().first) + " " + to_string(moved_cells[moved_cells.size()-1-i]->get_coord().first) + "\n" +output;
    }
    output = "NumMovedCellInst " + to_string(maxCellMove) + "\n" +output; 
    
    fout << output;
    fout.close();
    return 0;
}
