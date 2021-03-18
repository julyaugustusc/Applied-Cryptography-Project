#include <iostream>

vector<vector<unsigned char>> ECBmodeEncrypt (vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput,vector<vector<unsigned char>> key){

	//padding if input size is not multiple of block size

	for (int i = 0; i < 3; i++) { 
        for ( 
            auto it = cipherInput[i].begin(); 
            it != cipherInput[i].end(); it++){
     //   		Cipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput)
            }
        cout << endl; 
    }

}

vector<vector<unsigned char>> ECBmodeDecrypt (vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput,vector<vector<unsigned char>> key, vector<vector<unsigned char>> Cipher){

	for (int i = 0; i < 3; i++) { 
        for ( 
            auto it = Cipher[i].begin(); 
            it != Cipher[i].end(); it++){
     //   		invCipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput)
            }
        cout << endl; 
    }

}


vector<vector<unsigned char>> CBCmodeEncrypt (vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput,vector<vector<unsigned char>> key,unsigned char iv){

	//padding if input size is not multiple of block size
	//Generate IV and assign it to the block variable

	for (int i = 0; i < 3; i++) { 
        for ( 
            auto it = cipherInput[i].begin(); 
            it != cipherInput[i].end(); it++){
     //   		Xor(block,vector<vector<unsigned char>> cipherInput)
     //   		Cipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput)
            }
        cout << endl; 
    }

}


vector<vector<unsigned char>> CBCmodeDecrypt (vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput,vector<vector<unsigned char>> key, vector<vector<unsigned char>> Cipher){

	for (int i = 0; i < 3; i++) { 
        for ( 
            auto it = Cipher[i].begin(); 
            it != Cipher[i].end(); it++){
      //   		invCipher(vector<vector<unsigned char>> state, vector<vector<unsigned char>> cipherInput)
     //  		Xor(block,vector<vector<unsigned char>> Cipher) 
            }
        cout << endl; 
    }

}