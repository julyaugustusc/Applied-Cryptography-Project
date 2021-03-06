
#include <iostream>
#include <string>         // std::string
#include <bitset>         // std::bitset
#include <vector>
#include "AES.h"
using namespace std;
int Nb = 4;

void binaryPrint(unsigned char toBinary){
    printf("%02x \n", (unsigned int)(unsigned char)toBinary);
}

// 4.1
// addition is xor the bit values of unsigned char
unsigned char addition(unsigned char p1, unsigned char p2) {
    unsigned char add = p1 ^ p2;
    return add;
}

/*for every pair, we add the indexes if both values at the indexes are = 1 for the index location 
in the larger, new polynomial. We then FLIP the values (0 to 1 or 1 to 0) in this location. To
explain why, for example, if we have 0011 * 0011 = (x^1 + 1) * (x^1 + 1) = x^2 + x^1 + x^1 + 1. These "additions" 
are actually xors with the exponents cooresponding to the bit index. Thus, this actually will equal
x^2 + 1 because for every 2 they cancel themselves out. = (0101) 

THEN STILL PART OF THE MULT:
We then have to mod it so that it returns an 8-bit (unsigned char). The mod is a set value with 
m(x) = x^8 + x^4 + x^3 + x + 1, or 0b100011011. This is what is deemed a "Irreducible poly" much like a prime, 
it's only factors are itself and 1.

PLEASE NOTE THESE RULES THEY ARE VERY IMPORTANT TO UNDERSTAND
*/ 
//THIS IS THE EQUIVALENT OF WHEN YOU SEE "•" in the Publication
unsigned char multiply(unsigned char p1, unsigned char p2) {
    //cout<< "p1: ";
    //binaryPrint(p1);
    //cout<< "p2: ";
    //binaryPrint(p2);
    std::bitset<16> temp; 
    
    for(int i = 7; i >=0; i--){
        for(int j = 7; j >= 0; j--){
            //using a mask to see if individual bits are set
            if(((p1 >> (i)) & 1 == 1) && ((p2 >> (j)) & 1 == 1)) {
                //because it is NOT the size of a char. Output will be though.
                temp[i + j].flip(); 
            }
            
        }
    }
    
    //MOD
    //modulo means p1%p2 = answer but in polynomial fashion with these rules as specified above. Again complicated because of bit flipping 
    //and because of the different "multiplication" rules. This is very difficult to understand piece by piece.
    std::bitset<9> modx = 0b100011011; // PART OF AES STANDARD
    std::bitset<8> division;
    
    for (int i = 15; i >=0; i--){
        if (i<8){
            
        } else if (temp[i] == 1) {
            int divisionFlip;
            int firstcount = 0;
            
            for (int j = 8; j >= 0; j--){
                int tempFlip;
                
                if (modx[j] == 1 && firstcount == 0){
                    firstcount++;
                    divisionFlip = i-j;
                    division.flip(divisionFlip);
                    tempFlip = divisionFlip + j;
                    temp.flip(tempFlip);
                    
                } else if (modx[j] == 1){
                    tempFlip = divisionFlip + j;
                    temp.flip(tempFlip);
                }
            }
        }
    }
    
    //temp should end up with the remaining polynomial, now convert to unsigned char
    unsigned char result = 0b00000000;
    for(int i = 7;i >=0; i--){ //should always have 8 bits only (all others bits in temp i>8 WILL be zero)
        if (temp[i] == 1) {
            result |= (1<<i); //Or-ing the shifted 1 
        }
    }
    //cout<< "answer: ";
    //binaryPrint(result);
    return result;
}

void polyStringOut(unsigned char p1) {
    int numTrue = 0;
    for (int i = 0; i < 8; i++){
        if ((p1 >> (i)) & 1 == 1){
            numTrue++;
        }
    }
    int count = 0;
    
    for (int i = 7; i >= 0; i--){
        if ((p1 >> (i)) & 1 == 1){
            count++;
            if(i == 0){
                cout << 1 << endl;
            } else if (count == numTrue) {
                cout << "x^" ;
                cout << i << endl;
            } else {
                cout << "x^";
                cout << i;
                cout << " + ";
            }
        } 
    }
}

//If it comes in handy might as well leave it here:
/* void polyStringOut16(std::bitset<16> p1) {
    string output = "";
    int numTrue = p1.count();
    int count = 0;
    
    for (int i = 15; i >= 0; i--){
        if (p1[i] == 1){
            count++;
            if(i == 0){
                cout << 1 << endl;
            } else if (count == numTrue) {
                cout << "x^" ;
                cout << i << endl;
            } else {
                cout << "x^";
                cout << i;
                cout << " + ";
            }
        } 
    }
} */


//COEFFICIENTS NOW
//Please note that these are fundamentally different polynomial representations. The understanding is that the words 
//themselves, represented before as unsigned char/ 8-bit are the coefficents themselves as used here. 

