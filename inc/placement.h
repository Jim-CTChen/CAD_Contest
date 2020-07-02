#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void placement_init();
void countC0();
void solveInitialMatrix_x();
void solveInitialMatrix_y();
void calculate_phi_x(int layer);
void calculate_phi_y(int layer);




#endif // PLACEMENT_H