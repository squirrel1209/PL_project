#include <iostream>
#include <vector>
#include <cctype> // 處理字符  
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
    //char getNextChar():取得下一個字符並追蹤行號和列號
    //如果沒有超出input的大小，則讀取下一個char，並且記錄row和col 
    //超過col大小則換行 
    
        if ( lineIndex < input.size() ) {
            if ( columnIndex < input[lineIndex].size() ) {
                char nextChar = input[lineIndex][columnIndex++];
                return nextChar;
            } // end if
            
	  else {
                lineIndex++;
                columnIndex = 0;
                return '\n'; // 換行符表示行結束
            } // end else
            
        } // end if
        //cout << "getNextChar輸出結束\0" << endl ;
        return '\0'; // 若達到輸入結尾則返回空字符
    } // end getNextChar()
    

};

#endif
