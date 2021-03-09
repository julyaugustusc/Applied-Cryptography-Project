#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

std::vector<unsigned char> arr2;

int main(){

std::vector<vector<unsigned char> > arr;
std::vector<vector<unsigned char> > arr3;
int count;
std::cout << "The input matrix is";
std::cout<<"\n";

//This is a sample 4*4 matrix

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

//For printing the sample input matrix

 for (int i = 0; i < arr.size(); i++)
 {
     for (int j = 0; j < arr[i].size(); j++)
     {
         cout << arr[i][j];
         std::cout <<  hex << (int) arr[i][j] << " ";
     }
 	std::cout << "\n";
 }

std::cout<<"\n";

std::cout << "The Output matrix is";

//This loop takes each row of the given matrix and rotates(left shift) it as many times as the row number

for (int k=0;k<4;k++){
	count=k;
    arr2.clear();

	for (int l=0;l<4;l++){
		arr2.push_back(arr[k][l]);
	}

	std::rotate(arr2.begin(), arr2.begin()+count, arr2.end());

	arr3.push_back(arr2);

}
std::cout<<"\n";

//For printing the sample output matrix

for (int i = 0; i < arr3.size(); i++)
 {
     for (int j = 0; j < arr3[i].size(); j++)
     {
         cout << arr3[i][j];
         std::cout <<  hex << (int) arr3[i][j] << " ";
     }
 	std::cout << "\n";
 }

return 0;

}


