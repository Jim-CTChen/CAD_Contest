#include <iostream>
#include <vector>
#include "advance_DS.h"
#include "base_DS.h"

using namespace std;

extern int row_of_gGrid;
extern int column_of_gGrid;
extern vector< vector<float> > cvalues;
extern vector<float> c0values;
extern vector<float> d_x;

void Netlist::B2B_weight_x(){
    int max = pins[0]->get_cell()->get_coord().first , min = pins[0]->get_cell()->get_coord().first ;
    float w = 0;
    Pin* p_r = pins[0];
    Pin* p_l = pins[0];
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it)->get_cell()->get_coord().first > max){
            p_r = *it;
            max = (*it)->get_cell()->get_coord().first;
        }
        else if((*it)->get_cell()->get_coord().first < min){
            p_l = *it;
            min = (*it)->get_cell()->get_coord().first;
        }
    }
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it) != p_r){
            if(p_r->get_cell()->get_coord().first == (*it)->get_cell()->get_coord().first){
                w = 1000;
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_r->get_cell()->get_coord().first-(*it)->get_cell()->get_coord().first));
            }
            if((p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues[p_r->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues[(*it)->get_cell()->get_index()][p_r->get_cell()->get_index()] -= w;
            }
            else if(!(p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_x[(*it)->get_cell()->get_index()] -= w*p_r->get_cell()->get_coord().first;
            }
            else if((p_r->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                d_x[p_r->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().first;
            }
        }

        if((*it) != p_l && (*it) != p_r){
            if(p_l->get_cell()->get_coord().first == (*it)->get_cell()->get_coord().first){
                w = 1000;
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_l->get_cell()->get_coord().first-(*it)->get_cell()->get_coord().first));
            }
            if((p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues[p_l->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues[(*it)->get_cell()->get_index()][p_l->get_cell()->get_index()] -= w;
            }
            else if(!(p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_x[(*it)->get_cell()->get_index()] -= w*p_l->get_cell()->get_coord().first;
            }
            else if((p_l->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                d_x[p_l->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().first;
            } 
        }     
    }
}

void Netlist::B2B_weight_y(){
    int max = pins[0]->get_cell()->get_coord().second , min = pins[0]->get_cell()->get_coord().second  ;
    float w = 0;
    Pin* p_r = pins[0];
    Pin* p_l = pins[0];
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it)->get_cell()->get_coord().second > max){
            p_r = *it;
            max = (*it)->get_cell()->get_coord().second;
        }
        else if((*it)->get_cell()->get_coord().second < min){
            p_l = *it;
            min = (*it)->get_cell()->get_coord().second;
        }
    }
    for(auto it = pins.begin(); it != pins.end(); it++){
        if((*it) != p_r){
            if(p_r->get_cell()->get_coord().second == (*it)->get_cell()->get_coord().second){
                w = 1000;
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_r->get_cell()->get_coord().second-(*it)->get_cell()->get_coord().second));
            }
            if((p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues[p_r->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues[(*it)->get_cell()->get_index()][p_r->get_cell()->get_index()] -= w;
            }
            else if(!(p_r->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_x[(*it)->get_cell()->get_index()] -= w*p_r->get_cell()->get_coord().second;
            }
            else if((p_r->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues[p_r->get_cell()->get_index()][p_r->get_cell()->get_index()] += w;
                d_x[p_r->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().second;
            }
        }

        if((*it) != p_l && (*it) != p_r){
            if(p_l->get_cell()->get_coord().second == (*it)->get_cell()->get_coord().second){
                w = 1000;
            }
            else{
                w = 2.0/((pins.size()-1)*abs(p_l->get_cell()->get_coord().second-(*it)->get_cell()->get_coord().second));
            }
            if((p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                cvalues[p_l->get_cell()->get_index()][(*it)->get_cell()->get_index()] -= w;
                cvalues[(*it)->get_cell()->get_index()][p_l->get_cell()->get_index()] -= w;
            }
            else if(!(p_l->get_cell()->is_movable()) && ((*it)->get_cell()->is_movable())){
                cvalues[(*it)->get_cell()->get_index()][(*it)->get_cell()->get_index()] += w;
                d_x[(*it)->get_cell()->get_index()] -= w*p_l->get_cell()->get_coord().second;
            }
            else if((p_l->get_cell()->is_movable()) && !((*it)->get_cell()->is_movable())){
                cvalues[p_l->get_cell()->get_index()][p_l->get_cell()->get_index()] += w;
                d_x[p_l->get_cell()->get_index()] -= w*(*it)->get_cell()->get_coord().second;
            } 
        }     
    }  
}


