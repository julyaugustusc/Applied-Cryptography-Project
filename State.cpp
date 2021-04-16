#include "State.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>         // std::bitset
#include <vector>
#include <math.h>       /* pow */
#include <algorithm>
#include <iterator>
#include <sstream>
using namespace std;

//Takes a HexString of 32char
vector<vector<unsigned char>> State::insert128Hex(string hexInput){
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
                    ss.str("");
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
        cout << "Invalid Input For the state" << num << endl;
    }
}

//this might not work yet ???
string State::hexOutput(vector<vector<unsigned char>> vectInput){
	string output = "";
	string tempString = "";
	checkValidState();
	stringstream ss;
	unsigned int tempChar;

	for (int i = 0; i < 4;i++){
		for(int j = 0; j < 4; j++){
			tempChar = vectInput[i][j];
			ss << hex << tempChar;
			ss >> tempString;
			output += tempString;
			ss.str("");
            ss.clear();
		}
	}
	return output;
}

void State::checkValidState(){ //Validate for each step
	try{
		//should be 4x4, that's what we're checking.
		for(int i = 0; i <=4; i++){ // I AM including the index 4 to make sure it's null
			if(i == 4){
				if (currentState[i] != NULL) {
					throw 4;
				}
			} else {
				if(currentState[i] == NULL){
					throw 5;
				}

			}

		}

	} catch (...){
		cout << "Invalid State " << num << endl;
	}
}

// addition is xor the bit values of unsigned char
unsigned char State::addition(unsigned char p1, unsigned char p2) {
    return p1 ^ p2;
}