//EACH char is the coefficent of the 4-term polynomial. 
//Auggie has not tested this yet
vector<unsigned char> addCoef(vector<unsigned char> coef1, vector<unsigned char> coef2){
    vector<unsigned char> result;
    for (int i = 0; i <= 3; i++){
        result.push_back(addition(coef1[i], coef2[i])); //Most significant NOT first, based on index #, this is different
    }
    return result;
}


/*MODULAR PRODUCT (coefficent polys) Still (4.3)
Multiplication (MODULAR PRODUCT) is achieved in two steps. In the first step, the polynomial product c(x) = a(x) •
b(x) is algebraically expanded*/
//does • then is modulo x^4 + 1
vector<unsigned char> modPro(vector<unsigned char> coef1, vector<unsigned char> coef2){
    vector<unsigned char> cVector;
    vector<unsigned char> dVector;
    
    //All is straight from the Publication
    //c0 = a0 • b0 (as defined above)
    //cVector.push_back(multiply(coef1[0], coef2[0]));
    //c1 = a1 • b0 xor a0 • b1
    //cVector.push_back(addition(multiply(coef1[1], coef2[0]), multiply(coef1[0], coef2[1])));
    //c2 = a2 • b0 xor a1 • b1 xor a0 • b2
    //cVector.push_back(addition(addition(multiply(coef1[2], coef2[0]), multiply(coef1[1], coef2[1])), multiply(coef1[0], coef2[2])));
    //c3 = a3 • b0 xor a2 • b1 xor a1 • b2 xor a0 • b3 .
    //cVector.push_back(addition(addition(multiply(coef1[2], coef2[0]), multiply(coef1[1], coef2[1])), addition(multiply(coef1[0], coef2[2]), multiply(coef1[0], coef2[2]))));
    //c4 = a3 • b1 xor a2 • b2 xor a1 • b3
    //cVector.push_back(addition(addition(multiply(coef1[3], coef2[1]), multiply(coef1[2], coef2[2])), multiply(coef1[1], coef2[3])));
    //c5 = a3 • b2 xor a2 • b3
    //cVector.push_back(addition(multiply(coef1[3], coef2[2]), multiply(coef1[2], coef2[3])));
    //c6 = a3 • b3
    //cVector.push_back(multiply(coef1[3], coef2[3]));
    
    
    
    //mod (x^4 + 1) results in:
    //c4 xor c0 = d0= (a0 • b0 ) xor (a3 • b1 ) xor (a2 • b2 ) xor (a1 • b3 ) 
    unsigned char s0a = multiply(coef1[0], coef2[0]);
    unsigned char s0b = multiply(coef1[3], coef2[1]);
    unsigned char s0c = multiply(coef1[2], coef2[2]);
    unsigned char s0d = multiply(coef1[1], coef2[3]);
    
    
    dVector.push_back(addition(addition(s0a, s0b), addition(s0c, s0d)));
    //binaryPrint(dVector[0]);
    //c5 xor c1 = d1 = (a1 • b0 ) xor (a0 • b1 ) xor (a3 • b2 ) xor (a2 • b3 ) 
    dVector.push_back(addition(addition(multiply(coef1[1], coef2[0]), multiply(coef1[0], coef2[1])), addition(multiply(coef1[3], coef2[2]), multiply(coef1[2], coef2[3]))));
    //binaryPrint(dVector[1]);
    //dVector.push_back(addition(cVector[5], cVector[1]));
    // c6 xor c2 = d2 =  (a2 • b0 ) xor (a1 • b1 ) xor (a0 • b2 ) xor (a3 • b3 )
    dVector.push_back(addition(addition(multiply(coef1[2], coef2[0]), multiply(coef1[1], coef2[1])), addition(multiply(coef1[0], coef2[2]), multiply(coef1[3], coef2[3]))));
    //binaryPrint(dVector[2]);
    //dVector.push_back(addition(cVector[6], cVector[2]));
    // c3 = d3 =  (a3 • b0 ) xor (a2 • b1 ) xor (a1 • b2 ) xor (a0 • b3 )
    dVector.push_back(addition(addition(multiply(coef1[3], coef2[0]), multiply(coef1[2], coef2[1])), addition(multiply(coef1[1], coef2[2]), multiply(coef1[0], coef2[3]))));
    //binaryPrint(dVector[3]);
    //dVector.push_back(cVector[3]);
    
    return dVector;
    
}

