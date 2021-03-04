
#include <iostream>
#include <string>         // std::string
#include <bitset>         // std::bitset
#include <vector>
#include "AES.h"
using namespace std;
final int Nb = 4;


// 4.1
// addition is xor the values
unsigned char addition(unsigned char p1, unsigned char p2) {
    unsigned char add = p1 ^ p2;
    return add;
}

/*for every pair, we add the indexes if both values at the indexes are = 1 for the index location 
in the larger, new polynomial. We then FLIP the values (0 to 1 or 1 to 0) in this location. To
explain why, for example, if we have 0011 * 0011 = (x^1 + 1) * (x^1 + 1) = x^2 + x^1 + x^1 + 1. These "additions" 
are actually xors with the exponents cooresponding to the bit index. Thus, this actually will equal
x^2 + 1 because for every 2 they cancel. = (0101) 

THEN STILL PART OF THE MULT:

PLEASE NOTE THESE RULES
*/
unsigned char multiply(unsigned char p1, unsigned char p2) {
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
    
    //now modulo m(x) = x^8 + x^4 + x^3 + x + 1 (Irreducible poly)
    //modulo means p1%p2 = answer but in polynomial fashion with these rules as specified above. Again complicated because of bit flipping 
    //and because of the different "multiplication" rules. This is very difficult to understand piece by piece.
    std::bitset<9> modx = 100011011; // PART OF AES STANDARD
    std::bitset<8> division;
    
    for (int i = 15; i >=0; i--){
        if (temp[i] == 1) {
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
    for(int i = 7;i >=0; i++){
        if (temp[i] == 1) {
            result |= (1<<i); //Or-ing the shifted 1 
        }
    }
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

/*Four-term polynomials can be defined - with coefficients that are finite field elements - as:
a(x) = a3 x
3 + a2 x
2 + a1 x + a0 (4.5)
which will be denoted as a word in the form [a0 , a1 , a2 , a3 ]. Note that the polynomials in this
section behave somewhat differently than the polynomials used in the definition of finite field
elements, even though both types of polynomials use the same indeterminate, x. The coefficients
in this section are themselves finite field elements, i.e., bytes, instead of bits; also, the
multiplication of four-term polynomials uses a different reduction polynomial, defined below.
The distinction should always be clear from the context.
To illustrate the addition and multiplication operations, let
b(x) = b x3 + b x2 + b x + b (4.6) 3 2 1 0
define a second four-term polynomial. Addition is performed by adding the finite field
coefficients of like powers of x. This addition corresponds to an XOR operation between the
corresponding bytes in each of the words – in other words, the XOR of the complete word
values.
Thus, using the equations of (4.5) and (4.6),
a(x) + b(x) = (a xor b )x^3 + (a xor b )x^2 + (a xor b )x + (a xor b ) */

//UGH C++ doesn't support functions returning arrays GOTTA RETHINK THIS

//At some point, the paper outlined some version of thinking like it this way:
vector<char> addCoef(vector<char> coef1, vector<char> coef2){
    vector<char> result;
    for (int i = 0; i <= 3; i++){
        result.push_back(addition(coef1[i], coef2[i])); //Most significant NOT first, based on index #
    }
    return result;
}


/*Mult weird rules:
Multiplication is achieved in two steps. In the first step, the polynomial product c(x) = a(x) •
b(x) is algebraically expanded, and like powers are collected to give
6 5 4 3 2
c(x) = c x + c x + c x + c x + c x + c x + c (4.8) 6 5 4 3 2 1 0
where
c 0 0 0 = a • b c 4 3 1 2 2 1 3 = a • b ¯ a • b ¯ a • b
c1 = a1 • b0 ¯ a0 • b1 c5 = a3 • b2 ¯ a2 • b3
c2 = a2 • b0 ¯ a1 • b1 ¯ a0 • b2 c6 = a3 • b3 (4.9)
12
 
c = a • b ¯ a • b ¯ a • b ¯ a • b . 3 3 0 2 1 1 2 0 3
The result, c(x), does not represent a four-byte word. */
//does • then is modulo x^4 + 1
vector<char> modPro(vector<char> coef1, vector<char> coef2){
    vector<char> cVector;
    vector<char> dVector;
    
    //c0 = a0 • b0 (as defined above)
    cVector.push_back(multiply(coef1[0], coef2[0]));
    //c1 = a1 • b0 xor a0 • b1
    cVector.push_back(addCoef(multiply(coef1[1], coef2[0]), multiply(coef1[0], coef2[1])));
    //c2 = a2 • b0 xor a1 • b1 xor a0 • b2
    cVector.push_back(addCoef(addCoef(multiply(coef1[2], coef2[0]), multiply(coef1[1], coef2[1])), multiply(coef1[0], coef2[2])));
    //c3 = a3 • b0 xor a2 • b1 xor a1 • b2 xor a0 • b3 .
    cVector.push_back(addCoef(addCoef(multiply(coef1[2], coef2[0]), multiply(coef1[1], coef2[1])), addCoef(multiply(coef1[0], coef2[2]), multiply(coef1[0], coef2[2]))));
    //c4 = a3 • b1 xor a2 • b2 xor a1 • b3
    cVector.push_back(addCoef(addCoef(multiply(coef1[3], coef2[1]), multiply(coef1[2], coef2[2])), multiply(coef1[1], coef2[3])));
    //c5 = a3 • b2 xor a2 • b3
    cVector.push_back(addCoef(multiply(coef1[3], coef2[2]), multiply(coef1[2], coef2[3])));
    //c6 = a3 • b3
    cVector.push_back(multiply(coef1[3], coef2[3]));
    
    
    //mod (x^4 + 1) results in:
    //c4 xor c0 = d0= (a0 • b0 ) xor (a3 • b1 ) xor (a2 • b2 ) xor (a1 • b3 ) 
    dVector.push_back(addition(cVector[4], cVector[0]));
    //c5 xor c1 = d1 = (a1 • b0 ) xor (a0 • b1 ) xor (a3 • b2 ) xor (a2 • b3 ) 
    dVector.push_back(addition(cVector[5], cVector[1]));
    // c6 xor c2 = d2 =  (a2 • b0 ) xor (a1 • b1 ) xor (a0 • b2 ) xor (a3 • b3 )
    dVector.push_back(addition(cVector[6], cVector[2]));
    // c3 = d3 =  (a3 • b0 ) xor (a2 • b1 ) xor (a1 • b2 ) xor (a0 • b3 )
    dVector.push_back(cVector[3]);
    
    return dVector;
    
}

vector<vector<char>> MixColumns(vector<vector<char>> state){
    vector<char> aVector;
    vector<vector<char>> newState;
    //AES definied value
    aVector.push_back(0x02);
    aVector.push_back(0x01);
    aVector.push_back(0x01);
    aVector.push_back(0x03);
    
    for(int i = 0; i < Nb; i++){ //operates column by column, so column 0 to 3 
        vector<char> columnVector;
        
        columnVector = modPro(aVector, state[i]); //i represents which column, when in this weve bunched it up by columns so the outsider vector is the rows
        newState.push_back(columnVector);
        
    }
    
    return newState;
}

vector<vector<char>> invMixColumns(vector<vector<char>> state){
    vector<char> ainvVector;
    vector<vector<char>> newinvState;
    //AES definied value
    aVector.push_back(0x0e);
    aVector.push_back(0x09);
    aVector.push_back(0x0d);
    aVector.push_back(0x0b);
    
    for(int i = 0; i < Nb; i++){ //operates column by column, so column 0 to 3 
        vector<char> columnVector;
        
        columnVector = modPro(ainvVector, state[i]); //i represents which column, when in this weve bunched it up by columns so the outsider vector is the rows
        newinvState.push_back(columnVector);
        
    }
    
    return newinvState;
}






//Feel free to fix, for right now it's about the functions, they are yet to be tested however
int main()
{
    /* TEST for no COEFFICIENT methods
    
    
    std::bitset<8> poly1 (0x57);
    std::bitset<8> poly2 (0x83);
    
    polyStringOut(poly1);
    polyStringOut(poly2);
    
    cout << addition(poly1, poly2) << endl;
    cout << mult(poly1, poly2) << endl;
    
    polyStringOut(addition(poly1,poly2));
    polyStringOut16(mult(poly1, poly2));
    
    //should get 0
    std::bitset<8> modPoly = modulo8(mult(poly1, poly2), poly1);
    cout << modPoly<< endl;
    
    polyStringOut(modPoly);
    
    //should get poly 2
    std::bitset<16> divisionPoly = division(mult(poly1, poly2), poly1);
    cout << divisionPoly << endl;
    
    polyStringOut16(divisionPoly);
    */
    
    /*std::bitset<128> coef1 = 0x3213135043423145;
    cout << coef1<<endl;
    std::bitset<128> coef2 = 0x34c228b98392a134;
    cout << coef2<<endl;
    
    cout << addCoef(coef1, coef2);
    */
    
    
    
    
    

    return 0;
}