unsigned char State::multiply(unsigned char p1, unsigned char p2){
    std::bitset<16> temp; 
    
    for(int i = 7; i >=0; i--){ //
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

vector<unsigned char> State::addCoef(vector<unsigned char> coef1, vector<unsigned char> coef2){
    vector<unsigned char> result;
    for (int i = 0; i < Nb; i++){
        result.push_back(addition(coef1[i], coef2[i])); //Most significant NOT first, based on index #, this is different
    }
    return result;
}

//Please note that these are fundamentally different polynomial representations. The understanding is that the words 
//themselves, represented before as unsigned char/ 8-bit are the coefficents themselves as used here. 
//EACH char is the coefficent of the 4-term polynomial. 
/*MODULAR PRODUCT (coefficent polys) Still (4.3)
Multiplication (MODULAR PRODUCT) is achieved in two steps. In the first step, the polynomial product c(x) = a(x) •
b(x) is algebraically expanded*/
//does • then is modulo x^4 + 1

//I DON'T THINK THIS WILL WORK FOR OTHER Nk NOT EQUAL TO 4 YET
vector<unsigned char> State::modPro(vector<unsigned char> coef1, vector<unsigned char> coef2){
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

void State::MixColumns(){ //Adapted
	checkValidState();
    vector<unsigned char> aVector;
    vector<vector<unsigned char>> newState;
    
    //AES definied value (equation 4.14) a(x) = {03}x^3 + {01}x^2 + {01}x +{02}
    aVector.push_back(0x02);
    aVector.push_back(0x01);
    aVector.push_back(0x01);
    aVector.push_back(0x03);
    
    for(int i = 0; i < Nb; i++){ //operates column by column, so column 0 to 3 
        vector<unsigned char> columnVector;
        
        columnVector = modPro(aVector, currentState[i]); //i represents which column, when in this weve bunched it up by columns so the outsider vector is the rows
        newState.push_back(columnVector);
        
    }
    
    currentState = newState;
}

void State::invMixColumns(){ //Adapted
	checkValidState();
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
        
        columnVector = modPro(ainvVector, currentState[i]); 
        newinvState.push_back(columnVector);
        
    }
    
    currentState = newinvState;
}

vector<unsigned char> State::subWord(vector<unsigned char> in)
{
    for(int i = 0; i < in.size(); i++)
    {
        unsigned char x = (in[i] & 0xf0) >> 4;
        unsigned char y = in[i] & 0x0f;
        in[i] = sbox[x][y];
    }
    return in;
}

vector<unsigned char> State::rotWord(vector<unsigned char> in)
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
//*****PROBLEM: This won't work for longer than 128 bit cipher
//Key is the opposite demensions as the state - will always have 4 in a column, but variable amount of columns because of matrix multiplication
vector<vector<unsigned char>> State::KeyExpansion(vector<vector<unsigned char>> cipherkey){
    //number of words: [Nb*(Nr+1)]

    vector<vector<unsigned char>> wordList;
    vector<unsigned char> temp;
    vector<unsigned char> word;
    vector<unsigned char> Rcon;
    
    
    int i = 0;
    while (i < Nk){
        word = cipherkey[i];
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

void State::AddRoundKey(int rround, vector<vector<unsigned char>> KeyExpansion){
    int l = rround * Nb;
    for(int i = 0; i < Nb; i++){
        currentState[i] = addCoef(currentState[i], KeyExpansion[l + i]);
    }
}

void State::invAddRoundKey(void, int rround, vector<vector<unsigned char>> KeyExpansion){
    AddRoundKey(rround, KeyExpansion);
}

void State::subBytes() 
{
	checkValidState();

    for(int i = 0; i < currentState.size(); i++)
    {
        for(int j = 0; j < currentState.size(); j++)
        {
            unsigned char entry = currentState[i][j];
            unsigned char y = entry & 0x0f;
            unsigned char x = (entry & 0xf0)>>4;
            currentState[i][j] = sbox[x][y];
            //cout << "entry is " << hex << (int) entry << " x is " << hex << (int) x << " y is " << hex << (int) y << " box is " << hex << (int) sbox[x][y] << '\n';
        }
    }
}

void State::invSubBytes()
{
	checkValidState();

    //for each item in the state, find the row/column combination to find the original byte.
    unsigned char xvalue = 0x00;
    unsigned char yvalue = 0x00;
    for(int i = 0; i < currentState.size(); i++)
    {
        for(int j = 0; j < currentState.size(); j++)
        {
            unsigned char entry = currentState[i][j];
            for(int k = 0; k < sbox.size(); k++)
            {
                for(int l = 0; l < sbox.size(); l++)
                {
                    if(sbox[k][l] == currentState[i][j])
                    {
                        xvalue = k;
                        yvalue = l;
                        //cout << "x is " << hex << (int) xvalue << " y is " << hex << (int) yvalue << "\n";
                        xvalue = xvalue << 4;
                        entry = xvalue | yvalue;
                    }
                    
                }
            }
            currentState[i][j] = entry;
        }
    }
}

void State::ShiftRows(){
	checkValidState();
    vector<vector<unsigned char>> resultingState;
    vector<unsigned char> v;
    
    for (int k=0;k<4;k++){
	    
        v.clear();
        

	    for (int l=0;l<4;l++){
	        
		    v.push_back(currentState[(k+l) % Nb][l]);
		    //cout << hex << (int)v[l] << endl;
		    
	    }

	    //std::rotate(v.begin(), v.begin()+count, v.end());

	    resultingState.push_back(v);

    }
    currentState = resultingState;
}

void State::invShiftRows(){
	checkValidState();
    vector<vector<unsigned char>> resultingState;
    vector<unsigned char> v; 
    
    for (int k=0;k<4;k++){
	    int count=k;
        v.clear();

	    for (int l=0;l<4;l++){
	    	v.push_back(currentState[(k-l+Nb) % Nb][l]);
	    }

	    //std::rotate(v.begin(), v.begin()+v.size()-count, v.end());

	    resultingState.push_back(v);

    }
    
    currentState = resultingState;
}

State::State(){
	currentState = {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}};

}

State::State(string hexInput){
	currentStateString = hexInput;


}

State::State(string hexInput, int inputNk){
	currentStateString = hexInput;
	try{
		if((inputNk != 4) && (inputNk != 6) && (inputNk != 8)){
			throw 6;
		}
	} catch (...) {
		
	}

}

State::State(vector<vector<unsigned char>> input){


}

State::State(vector<vector<unsigned char>> input, int inputNk){
	
}


vector<vector<unsigned char>> State::getCurrentStateVect();
void State::setCurrentStateVect(vector<vector<unsigned char>> inputVect);

string State::getCurrentStateString();
void State::setCurrentStateString(string hexInput);

void State::viewCurrentState();




void State::Cipher(vector<vector<unsigned char>> cipherKey){
    
    vector<vector<unsigned char>> roundKeyVec = KeyExpansion(cipherKey);
    
    AddRoundKey(0, roundKeyVec);
    
    for(int i = 1; i < Nr ; i++){
        subBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(i, roundKeyVec);
    }
    
    subBytes();
    ShiftRows();
    AddRoundKey(Nr, roundKeyVec); 
} 
    
void State::invCipher(vector<vector<unsigned char>> cipherKey){
    
    vector<vector<unsigned char>> roundKeyVec = KeyExpansion(cipherKey);
    
    invAddRoundKey(Nr, roundKeyVec);
    
    for(int i = Nr-1; i >= 1; i--){
        invShiftRows();
		invSubBytes();
        invAddRoundKey(i, roundKeyVec);
        invMixColumns();
    }
    
    invShiftRows();
    invSubBytes();
    invAddRoundKey(0, roundKeyVec); 
} 






