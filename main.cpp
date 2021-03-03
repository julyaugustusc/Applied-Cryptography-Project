
//AES Algo: m(x) = x^8 + x^4 + x^3 + x + 1 (Irreducible poly)


#include <iostream>
#include <string>         // std::string
#include <bitset>         // std::bitset
#include "AES.h"
using namespace std;

// ALL FOR NO COEFFICIENTS FOR NOW - FINITE FIELD ELEMENTS

// addition is xor the values
std::bitset<8> addition(std::bitset<8> p1, std::bitset<8> p2) {
    std::bitset<8> add = p1 ^ p2;
    return add;
}

/*for every pair, we add the indexes if both values at the indexes are = 1 for the index location 
in the larger, new polynomial. We then FLIP the values (0 to 1 or 1 to 0) in this location. To
explain why, for example, if we have 0011 * 0011 = (x^1 + 1) * (x^1 + 1) = x^2 + x^1 + x^1 + 1. These "additions" 
are actually xors with the exponents cooresponding to the bit index. Thus, this actually will equal
x^2 + 1 because for every 2 they cancel. = (0101) 
PLEASE NOTE THESE RULES
*/
std::bitset<16> mult(std::bitset<8> p1, std::bitset<8> p2) {
    std::bitset<16> mult;
    
    for(int i = 7; i >=0; i--){
        for(int j = 7; j >= 0; j--){
            if(p1[i] == 1 && p2[j] == 1){
                mult[i + j].flip(); 
            }
        }
    }
    
    return mult;
}

//modulo means p1%p2 = answer but in polynomial fashion with these rules as specified above. Again complicated because of bit flipping 
//and because of the different "multiplication" rules. This is very difficult to understand piece by piece.
std::bitset<8> modulo8(std::bitset<16> p1, std::bitset<8> p2){
    //I figured out an easier way to do this a bit later but eh
    std::bitset<16> division;
    std::bitset<8> modulo;
    
    for (int i = 15; i >=0; i--){
        if (p1[i] == 1) {
            int divisionFlip;
            int firstcount = 0;
            for (int j = 7; j >= 0; j--){
                int p1Flip;
                if (p2[j] == 1 && firstcount == 0){
                    firstcount++;
                    divisionFlip = i-j;
                    division.flip(divisionFlip);
                    p1Flip = divisionFlip + j;
                    p1.flip(p1Flip);
                } else if (p2[j] == 1){
                    p1Flip = divisionFlip + j;
                    p1.flip(p1Flip);
                }
                
            }
        }
    }
    for (int i = 7; i >=0; i--){
        modulo[i] = p1[i];
    }
    
    return modulo;
    
}

std::bitset<8> modulo16(std::bitset<16> p1, std::bitset<16> p2){
    //I figured out an easier way to do this a bit later but eh
    std::bitset<16> division;
    std::bitset<8> modulo;
    
    for (int i = 15; i >=0; i--){
        if (p1[i] == 1) {
            int divisionFlip;
            int firstcount = 0;
            for (int j = 7; j >= 0; j--){
                int p1Flip;
                if (p2[j] == 1 && firstcount == 0){
                    firstcount++;
                    divisionFlip = i-j;
                    division.flip(divisionFlip);
                    p1Flip = divisionFlip + j;
                    p1.flip(p1Flip);
                } else if (p2[j] == 1){
                    p1Flip = divisionFlip + j;
                    p1.flip(p1Flip);
                }
                
            }
        }
    }
    
    for (int i = 7; i >=0; i--){
        modulo[i] = p1[i];
    }
    
    return modulo;
    
}


//division instead
std::bitset<16> division(std::bitset<16> p1, std::bitset<8> p2){
    //I figured out an easier way to do this a bit later but eh
    std::bitset<16> division;
    
    for (int i = 15; i >=0; i--){
        if (p1[i] == 1) {
            int divisionFlip;
            int firstcount = 0;
            for (int j = 7; j >= 0; j--){
                int p1Flip;
                if (p2[j] == 1 && firstcount == 0){
                    firstcount++;
                    divisionFlip = i-j;
                    division.flip(divisionFlip);
                    p1Flip = divisionFlip + j;
                    p1.flip(p1Flip);
                } else if (p2[j] == 1){
                    p1Flip = divisionFlip + j;
                    p1.flip(p1Flip);
                }
                
            }
        }
    }
    
    return division;
    
}


void polyStringOut(std::bitset<8> p1) {
    string output = "";
    int numTrue = p1.count();
    int count = 0;
    
    for (int i = 7; i >= 0; i--){
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
    
}

void polyStringOut16(std::bitset<16> p1) {
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
    
}

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
std::bitset<128> addCoef(std::bitset<128> coef1, std::bitset<128> coef2){
    return coef1 ^ coef2;
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

std::bitset<192> multcoef(std::bitset<128> coef1, std::bitset<128> coef2){
    std::bitset<192> result;
    return result;
}






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
    
    std::bitset<128> coef1 = 0x3213135043423145;
    cout << coef1<<endl;
    std::bitset<128> coef2 = 0x34c228b98392a134;
    cout << coef2<<endl;
    
    cout << addCoef(coef1, coef2);
    
    
    
    
    
    

    return 0;
}
