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

// Global variables
// const string file_path = "../test/case3.txt";
int maxCellMove = 0;
int row_of_gGrid = 0;
int column_of_gGrid = 0;
int layer_of_gGrid = 0;
int8_t DEMANDFLAG = 0;

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

unordered_map <string, Cell*> cells;

Grid** model = 0;
Demand*** all_demand = 0;

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;

using namespace std;
int main()
{
    // cout << "hello world!" << endl;
    ifstream file;
    file.open(file_path);
    char line[100];
    string tok;
    string temp[6];
    int pos = 0 ,num = 0;
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
                cout << temp[0] <<endl;
            }
            break;
        }
    }
}