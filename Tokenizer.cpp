#include <iostream>
#include <vector>
#include <cctype> // �B�z�r��  
#include <string>
#include <fstream>

#ifndef Tokenizer_H
#define Tokenizer_H

class Tokenizer {
private:
    vector<string> input;
    int lineIndex;
    int columnIndex;

public:
    Tokenizer( vector<string> input) : input(input), lineIndex(0), columnIndex(0) {}
    
    char getNextChar() {
    //char getNextChar():���o�U�@�Ӧr�Ũðl�ܦ渹�M�C��
    //�p�G�S���W�Xinput���j�p�A�hŪ���U�@��char�A�åB�O��row�Mcol 
    //�W�Lcol�j�p�h���� 
    
        if ( lineIndex < input.size() ) {
            if ( columnIndex < input[lineIndex].size() ) {
                char nextChar = input[lineIndex][columnIndex++];
                return nextChar;
            } // end if
            
	  else {
                lineIndex++;
                columnIndex = 0;
                return '\n'; // ����Ū�ܦ浲��
            } // end else
            
        } // end if
        //cout << "getNextChar��X����\0" << endl ;
        return '\0'; // �Y�F���J�����h��^�Ŧr��
    } // end getNextChar()
    

};

#endif
