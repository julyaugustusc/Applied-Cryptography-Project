#include <iostream>
#include <vector>

using namespace std;

std::vector<unsigned char> arr2;

// void shiftright(std::vector<unsigned char> arr1, int size){

// unsigned char temp;

// for(int z=0;z<size;z++){
// 	temp=arr1[size-1];
// 	arr1[size-1]=arr1[z];
// 	arr1[z]=temp;
// }

// }

int main(){

vector<vector<unsigned char> > arr;
int count;
std::cout << "The input matrix is";
std::cout<<"\n";

std::vector<unsigned char> cVector={'0','1'};

for(int i=0;i<4;i++){
	for (int j=0;j<4;j++){
		if(j==0){
		arr[i][j].pushback(cVector[0]);
		std::cout<<" "<<arr[i][j]<<" ";
		}
		else{
		arr[i][j].pushback(cVector[1]);
		std::cout<<" "<<arr[i][j]<<" ";
		}
	}
	std::cout<<"\n";

}

// std::cout<<"\n";

// for (int k=0;k<4;k++){
// 	count=k;
// 	for (int l=0;l<4;l++){
// 		arr2[l].pushback(arr[k][l]);
// 	}

// 	std::cout<<"\n";
// 	while (count>0){
// 		shiftright(arr2,4);
// 		count--;
// 	}

// 	for (int l=0;l<4;l++){
// 		arr[k][l].pushback(arr2[l]);
// 		std::cout<<" "<<arr[k][l]<<" ";
// 	}

// }

return 0;

}


