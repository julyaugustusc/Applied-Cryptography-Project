#include <iostream>
#include <vector>

using namespace std;

int main(){

 std::vector<vector<unsigned char> > arr;

 std::cout << "The input matrix is";
 std::cout<<"\n";

 for(int i=0;i<4;i++){
    vector<unsigned char> v;
 	for (int j=0;j<4;j++){
 		if(j==0){
 		v.push_back(0x00);
 		}
 		else{
 		v.push_back(0x01);
 		}
 	}
 	arr.push_back(v);
 }

 std::cout << arr.size() << '\n';

 for (int i = 0; i < arr.size(); i++)
 {
     for (int j = 0; j < arr[i].size(); j++)
     {
         cout << arr[i][j];
         std::cout <<  hex << (int) arr[i][j] << " ";
     }
 	std::cout << "\n";
 }
 
 return 0;
}


