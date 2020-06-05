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
    ifstream file;
    file.open("../test/case3.txt");
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
