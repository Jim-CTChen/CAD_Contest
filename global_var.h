#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
class Layer;
class MasterCell;
class SameGGrid;
class AdjHGGrid;

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

vector<AdjHGGrid> adjGGrids;
vector<SameGGrid> sameGGrids;





#endif // GLOBAL_VAR_H