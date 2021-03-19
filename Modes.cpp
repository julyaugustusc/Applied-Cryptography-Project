#include <stdio.h>
#include <iostream>
#include <string>

int Nb = 4;
int Nk = 4;

int b = Nb * Nk; //input and output blocks it's the number of columns * rows? this is then has 8 bits per block


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
            inputChar = "0";
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



vector<vector<vector<unsigned char>>> CBCmodeEncrypt (vector<vector<vector<unsigned char>>> inputBlocks, vector<vector<unsigned char>> IV){
    // ssuming that we've transformed the IV into a similar sized "state"
    //the input text would also be transformed to a vector of states. So it can go one by one as many times as needed.
    vector<vector<unsigned char>> tempBlock;
    vector<vector<vector<unsigned char>>> outputBlocks;
    
    vector<vector<unsigned char>> outCipher;
    
    for(int i = 0; i < IV.size(); i++){
        tempBlock.push_back(addCoef(IV[i],inputBlocks[0][i]));
    }
    
    outCipher = Cipher(tempBlock);
    
    outputBlocks.push_back(outCipher);
    
    for(int i = 1; i<inputBlocks.size(); i++){
        tempBlock.clear();
        
        for(int j = 0; j < IV.size(); j++){ // or outCipher.size()
            tempBlock.push_back(addCoef(outCipher[j],inputBlocks[i][j]));
        }
        
        outCipher.clear();
        outCipher = Cipher(tempBlock);
        
        outputBlocks.push_back(outCipher);
        
    }
    
    return outputBlocks;
}

vector<vector<vector<unsigned char>>> CBCmodeDecrypt (vector<vector<vector<unsigned char>>> cipherText, vector<vector<unsigned char>> IV){
    // ssuming that we've transformed the IV into a similar sized "state"
    //the input text would also be transformed to a vector of states. So it can go one by one as many times as needed.
    vector<vector<unsigned char>> tempBlock;
    vector<vector<vector<unsigned char>>> plainText;
    
    vector<vector<unsigned char>> outInvCipher = invCipher(cipherText[0]);
   
    for(int i = 0; i < IV.size(); i++){
        tempBlock.push_back(addCoef(IV[i], outInvCipher[i]));
    }
    
    plainText.push_back(tempBlock);
    
    for(int i = 1; i<input.size(); i++){
        tempBlock.clear();
        
        outInvCipher = invCipher(cipherText[i]);
        
        for(int j = 0; j < IV.size(); j++){
            tempBlock.push_back(addCoef(outInvCipher[j], cipherText[i-1][j]));
        }
        
        plainText.push_back(tempBlock);
        
    }
    
    return plainText;
}
    
    
     
