#include <vector>
#include <iostream>
#define VERBOSE 0
using namespace std;

int main() 
{
    void printState(vector<vector<unsigned char>> state);
    vector<vector<unsigned char>> subBytes(vector<vector<unsigned char>>, vector<vector<unsigned char>>);
    vector<vector<unsigned char>> invSubBytes(vector<vector<unsigned char>>, vector<vector<unsigned char>>);
    vector<unsigned char> subWord(vector<unsigned char>, vector<vector<unsigned char>>);

    
    //lengh of input block is 128 bits, nb=4 because 4 32-bit words (columns) in the state
    int nb = 4;
    // length of  key is 128, 192, 256 bits
    // nk can be 4, 6, 8
    int nk = 4;
    // nr is a func of nk and nb and can be 10, 12, 14
    int nr;
    if(nk == 4){nr = 10;}
    else if(nk == 6){nr = 12;}
    else{nr = 14;}

    
    vector<vector<unsigned char>> input = {{0x19, 0xa0, 0x9a, 0xe9}, {0x3d, 0xf4, 0xc6, 0xf8}, {0xe3, 0xe2, 0x8d, 0x48}, {0xbe, 0x2b, 0x2a, 0x08}};
    vector<vector<unsigned char>> state = input;
    
    vector<vector<unsigned char>> sbox;
    sbox = 
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

    /* AddRoundKey();

    for(int r = 1; r < nr)
    {
        subBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey();
    }

    subBytes();
    ShiftRows();
    MixColumns(); 
    
    return state; */

    state = subBytes(state, sbox);

    if(VERBOSE){printState(state);}

    state = invSubBytes(state, sbox);

    if(VERBOSE){printState(state);}

    vector<unsigned char> word = {0x19, 0xa0, 0x9a, 0xe9};
    if(VERBOSE)
    {
        for(int i = 0; i < word.size(); i++)
        {
            cout << hex << (int) word[i] << " ";
        }
        cout << " \n";
    }
    word = subWord(word, sbox);
    if(VERBOSE)
    {
        for(int i = 0; i < word.size(); i++)
        {
            cout << hex << (int) word[i] << " ";
        }
        cout << " \n";
    }

    return 0;


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
                        cout << "x is " << hex << (int) xvalue << " y is " << hex << (int) yvalue << "\n";
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