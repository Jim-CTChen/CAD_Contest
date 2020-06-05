#include <advance_DS.h>
#include <base_DS.h>
#include <definition.h>
#include <global_func.h>

int maxCellMove = 0;
int row_of_gGrid = 0;
int column_of_gGrid = 0;
int layer_of_gGrid = 0;

unordered_map <string, Layer*> layers;  

unordered_map <string, MasterCell*> mastercells;

unordered_map <string, Netlist*> netlists;

Grid*** model = 0;

vector<SameGGrid> sameGGrids;
vector<AdjHGGrid> adjGGrids;

using namespace std;
int main()
{
    cout << "hello world!" << endl;
}