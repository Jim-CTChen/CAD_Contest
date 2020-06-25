#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int maisdfn(){
    int a = 3;
    string b = "sgdg";
    b = b +to_string(a);
    cout << b;
}
    // vector<Steiner_pts*>::iterator it1, it3;
    // Steiner_pts* temp = root ;
    // vector<Steiner_pts*> temp1 ,temp2 ,temp3 ;//root in temp1,leaf in temp2,finished root in temp3 
    // temp1.push_back(root);
    // cout << temp1.size();
    // while(pts_to_pts.size() != 0){
    //     // pts_to_pts.begin()->first->hi();
    //     // pts_to_pts.begin()->second->hi();
    //     // cout << pts_to_pts.size() << endl ;
        
    //     for(auto it = temp1.begin(); it != temp1.end(); it++){
    //         cout << temp1.size();
    //         for(auto it2 = pts_to_pts.begin(); it2 != pts_to_pts.end(); it2++){
    //             cout << temp2.size();
    //             if(*(it2->first) == **it){
    //                 it1 = find (temp3.begin(), temp3.end(), (it2->second));
    //                 it3 = find (temp2.begin(), temp2.end(), (it2->second));
    //                 if(it1 != temp3.end() | it3 != temp2.end()){
    //                     cout << temp2.size();
    //                     pts_to_pts.erase(it2);
    //                     if(pts_to_pts.size() == 0){
    //                         return ;
    //                     }
    //                     if(it2 != pts_to_pts.end()-1){
    //                         it2 -= 1; 
    //                     } 
    //                 }
    //                 else{
    //                     (*it)->set_fanout(it2->second);
    //                     (it2->second)->set_fanin(*it);
    //                     temp2.push_back(it2->second);
    //                     pts_to_pts.erase(it2);
    //                     if(pts_to_pts.size() == 0){
    //                         return ;
    //                     }
    //                     if(it2 != pts_to_pts.end()-1){
    //                         it2 -= 1; 

    //                     }
    //                 } 
    //             }
    //             else if(*(it2->second) == **it){
    //                 it1 = find (temp3.begin(), temp3.end(), (it2->first));
    //                 it3 = find (temp2.begin(), temp2.end(), (it2->first));
    //                 if(it1 != temp3.end() | it3 != temp2.end()){
    //                     pts_to_pts.erase(it2);
    //                     if(pts_to_pts.size() == 0){
    //                         return ;
    //                     }
    //                     if(it2 != pts_to_pts.end()-1){
    //                         it2 -= 1; 
    //                     } 
    //                     cout << "a";

    //                 }
    //                 else{
    //                     (*it)->set_fanout(it2->first);
    //                     (it2->first)->set_fanin(*it);
    //                     temp2.push_back(it2->first);
    //                     pts_to_pts.erase(it2);
    //                     if(pts_to_pts.size() == 0){
    //                         return ;
    //                     }
    //                     if(it2 != pts_to_pts.end()-1 ){
    //                         it2 -= 1; 
    //                     }
    //                 } 
    //             }
    //         }
    //     }
    //     for(auto it = temp1.begin(); it != temp1.end(); it++){
    //         temp3.push_back(*(it));
    //     }
    //     temp1.clear();
    //     for(auto it = temp2.begin(); it != temp2.end(); it++){
    //         temp1.push_back(*(it));
    //     }
    //     temp2.clear();

    // }

