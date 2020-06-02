#include <iostream>
#include <unordered_map>
#include <string>
#include <global_func.h>
using namespace std;

extern unordered_map <string, Layer*> layers;  
extern unordered_map <string, MasterCell*> mastercells;
void clear(){
    for(auto it = layers.begin(); it!= layers.end(); it++){
        delete it->second;
    }
    for(auto it = mastercells.begin(); it!= mastercells.end(); it++){
        delete it->second;
    }
}