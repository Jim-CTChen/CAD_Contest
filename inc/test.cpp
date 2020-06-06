#include<iostream>

using namespace std;
int main(){
    int a = 10;
    int* b = &a;
    int* c;
    c = &a; 
    cout << &a << endl << b << endl << c;
}