//Vector of a vector because char represents the word themselves, then the vector<char> represents the each column block,
//vector<vector<char>> is the "array" of all those columns put together. Number of Columns can change (because Nb), 
//but number of words in a column cannot
//The State is <vector<vector<char>>
vector<vector<unsigned char>> MixColumns(vector<vector<unsigned char>> state){
    vector<unsigned char> aVector;
    vector<vector<unsigned char>> newState;
    
    //AES definied value (equation 4.14) a(x) = {03}x^3 + {01}x^2 + {01}x +{02}
    aVector.push_back(0x02);
    aVector.push_back(0x01);
    aVector.push_back(0x01);
    aVector.push_back(0x03);
    
    for(int i = 0; i < Nb; i++){ //operates column by column, so column 0 to 3 
        vector<unsigned char> columnVector;
        
        columnVector = modPro(aVector, state[i]); //i represents which column, when in this weve bunched it up by columns so the outsider vector is the rows
        newState.push_back(columnVector);
        
    }
    
    return newState;
}

vector<vector<unsigned char>> invMixColumns(vector<vector<unsigned char>> state){
    vector<unsigned char> ainvVector;
    vector<vector<unsigned char>> newinvState;
    
    //AES definied value (equation 4.15) a^-1(x) = {0b}x^3 + {0d}x^2 + {09}x +{0e}
    ainvVector.push_back(0x0e); //least significant pushed first to match index value
    ainvVector.push_back(0x09);
    ainvVector.push_back(0x0d);
    ainvVector.push_back(0x0b);
    
    //i represents which column, when in this we've bunched it up by columns so the outsider vector is the array of those put together
    for(int i = 0; i < Nb; i++){ //operates column by column, so column 0 to Nb-1
        vector<unsigned char> columnVector;
        
        columnVector = modPro(ainvVector, state[i]); 
        newinvState.push_back(columnVector);
        
    }
    
    return newinvState;
}

vector<vector<unsigned char>> assignInput(unsigned char s00,unsigned char s10, 
unsigned char s20, unsigned char s30,unsigned char s01,unsigned char s11,unsigned char s21,
unsigned char s31,unsigned char s02,unsigned char s12,unsigned char s22,unsigned char s32,
unsigned char s03,unsigned char s13,unsigned char s23,unsigned char s33){
    vector<unsigned char> columnVector;
    vector<vector<unsigned char>> resultingState;
    columnVector.push_back(s00); columnVector.push_back(s10); columnVector.push_back(s20);
    columnVector.push_back(s30);
    resultingState.push_back(columnVector);
    columnVector.clear();
    columnVector.push_back(s01); columnVector.push_back(s11); columnVector.push_back(s21);
    columnVector.push_back(s31);
    resultingState.push_back(columnVector);
    columnVector.clear();
    columnVector.push_back(s02); columnVector.push_back(s12); columnVector.push_back(s22);
    columnVector.push_back(s32);
    resultingState.push_back(columnVector);
    columnVector.clear();
    columnVector.push_back(s03); columnVector.push_back(s13); columnVector.push_back(s23);
    columnVector.push_back(s33);
    resultingState.push_back(columnVector);
    columnVector.clear();
    
    return resultingState;
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





//Feel free to fix, for right now it's about the functions, they are yet to be tested however
int main()
{
   
    
    //Addition (4.1) test as expressed in the AES Publication
    unsigned char poly1 = 0b01010111;
    polyStringOut(poly1);
    unsigned char poly2 = 0b10000011;
    polyStringOut(poly2);
    
    unsigned char addAnswer = addition(poly1, poly2);
    std::bitset<8> binaryAddAnswer;
    for(int i = 7; i >= 0; i--){
        binaryAddAnswer[i] = ((addAnswer >> i) & 1);
    }
    polyStringOut(addAnswer);
    cout << binaryAddAnswer << endl;
    cout << "Should be: 11010100" << endl;
    
    //Multiplication (4.2) test as expressed in the AES Publication
    unsigned char multAnswer = multiply(poly1, poly2);
    std::bitset<8> binaryMultAnswer;
    for(int i = 7; i >= 0; i--){
        binaryMultAnswer[i] = ((multAnswer >> i) & 1);
    }
    polyStringOut(multAnswer);
    cout << binaryMultAnswer << endl;
    cout << "Should be: 11000001" << endl;
    
    
    //MixColumns() test - will also be testing the modPro function
    //See Appendix B for first MixColumns before and after, before is previous at ShiftRows
    
    //I don't know the best way to assign space numbers I wrote a function above
    vector<vector<unsigned char>> input = assignInput(0xd4,0xbf,0x5d,0x30,0xe0,0xb4,0x52,0xae,0xb8,0x41,0x11,0xf1,0x1e,0x27,0x98,0xe5);
    
    //vector<vector<unsigned char>> input = assignInput(0x95,0x65,0xfd,0xf3,0x90,0xfb,0xb1,0x92,0x89,0x67,0xa6,0x70,0xc3,0xc9,0x6e,0xff);
    
    
    viewState(input);
    
    vector<vector<unsigned char>> output = MixColumns(input);
    
    viewState(output);
    
    vector<vector<unsigned char>> reverse = invMixColumns(output);

    viewState(reverse);
    
    
    
    
    
    

    return 0;
}
