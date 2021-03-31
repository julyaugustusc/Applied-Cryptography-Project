#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>         // std::bitset
#include <vector>
#include <math.h>       /* pow */
#include <algorithm>
#include <iterator>
using namespace std;

unsigned char addition(unsigned char p1, unsigned char p2);
unsigned char multiply(unsigned char p1, unsigned char p2);
vector<unsigned char> addCoef(vector<unsigned char> coef1, vector<unsigned char> coef2);
vector<unsigned char> modPro(vector<unsigned char> coef1, vector<unsigned char> coef2);
vector<vector<unsigned char> > MixColumns(vector<vector<unsigned char>> state);
vector<vector<unsigned char>> invMixColumns(vector<vector<unsigned char>> state);
vector<unsigned char> subWord(vector<unsigned char> in, vector<vector<unsigned char>> sbox); 
vector<unsigned char> rotWord(vector<unsigned char> in);
vector<vector<unsigned char>> KeyExpansion(vector<vector<unsigned char>> key);
vector<vector<unsigned char>> AddRoundKey(vector<vector<unsigned char>> state, int rround, vector<vector<unsigned char>> KeyExpansion);
vector<vector<unsigned char>> invRoundKey(vector<vector<unsigned char>> state, int rround, vector<vector<unsigned char>> KeyExpansion);
vector<vector<unsigned char>> subBytes(vector<vector<unsigned char>> state, vector<vector<unsigned char>> sbox);
vector<vector<unsigned char>> invSubBytes(vector<vector<unsigned char>>state, vector<vector<unsigned char>> sbox);
vector<vector<unsigned char>> ShiftRows(vector<vector<unsigned char>> state);
vector<vector<unsigned char>> invShiftRows(vector<vector<unsigned char>> state);
vector<vector<unsigned char>> Cipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput);
vector<vector<unsigned char>> invCipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput);
vector<vector<unsigned char>> assignInput(unsigned char s00,unsigned char s10,
unsigned char s20, unsigned char s30,unsigned char s01,unsigned char s11,unsigned char s21,
unsigned char s31,unsigned char s02,unsigned char s12,unsigned char s22,unsigned char s32,
unsigned char s03,unsigned char s13,unsigned char s23,unsigned char s33);
void viewState(vector<vector<unsigned char>> state); //For 4x4 only
void printState(vector<vector<unsigned char>> state);


int Nb = 4;
int Nk = 4;
int Nr = Nb + Nk + 2;

int b = Nb * Nk * 8 ; //input and output blocks it's the number of columns * rows? this is then has 8 bits per block

