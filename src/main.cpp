#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

size_t myStrGetTok(const string& str, string& tok, size_t pos = 0,
            const char del = ' ')
{
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}

int main(){
    vector<int> a={1,2,3};
    vector<int> b={3,4,5};
    a = b;
    for(int i = 0;i < 3;i++){
        cout << a[i];
    }
}
