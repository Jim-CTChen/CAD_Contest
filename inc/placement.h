#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void placement_init();
void countC0();

void calculateCvalue_x();
void calculateCvalue_y();
void choose_movable();
void solveInitialMatrix_x();
void solveInitialMatrix_y();
void calculate_phi_x();
void calculate_phi_y();

void solveGlobalMatrix_x();
void solveGlobalMatrix_y();




#endif // PLACEMENT_H