vector<vector<unsigned char>> sbox = 
    {
        {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
        {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
        {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
        {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
        {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
        {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
        {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
        {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
        {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
        {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
        {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
        {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
        {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
        {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
        {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
        {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }

    };

/*For all of the modes in this recommendation, the plaintext must be represented as a sequence of
bit strings; the requirements on the lengths of the bit strings vary according to the mode: 
*/

/*For the ECB and CBC modes, the total number of bits in the plaintext must be a multiple of the
block size, b; in other words, for some positive integer n, the total number of bits in the plaintext
must be nb. The plaintext consists of a sequence of n bit strings, each with bit length b. The bit
strings in the sequence are called data blocks, and the plaintext is denoted P1, P2,…, P .*/

// addition is xor the bit values of unsigned char
unsigned char addition(unsigned char p1, unsigned char p2) {
    unsigned char add = p1 ^ p2;
    return add;
}

vector<unsigned char> addCoef(vector<unsigned char> coef1, vector<unsigned char> coef2){
    vector<unsigned char> result;
    for (int i = 0; i < Nk; i++){
        result.push_back(addition(coef1[i], coef2[i])); //Most significant NOT first, based on index #, this is different
    }
    return result;
}

vector<unsigned char> addCoef2(vector<unsigned char> coef1, vector<unsigned char> coef2){
    vector<unsigned char> result;
    for (int i = 0; i < Nk; i++){
        result.push_back(addition(coef1[i], coef2[i])); //Most significant NOT first, based on index #, this is different
    }
    return result;
}

vector<vector<vector<unsigned char>>> inputTextToBlocks(string input){
    unsigned char inputChar;
    vector<unsigned char> columns;
    vector<vector<unsigned char>> states;
    vector<vector<vector<unsigned char>>> inputTextStateBlocks;
    
    int numberOfBlocks = (input.size() / b) + 1; 
    int countForCol = 0;
    int countForState = 0;
    
    for(int i = 0; i < numberOfBlocks * b; i++){
        if(!input[i]){ // this might not work? might have to be == NULL
            inputChar = 0x00;
        } else {
            inputChar = input[i];
        }
        
        columns.push_back(inputChar);
        countForCol++;
        
        if(countForCol == Nb){
            states.push_back(columns);
            columns.clear();
            countForCol = 0;
            countForState++;
        }
        
        if(countForState == Nk){
            inputTextStateBlocks.push_back(states);
            states.clear();
            countForState = 0;
        }
        
    }
    
}



vector<vector<vector<unsigned char>>> CBCmodeEncrypt (vector<vector<vector<unsigned char>>> inputBlocks, vector<vector<unsigned char>> IV, vector<vector<unsigned char>> key){
    // ssuming that we've transformed the IV into a similar sized "state"
    //the input text would also be transformed to a vector of states. So it can go one by one as many times as needed.
    vector<vector<unsigned char>> tempBlock;
    vector<vector<vector<unsigned char>>> outputBlocks;
    
    vector<vector<unsigned char>> outCipher;
    
    for(int i = 0; i < IV.size(); i++){
        tempBlock.push_back(addCoef(IV[i],inputBlocks[0][i]));
    }
    
    outCipher = Cipher(tempBlock, key);
    
    outputBlocks.push_back(outCipher);
    
    for(int i = 1; i<inputBlocks.size(); i++){
        tempBlock.clear();
        
        for(int j = 0; j < IV.size(); j++){ // or outCipher.size()
            tempBlock.push_back(addCoef(outCipher[j],inputBlocks[i][j]));
        }
        
        outCipher.clear();
        outCipher = Cipher(tempBlock, key);
        
        outputBlocks.push_back(outCipher);
        
    }
    
    return outputBlocks;
}

vector<vector<vector<unsigned char>>> CBCmodeDecrypt (vector<vector<vector<unsigned char>>> cipherText, vector<vector<unsigned char>> IV, vector<vector<unsigned char>> key){
    // ssuming that we've transformed the IV into a similar sized "state"
    //the input text would also be transformed to a vector of states. So it can go one by one as many times as needed.
    vector<vector<unsigned char>> tempBlock;
    vector<vector<vector<unsigned char>>> plainText;
    
    vector<vector<unsigned char>> outInvCipher = invCipher(cipherText[0], key);
   
    for(int i = 0; i < IV.size(); i++){
        tempBlock.push_back(addCoef(IV[i], outInvCipher[i]));
    }
    
    plainText.push_back(tempBlock);
    
    for(int i = 1; i<cipherText.size(); i++){
        tempBlock.clear();
        
        outInvCipher = invCipher(cipherText[i], key);
        
        for(int j = 0; j < IV.size(); j++){
            tempBlock.push_back(addCoef(outInvCipher[j], cipherText[i-1][j]));
        }
        
        plainText.push_back(tempBlock);
        
    }
    
    return plainText;
}
    
unsigned char multiply(unsigned char p1, unsigned char p2) {
    //cout<< "p1: ";
    //binaryPrint(p1);
    //cout<< "p2: ";
    //binaryPrint(p2);
    std::bitset<16> temp; 
    
    for(int i = 7; i >=0; i--){
        for(int j = 7; j >= 0; j--){
            //using a mask to see if individual bits are set
            if((((p1 >> (i)) & 1) == 1) && (((p2 >> (j)) & 1) == 1)) {
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

//COEFFICIENTS NOW
//Please note that these are fundamentally different polynomial representations. The understanding is that the words 
//themselves, represented before as unsigned char/ 8-bit are the coefficents themselves as used here. 

//EACH char is the coefficent of the 4-term polynomial. 
//Auggie has not tested this yet
/*vector<unsigned char> addCoef(vector<unsigned char> coef1, vector<unsigned char> coef2){
    vector<unsigned char> result;
    for (int i = 0; i < Nk; i++){
        result.push_back(addition(coef1[i], coef2[i])); //Most significant NOT first, based on index #, this is different
    }
    return result;
}*/


/*MODULAR PRODUCT (coefficent polys) Still (4.3)
Multiplication (MODULAR PRODUCT) is achieved in two steps. In the first step, the polynomial product c(x) = a(x) •
b(x) is algebraically expanded*/
//does • then is modulo x^4 + 1

//I DON'T THINK THIS WILL WORK FOR OTHER Nk NOT EQUAL TO 4 YET
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

vector<vector<unsigned char> > MixColumns(vector<vector<unsigned char>> state){
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

vector<unsigned char> subWord(vector<unsigned char> in, vector<vector<unsigned char>> sbox)
{
    for(int i = 0; i < in.size(); i++)
    {
        unsigned char x = (in[i] & 0xf0) >> 4;
        unsigned char y = in[i] & 0x0f;
        in[i] = sbox[x][y];
    }
    return in;
}

vector<unsigned char> rotWord(vector<unsigned char> in)
{
    char temp;
    temp = in[0];
    for(int i = 0; i < in.size()-1; i++)
    { 
        in[i] = in[i+1]; 
    }
    in[in.size()-1] = temp;
    return in;
}

//Note: this does NOT return a 4x4 state but Nb*(Nr+1) words (4 chars that are a vector)
//However it does TAKE the cipher as a state
//Key is the opposite demensions as the state - will always have 4 in a column, but variable amount of columns because of matrix multiplication
vector<vector<unsigned char>> KeyExpansion(vector<vector<unsigned char>> key){
    //number of words: [Nb*(Nr+1)]

    vector<vector<unsigned char>> wordList;
    vector<unsigned char> temp;
    vector<unsigned char> word;
    vector<unsigned char> Rcon;
    
    
    int i = 0;
    while (i < Nk){
        word = key[i];
        wordList.push_back(word);
        i++;
    }

    i = Nk; 

    while (i < Nb * (Nr + 1)){ 
        //cout << "i = " << i << endl;
        temp = wordList[i-1];
        //cout << "temp: " << hex << (int)temp[0] << (int)temp[1] << (int)temp[2] << (int)temp[3] << endl;

        if(i % Nk == 0){
            Rcon.clear();
            int tempInt = pow(2, (i/Nk - 1));
            if (i == 0x24){ // Sorry I had to hard - code these, I don't know where these numbers are coming from.
                tempInt = 0x1b;
            } else if (i == 0x28){
                tempInt = 0x36;
            }
            unsigned char firstChar = (char)(tempInt);
            Rcon.push_back(firstChar);

            for(int j = 1; j < 4; j++){
                Rcon.push_back(0x00);
            }
            //cout << "rotWord: " << hex << (int)rotWord(temp)[0] << (int)rotWord(temp)[1] << (int)rotWord(temp)[2] << (int)rotWord(temp)[3] << endl;
            //cout << "subWord: " << hex << (int)subWord(rotWord(temp), sbox)[0] << (int)subWord(rotWord(temp), sbox)[1] << (int)subWord(rotWord(temp), sbox)[2] << (int)subWord(rotWord(temp), sbox)[3] << endl;
            //cout << "Rcon: " << hex << (int)Rcon[0] << (int)Rcon[1] << (int)Rcon[2] << (int)Rcon[3] << endl;

            temp = addCoef(subWord(rotWord(temp), sbox), Rcon);
            //cout << hex << (int)temp[0] << (int)temp[1] << (int)temp[2] << (int)temp[3] << endl;

        } else if ((Nk>6) && (i % Nk == 4)){
            temp = subWord(temp, sbox);
        }

        wordList.push_back(addCoef(wordList[i-Nk],temp));
        i++;
        
    }
    return wordList;
}

vector<vector<unsigned char>> AddRoundKey(vector<vector<unsigned char>> state, int rround, vector<vector<unsigned char>> KeyExpansion){
    
    int l = rround * Nb;
    for(int i = 0; i < Nb; i++){
        state[i] = addCoef(state[i], KeyExpansion[l + i]);
    }
    return state;
}

vector<vector<unsigned char>> invAddRoundKey(vector<vector<unsigned char>> state, int rround, vector<vector<unsigned char>> KeyExpansion){
    return AddRoundKey(state, rround, KeyExpansion);
}

vector<vector<unsigned char>> subBytes(vector<vector<unsigned char>> state, vector<vector<unsigned char>> sbox) 
{

    for(int i = 0; i < state.size(); i++)
    {
        for(int j = 0; j < state.size(); j++)
        {
            unsigned char entry = state[i][j];
            unsigned char y = entry & 0x0f;
            unsigned char x = (entry & 0xf0)>>4;
            state[i][j] = sbox[x][y];
            //cout << "entry is " << hex << (int) entry << " x is " << hex << (int) x << " y is " << hex << (int) y << " box is " << hex << (int) sbox[x][y] << '\n';
        }
    }

    return state;
}

vector<vector<unsigned char>> invSubBytes(vector<vector<unsigned char>> state, vector<vector<unsigned char>> sbox)
{
    //for each item in the state, find the row/column combination to find the original byte.
    unsigned char xvalue = 0x00;
    unsigned char yvalue = 0x00;
    for(int i = 0; i < state.size(); i++)
    {
        for(int j = 0; j < state.size(); j++)
        {
            unsigned char entry = state[i][j];
            for(int k = 0; k < sbox.size(); k++)
            {
                for(int l = 0; l < sbox.size(); l++)
                {
                    if(sbox[k][l] == state[i][j])
                    {
                        xvalue = k;
                        yvalue = l;
                        //cout << "x is " << hex << (int) xvalue << " y is " << hex << (int) yvalue << "\n";
                        xvalue = xvalue << 4;
                        entry = xvalue | yvalue;
                    }
                    
                }
            }
            state[i][j] = entry;
        }
    }

    return state;
}

vector<vector<unsigned char>> ShiftRows(vector<vector<unsigned char>> state){
    vector<vector<unsigned char>> resultingState;
    vector<unsigned char> v;
    
    for (int k=0;k<4;k++){
	    
        v.clear();
        

	    for (int l=0;l<4;l++){
	        
		    v.push_back(state[(k+l) % Nb][l]);
		    //cout << hex << (int)v[l] << endl;
		    
	    }

	    //std::rotate(v.begin(), v.begin()+count, v.end());

	    resultingState.push_back(v);

    }
    return resultingState;
}

vector<vector<unsigned char>> invShiftRows(vector<vector<unsigned char>> state){
    vector<vector<unsigned char>> resultingState;
    vector<unsigned char> v; 
    
    for (int k=0;k<4;k++){
	    int count=k;
        v.clear();

	    for (int l=0;l<4;l++){
	    	v.push_back(state[(k-l+Nb) % Nb][l]);
	    }

	    //std::rotate(v.begin(), v.begin()+v.size()-count, v.end());

	    resultingState.push_back(v);

    }
    
    return resultingState;
}



vector<vector<unsigned char>> Cipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput){
    
    vector<vector<unsigned char>> roundKeyVec = KeyExpansion(cipherInput);
    
    state = AddRoundKey(state, 0, roundKeyVec);
    //viewState(state);
    
    for(int i = 1; i < Nr ; i++){
        state = subBytes(state, sbox);
        //viewState(state);
        state = ShiftRows(state);
        //viewState(state);
        state = MixColumns(state);
        //viewState(state);
        state = AddRoundKey(state, i, roundKeyVec);
        //viewState(state);
    }
    
    state = subBytes(state, sbox);
    //viewState(state);
    state = ShiftRows(state);
    //viewState(state);
    state = AddRoundKey(state, Nr, roundKeyVec);
    //viewState(state);
    
    return state;
    
    
} 
    
vector<vector<unsigned char>> invCipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput){
    
    vector<vector<unsigned char>> roundKeyVec = KeyExpansion(cipherInput);
    
    state = invAddRoundKey(state, Nr, roundKeyVec);
    //viewState(state);
    
    //cout << Nr;
    for(int i = Nr-1; i >= 1; i--){
        //cout << "I'm started";
        state = invShiftRows(state);
        //viewState(state);
        state = invSubBytes(state, sbox);
        //viewState(state);
        state = invAddRoundKey(state, i, roundKeyVec);
        //viewState(state);
        state = invMixColumns(state);
        //viewState(state);
    }
    
    state = invShiftRows(state);
    //viewState(state);
    state = invSubBytes(state, sbox);
    //viewState(state);
    state = invAddRoundKey(state, 0, roundKeyVec);
    //viewState(state);
    
    return state;
 
} 

vector<vector<unsigned char>> assignInput(unsigned char s00,unsigned char s10,
unsigned char s20, unsigned char s30,unsigned char s01,unsigned char s11,unsigned char s21,
unsigned char s31,unsigned char s02,unsigned char s12,unsigned char s22,unsigned char s32,
unsigned char s03,unsigned char s13,unsigned char s23,unsigned char s33){
    vector<unsigned char> columnVector;
    vector<vector<unsigned char> > resultingState;
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

void printState(vector<vector<unsigned char>> state)
{
    for(int i = 0; i < state.size(); i++)
        {
            for(int j = 0; j < state.size(); j++)
            {
                cout << hex << (int) state[i][j] << ' ';
            }
            cout << "\n";
        }

        cout << "\n";
}

vector<vector<unsigned char>> CFBmodeEncrypt (double s, vector<vector<unsigned char>> plainText, vector<vector<unsigned char>> IV, vector<vector<unsigned char>> key){
    //plainText can be any number of bits from 1 - 128. This presents some challenges. The inner vector represents those 
    //1-128 bit values in 8 bit unsigned characters, most significant bit as the first unsigned char in the leftmost place,
    //just as in a state. Only this is a 1-dimentional vector and not a 2D vector for concatination/simplification of code purposes. The 2D comes in 
    //with multiple plainTexts like 1, 2, 3 as represented in the diagram
    vector<vector<unsigned char>> tempBlock;
    vector<vector<unsigned char>> outputBlockCipher;
    vector<vector<unsigned char>> cipherTextBlock;
    vector<unsigned char> cipherText;
    int remainderInt = (int)s % 8;
    unsigned char tempChar;
    unsigned char tempChar2;
    //cout << "Here1"<< endl;
    //cout << ceil(s/8) << endl;
    
    for(int i = 0; i < IV.size(); i++){
        tempBlock.push_back(IV[i]);
    }
    viewState(IV);
    
    outputBlockCipher = Cipher(tempBlock, key);
    viewState(outputBlockCipher);
    
    for(int i = 0; i < ceil(s/8); i++){ // ceil might not work here, guess we will find out - it didn't changed to double.
        if(remainderInt != 0 && i == ceil(s/8)-1){
            //note the 8-remainderInt zeros on the end are NOT part of this, they are least significant, but I am limited
            tempChar = outputBlockCipher[i/Nk][i%Nb];
            //cout << hex << (int)tempChar << endl;
            tempChar = tempChar >> (8-remainderInt);
            //cout << hex << (int)tempChar << endl;
            tempChar2 = plainText[0][i];
            //cout << hex << (int)tempChar2 << endl;
            tempChar2 = tempChar2;
            //cout << hex << (int)tempChar2 << endl;
            tempChar ^= tempChar2;
            //cout << hex << (int)tempChar << endl;
            tempChar = tempChar << (8-remainderInt);
            //cout << hex << (int)tempChar << endl;
            cipherText.push_back(tempChar);
        }else{
            cipherText.push_back(plainText[0][i] ^ outputBlockCipher[i/Nk][i%Nb]);
            //cout << hex << (int)(plainText[0][i] ^ outputBlockCipher[i/Nk][i%Nb]) << endl;
        }
    }
    //cout << "Here2"<< endl;
    
    cout << "tempChar: ";
    cout << hex << (int)tempChar << endl; // for 1 this maybe 8 digits long
    cipherTextBlock.push_back(cipherText);
    cipherText.clear();
    
    for(int i = 1; i < plainText.size(); i++){
        tempBlock.clear();
        //IV shift left s bits
        bitset<128> IVbits;
        bitset<128> cipherTextBits; // won't always have 128, just a space holder
        bitset<128> tempIVholdBits;
        cout << "Here3"<< endl;
        //viewState(IV);
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++){
                //will always be 128 bits
                tempIVholdBits = IV[j][k];
                //cout << "tempIVholdBits" << tempIVholdBits << endl;
                IVbits |= (tempIVholdBits << (((3-j)*32)+((3-k)*8)));
                //cout << "IVbits " << IVbits << endl;
            }
        }
        
        IVbits = IVbits << s;
        //cout << "IVbits shift" << IVbits << endl;
        //concat with cipherTextBlock
        cout << "Here4"<< endl;
        for(int j = 0; j < ceil(s/8); j++){
            if(remainderInt != 0 && j == ceil(s/8)-1){
                //note the 8-remainderInt zeros on the end are NOT part of this, they are least significant, but I am limited
                cout << "Here6"<< endl;
                cout << "i: " << i << "j: " << j << endl;
                cout << "cipherTextBlock[i-1][j]:" << hex << (int)cipherTextBlock[i-1][j] << endl; // PROBLEM RIGHT HERE
                cipherTextBits |= (cipherTextBlock[i-1][j] >> (8-remainderInt));
            }else{
                cipherTextBits |= (cipherTextBlock[i-1][j] << ((((int)s/8)-j)*8+remainderInt));
            }   
        }
        
        IVbits |= cipherTextBits;
        cout << "IVbits OR" << IVbits << endl;
        bitset<128> tempMask = 0b11111111;
        bitset<128> loadBits;
        
        //cout << "Here5"<< endl;
        //back to vector<unsigned char> form
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++){
                //cout << "j" << j << " k" << k << endl;
                //will always be 128 bits // takes care of anything not 8 divisible
                loadBits = ((IVbits >> (((3-j)*32)+((3-k)*8))) & tempMask);
                //cout << "IVbits" << IVbits << endl;
                //cout << "loadBits" << loadBits <<endl;
                
                unsigned char loadChar = 00000000;
                for(int z = 0; z < 8; z++){
                    //cout << "loadBits[z]" << loadBits[z] <<endl;
                    loadChar |= loadBits[z] << z;
                    //cout << "loadChar"; 
                    //cout << hex << loadChar <<endl;
                    
                }
                IV[j][k] = loadChar;
                //IV[j][k] = (IVbits >> (((3-j)*32)+((3-k)*8))) & 0b11111111;
            }
        }
        cout << "inputblock: " << endl;
        viewState(IV);
        
        for(int j = 0; j < IV.size(); j++){
            tempBlock.push_back(IV[j]);
        }
        
        outputBlockCipher = Cipher(tempBlock, key);
        cout << "outputblock: " << endl;
        viewState(outputBlockCipher);
        
        for(int j = 0; j < ceil(s/8); j++){ // ceil might not work here, guess we will find out
            if(remainderInt != 0 && j == ceil(s/8)-1){
                //note the 8-remainderInt zeros on the end are NOT part of this, they are least significant, but I am limited
                tempChar = outputBlockCipher[j/Nk][j%Nb];
                cout << "tempChar:" << hex << (int)tempChar << endl;
                tempChar = tempChar >> (8-remainderInt);
                cout << "tempChar:" << hex << (int)tempChar << endl;
                tempChar2 = plainText[i][j];
                cout << "tempChar2:" << hex << (int)tempChar2 << endl;
                tempChar2 = tempChar2; // originally thought shifting was necessary, but maybe not? this is bc the 1 isn't in the most significant bit but already shifted
                cout << "tempChar2:" << hex << (int)tempChar2 << endl;
                tempChar ^= tempChar2;
                cout << "tempChar:" << hex << (int)tempChar << endl;
                tempChar = tempChar << (8-remainderInt);
                cout << "tempChar:" << hex << (int)tempChar << endl;
                cipherText.push_back(tempChar);
                
                //cipherText.push_back((outputBlockCipher[j/Nk][j%Nb] >> (8-remainderInt)) ^ (plainText[i][j] >> (8-remainderInt))) <<(8-remainderInt)
            }else{
                cipherText.push_back(plainText[i][j] ^ outputBlockCipher[j/Nk][j%Nb]);
            }
        }
        
        cout << "tempChar: ";
        cout << hex << (int)tempChar << endl; // for 1 this maybe 8 digits long
        
        cipherTextBlock.push_back(cipherText);
        cipherText.clear();
    }
    
    
    
    viewState(cipherTextBlock);
    

    return cipherTextBlock;
}

/*vector<vector<vector<unsigned char>>> CFBmodeDecrypt (vector<vector<vector<unsigned char>>> cipherText, vector<vector<unsigned char>> IV, vector<vector<unsigned char>> key){
    // ssuming that we've transformed the IV into a similar sized "state"
    //the input text would also be transformed to a vector of states. So it can go one by one as many times as needed.
    vector<vector<unsigned char>> tempBlock;
    vector<vector<vector<unsigned char>>> plainText;
    
    vector<vector<unsigned char>> outInvCipher;
   
    for(int i = 0; i < IV.size(); i++){
        tempBlock.push_back(IV[i]);
    }
    outInvCipher = invCipher(tempBlock, key);
    
    plainText.push_back(addCoef(outInvCipher, cipherText[0]));
    
    tempBlock.clear();
    
    for(int i = 1; i<cipherText.size(); i++){
        outInvCipher = invCipher(cipherText[i-1], key);
        
        plainText.push_back(addCoef(outInvCipher, cipherText[i]));
        
    }
    
    return plainText;
}*/
    
    vector<vector<vector<unsigned char>>> ECBmodeEncrypt (vector<vector<vector<unsigned char>>> inputBlocks,vector<vector<unsigned char>> key){
    vector<vector<vector<unsigned char>>> outputBlocks;
     vector<vector<unsigned char>> outCipher;
    
    for(int i = 0; i<inputBlocks.size(); i++){
        outCipher = Cipher(inputBlocks[i],key);
        
        outputBlocks.push_back(outCipher);
        
    }
    
    return outputBlocks;
}

vector<vector<vector<unsigned char>>> ECBmodeDecrypt (vector<vector<vector<unsigned char>>> cipherText,vector<vector<unsigned char>> key){
    vector<vector<vector<unsigned char>>> plainText;
    vector<vector<unsigned char>> tempBlock;
    vector<vector<unsigned char>> outInvCipher;
    
    //plainText.push_back(tempBlock);
    
    for(int i = 0; i<cipherText.size(); i++){
        
        outInvCipher = invCipher(cipherText[i],key);
        
        plainText.push_back(outInvCipher);
        
    }
    
    return plainText;
}
    
vector<vector<vector<unsigned char>>> CTRmodeEncrypt (vector<vector<unsigned char>> counter1,vector<vector<vector<unsigned char>>> inputBlocks,vector<vector<unsigned char>> key){
    vector<vector<vector<unsigned char>>> outputBlocks;
    vector<vector<vector<unsigned char>>> output;
     vector<vector<unsigned char>> outCipher;
     vector<vector<unsigned char>> tempBlock;
     int j=3;
     int k=3;
     int l;
     int m;
    for(int i = 0; i<4; i++){
        outCipher = Cipher(counter1,key);
        outputBlocks.push_back(outCipher); 
    
    //alternative to incrementing counter
    // if(counter1[3][3]==0xff){
    //     counter1[3][3]=0x00;
    //     counter1[3][2]++;
    // } else {counter1[3][3]++;
    // }
    
    if(counter1[j][k]==0xff) {
        l=j;
        m=k;
        while(j>0 and counter1[j][k]==0xff){
            while (k>=0 and counter1[j][k]==0xff){
                counter1[j][k]=0x00;
                if (k==0 and !counter1[j-1][k]==0xff){
                    counter1[j][k+3]++;
                    break;
                }
                if (counter1[j][k-1]==0xff){
                    continue;
                }
                else{
                    counter1[j][k-1]++;
                    break;
                }
                k--;
            }
            j--;
            k=3;
        }
        j=l;
        k=m;
    }
    else{
        counter1[3][3]++;
        }

    }
    
    for(int j = 0; j < inputBlocks.size(); j++){
        for(int i = 0; i < inputBlocks.size(); i++){
            tempBlock.push_back(addCoef2(inputBlocks[j][i],outputBlocks[j][i]));
        }
        output.push_back(tempBlock);
        tempBlock.clear();
    }
    return output;
}

vector<vector<vector<unsigned char>>> CTRmodeDecrypt (vector<vector<unsigned char>> counter1,vector<vector<vector<unsigned char>>> cipherText,vector<vector<unsigned char>> key){
    vector<vector<vector<unsigned char>>> plainText;
    vector<vector<vector<unsigned char>>> outText;
    vector<vector<unsigned char>> tempBlock;
    vector<vector<unsigned char>> outInvCipher;
     int j=3;
     int k=3;
     int l;
     int m;
    
    //plainText.push_back(tempBlock);
    
    for(int i = 0; i<4; i++){
        
        outInvCipher = Cipher(counter1,key);
        
        plainText.push_back(outInvCipher);
        
    if(counter1[3][3]==0xff){
        counter1[3][3]=0x00;
        counter1[3][2]++;
    } else {counter1[3][3]++;
    }
        
    // if(counter1[j][k]==0xff) {
    //     l=j;
    //     m=k;
    //     while(j>0 and counter1[j][k]==0xff){
    //         while (k>=0 and counter1[j][k]==0xff){
    //             counter1[j][k]=0x00;
    //             if (k==0 and !counter1[j-1][k]==0xff){
    //                 counter1[j][k+3]++;
    //                 break;
    //             }
    //             if (counter1[j][k-1]==0xff){
    //                 continue;
    //             }
    //             else{
    //                 counter1[j][k-1]++;
    //                 break;
    //             }
    //             k--;
    //         }
    //         j--;
    //         k=3;
    //     }
    //     j=l;
    //     k=m;
    // }
    // else{
    //     counter1[3][3]++;
    //     }

    }    
    for(int j = 0; j < cipherText.size(); j++){
        for(int i = 0; i < cipherText.size(); i++){
            tempBlock.push_back(addCoef2(plainText[j][i],cipherText[j][i]));
        }
        outText.push_back(tempBlock);
        tempBlock.clear();
    }
    
    return outText;
}
    

  
int main()
{
   
    /*
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
    */
    
    //MixColumns() test - will also be testing the modPro function
    //See Appendix B for first MixColumns before and after, before is previous at ShiftRows
    
    //I don't know the best way to assign space numbers I wrote a function above
    //vector<vector<unsigned char>> state = assignInput(0xd4,0xbf,0x5d,0x30,0xe0,0xb4,0x52,0xae,0xb8,0x41,0x11,0xf1,0x1e,0x27,0x98,0xe5);
    
    //vector<vector<unsigned char>> input = assignInput(0x95,0x65,0xfd,0xf3,0x90,0xfb,0xb1,0x92,0x89,0x67,0xa6,0x70,0xc3,0xc9,0x6e,0xff);
    
    
    /*viewState(input);
    
    vector<vector<unsigned char>> output = MixColumns(input);
    
    viewState(output);
    
    vector<vector<unsigned char>> reverse = invMixColumns(output);
    viewState(reverse);*/
    /*vector<vector<unsigned char>> state = assignInput(0x32, 0x43, 0xf6, 0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34);
    vector<vector<unsigned char>>  input = assignInput(0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c);
    viewState(input);
    vector<vector<unsigned char>> output = AddRoundKey(state, 0, KeyExpansion(input));
    viewState(output);*/
    
    //Cipher testing
    
    /*vector<vector<unsigned char>> state = assignInput(0x32, 0x43, 0xf6, 0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34);
    vector<vector<unsigned char>> input = assignInput(0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c);
    
    viewState(state);
    viewState(input);
    
    vector<vector<unsigned char>> output = Cipher(state,input);
    vector<vector<unsigned char>> invCipherOut = invCipher(output,input);
    viewState(invCipherOut);*/
    
    //MODE testing
    vector<vector<unsigned char>> key = assignInput(0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c);
    vector<vector<unsigned char>> IV = assignInput(0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f);
    vector<vector<vector<unsigned char>>> plainText = {{{0x6b, 0xc1, 0xbe, 0xe2}, {0x2e, 0x40, 0x9f, 0x96}, {0xe9, 0x3d, 0x7e, 0x11}, {0x73, 0x93, 0x17, 0x2a}}, 
        {{0xae, 0x2d, 0x8a, 0x57}, {0x1e, 0x03, 0xac, 0x9c}, {0x9e, 0xb7, 0x6f, 0xac}, {0x45, 0xaf, 0x8e, 0x51}}, {{0x30, 0xc8, 0x1c, 0x46}, {0xa3, 0x5c, 0xe4, 0x11}, 
        {0xe5, 0xfb, 0xc1, 0x19}, {0x1a, 0x0a, 0x52, 0xef}}, {{0xf6, 0x9f, 0x24, 0x45}, {0xdf, 0x4f, 0x9b, 0x17}, {0xad, 0x2b, 0x41, 0x7b}, {0xe6, 0x6c, 0x37, 0x10}}};
    vector<vector<unsigned char>> plainText1 = {{0b00}, {0b01}, {0b01}, {0b00}, {0b01}, {0b00}, {0b01}, {0b01}, {0b01}, {0b01}, {0b00}, {0b00}, {0b00}, {0b00}, {0b00}, {0b01}};
    vector<vector<unsigned char>> plainText2 = {{0xf0, 0xf1, 0xf2, 0xf3}, {0xf4, 0xf5, 0xf6, 0xf7}, {0xf8, 0xf9, 0xfa, 0xfb}, {0xfc, 0xfd, 0xfe, 0xff}};
    vector<vector<vector<unsigned char>>> cipherText1 = {{{0x87, 0x4d, 0x61, 0x91}, {0xb6, 0x20, 0xe3, 0x26}, {0x1b, 0xef, 0x68, 0x64}, {0x99, 0x0d, 0xb6, 0xce}}, 
        {{0x98, 0x06, 0xf6, 0x6b}, {0x79, 0x70, 0xfd, 0xff}, {0x86, 0x17, 0x18, 0x7b}, {0xb9, 0xff, 0xfd, 0xff}}, {{0x5a, 0xe4, 0xdf, 0x3e}, {0xdb, 0xd5, 0xd3, 0x5e}, 
        {0x5b, 0x4f, 0x09, 0x02}, {0x0d, 0xb0, 0x3e, 0xab}}, {{0x1e, 0x03, 0x1d, 0xda}, {0x2f, 0xbe, 0x03, 0xd1}, {0x79, 0x21, 0x70, 0xa0}, {0xf3, 0x00, 0x9c, 0xee}}};
   
    /*vector<vector<vector<unsigned char>>> encrypt = CBCmodeEncrypt(plainText, IV, key);
    
    viewState(encrypt[0]);
    viewState(encrypt[1]);
    viewState(encrypt[2]);
    viewState(encrypt[3]);
    
    vector<vector<vector<unsigned char>>> decrypt = CBCmodeDecrypt(encrypt, IV, key);
    
    viewState(decrypt[0]);
    viewState(decrypt[1]);
    viewState(decrypt[2]);
    viewState(decrypt[3]);*/
    
    // vector<vector<unsigned char>> encrypt1 = CFBmodeEncrypt(8, plainText1, IV, key);
    
    // for(int i = 0; i < encrypt1.size(); i++){
    //     cout << (bitset<8>)(int)encrypt1[i][0] << endl;
    // }
    
    //vector<vector<vector<unsigned char>>> encrypt = ECBmodeEncrypt(plainText,key);
    
    //viewState(encrypt[0]);
    // viewState(encrypt[1]);
    // viewState(encrypt[2]);
    // viewState(encrypt[3]);
    
    //vector<vector<vector<unsigned char>>> decrypt = ECBmodeDecrypt(encrypt, key);
    
    //viewState(decrypt[0]);
    // viewState(decrypt[1]);
    // viewState(decrypt[2]);
    // viewState(decrypt[3]);
    
    // vector<vector<vector<unsigned char>>> encrypt = CTRmodeEncrypt(plainText2,plainText,key);
    // viewState(encrypt[0]);
    // viewState(encrypt[1]);
    // viewState(encrypt[2]);
    // viewState(encrypt[3]);

    vector<vector<vector<unsigned char>>> decrypt = CTRmodeDecrypt(plainText2,cipherText1,key);
    viewState(decrypt[0]);
    viewState(decrypt[1]);
    viewState(decrypt[2]);
    viewState(decrypt[3]);


    return 0;
}





