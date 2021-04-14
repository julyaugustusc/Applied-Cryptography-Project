/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
using namespace std;

vector<vector<unsigned char>> insert128Hex(string hexInput){
    //should look something like: 6bc1bee22e409f96e93d7e117393172a 
    //128/4 = 32 char
    string validChars = "0123456789abcdef";
    vector<vector<unsigned char>> outputVector;
    vector<unsigned char> columnVec;
    string tempHex = "zz"; // should only be 2 letters long
    unsigned int tempChar; // this is weird but it must be int instead of char for stringstream
    std::stringstream ss;
    
    //Validate input first
    try {
        if(hexInput.size()!= 32){
            throw 3;
        }
        
        for(int i = 0; i < 32; i++){
            
            if(validChars.find(hexInput[i]) == -1){
                throw 1;
            }
            
            else {
                if(i % 2 == 0){
                    tempHex[0] = hexInput[i];
                    cout << tempHex[0] << endl;
                }
                
                else {
                    tempHex[1] = hexInput[i]; 
                    cout << tempHex << endl;
                    
                    ss << tempHex;
                    ss >> hex >> tempChar;
                    cout << hex << tempChar << endl;
                    columnVec.push_back(tempChar);
                    ss.clear(); //ss will not be reset by "<< tempHex"
                    
                    if(columnVec.size() == 4){ //4 is how many items in a column
                        outputVector.push_back(columnVec);
                        columnVec.clear();
                    }
                }
            }
            
        }
        
        if (outputVector.size() != 4){
            cout << outputVector.size() << endl;
            throw 2;
        } 
        
        else {
            return outputVector;
        }
        
    } catch (int num) {
        cout << "Invalid char Input " << num << endl;
    }
    
}

void viewState(vector<vector<unsigned char>> state){
    //ONLY SUPPORTS 4x4 AT THE MOMENT
    vector<unsigned char> columnVector1 = state[0];
    vector<unsigned char> columnVector2 = state[1];
    vector<unsigned char> columnVector3 = state[2];
    vector<unsigned char> columnVector4 = state[3];
    
    cout << "State:" << endl;
    
    //this is difficult due to the fact it functionally goes by columns and not rows, so we can't just print columns.
    vector<unsigned char>::iterator it1 = columnVector1.begin();
    vector<unsigned char>::iterator it2 = columnVector2.begin();
    vector<unsigned char>::iterator it3 = columnVector3.begin();
    vector<unsigned char>::iterator it4 = columnVector4.begin();
    
    for (int i = 0; i < 4; i++){ 
        
        printf("%02x ", (unsigned int)(unsigned char)*it1);
        printf("%02x ", (unsigned int)(unsigned char)*it2);
        printf("%02x ", (unsigned int)(unsigned char)*it3);
        printf("%02x \n", (unsigned int)(unsigned char)*it4);
        
        it1++;
        it2++;
        it3++;
        it4++;
    }

    cout << '\n';
}

int main()
{
    viewState(insert128Hex("6bc1bee22e409f96e93d7e117393172a"));

    return 0;
}
