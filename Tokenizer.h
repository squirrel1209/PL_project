#ifndef Tokenizer_H
#define Tokenizer_H

#include <iostream>
#include <vector>
#include <cctype> // 處理字符  
#include <string>
#include <fstream>

using namespace std;
enum class Type {
    RPAREN,    // ')'
    LPAREN,    // '('
    IDENT,
    NUM,
    SIGN,
    OTHER,
    QUIT
};

struct TokenWithType {
    string token;
    Type type;
};

class Tokenizer {
private:
    vector<string> input;
    int lineIndex;
    int columnIndex;
    
public:
    Tokenizer( vector<string> input) : input(input), lineIndex(0), columnIndex(0) {}

    // 函數：處理token，將每個token加上型態
    vector<TokenWithType> processTokens() {
        vector<TokenWithType> tokensWithType;
        string token = getNextToken() ;
        
        // 迴圈處理每個token，並給予型態 
        while ( token != "quit" ) {
            TokenWithType tokenWithType;
            tokenWithType.token = token;

            // 假設此函數返回token的型態
            tokenWithType.type = getTokenType( token ); 
            tokensWithType.push_back(tokenWithType);
            token = getNextToken() ;
        }  // end while

        return tokensWithType;
    }  // end processTokens()

    // 函數：返回token的型態
    Type getTokenType( string token ) {
        if ( isIDENT( token ) ) return Type::IDENT;
        else if ( isNUM( token ) ) return Type::NUM;
        else if ( isSIGN( token ) ) return Type::SIGN;
        else if ( token.compare( "(" ) == 0 ) return Type::LPAREN;
        else if ( token.compare( ")" ) == 0 ) return Type::RPAREN;
        else if ( token.compare( "\0" ) == 0 ) return Type::QUIT;
        else return Type::OTHER;
    } // end getTokenType()

    bool isSIGN( string str ) {
        if ( str.compare( "+" ) == 0 || str.compare( "-" ) == 0 )
            return true ;
        else return false ;
    } // end isSIGN

    bool isNUM( string str ) {
    // 空字符串不是數字
    if ( str.empty() ) return false;

    bool hasDot = false; // 標記是否有小數點
    bool hasDigit = false; // 標記是否有數字

    // 檢查每個字符
    for ( char c : str ) {
        if ( c == '.' ) {
            if ( hasDot ) {
                // 已經有一個小數點，這不是合法的浮點數
                return false;
            }
            hasDot = true;
        } // end if 
        
        else if ( isdigit(c) ) hasDigit = true;
       
        else {
            // 如果不是數字也不是小數點，返回 false
            return false;
        } // end else
    } // end for

        // 最終，需要至少有一個數字才能確定是有效的數字表示
        return hasDigit;
    } // end isNum

    bool isIDENT( string str ) {
      if ( str.empty() ) // 空字串不是合法的標識符
        return false;

      if ( !isalpha( str[0] ) ) // 第一個字元必須是字母
        return false;

      for ( size_t i = 1; i < str.length(); i++ ) {
        if ( !isalnum( str[i] ) && str[i] != '_' ) // 其餘字元必須是數字、字母或底線
            return false;
      } // end for

        return true;
    } // end isIDENT
    
    bool isDelimiter( char ch ) {
        if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' )
            return true ;
        else return false ;
    	
    } // end isDelimiter
    
    string getDelimiter( char ch ) {
        if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' ) {
            return string(1, ch);
        } // end if
    } // end getDelimiter()
//---------------------------------------GetToken--------------------------------------- 
    string getNextToken() {
    //getNextToken():取得下一個 token
    //取得下一個非whitespace的char
    //1.如果下一個char為delimiter就直接回傳delimiter
    //2.否則取得剩餘的token ( 呼叫 readRemainingToken )
    
        char nextChar = getNextNonWhiteSpaceChar();
        if ( lineIndex == input.size() ) {
            return "quit";
        } // end if 
        
        else if ( nextChar == '/' ) {
            nextChar = getNextChar();
            
            if ( nextChar == '/' ) {
                while ( nextChar != '\n' ) nextChar = getNextChar() ;
                nextChar = getNextNonWhiteSpaceChar();
                string tokenValue = readRemainingToken();
                return nextChar + tokenValue;
            } // end if
            
	  else {
	    columnIndex-- ;
	    return "/" + readRemainingToken() ;
	  } // end else
        } // end else if 
        
        else if ( isDelimiter( nextChar ) ) {
           return getDelimiter( nextChar ) ;
        } // end else if
        
        else {
            string tokenValue = readRemainingToken();
            return nextChar + tokenValue;
        } // end else 
    } // end getNextToken()

    string readRemainingToken() {
    //readRemainingToken():讀取 token 的剩餘部分
    //讀下一個token
    //如果下一個token並非whitespace或者delimiter，則把char接成token 

        string tokenValue;
        char nextChar = getNextChar();
        
        while ( !isspace( nextChar ) && !isDelimiter( nextChar ) ) { // 如果不是writespace 和 delimiter 
        
            if ( nextChar == '/' ) {       // 處理連續的註解 
                nextChar = getNextChar() ;
                if ( nextChar == '/' ) 
                	while ( nextChar != '\n' ) nextChar = getNextChar() ;
	      else columnIndex-- ;
            } // end if
            
            else {
                tokenValue += nextChar;
                nextChar = getNextChar();
            } // end else
        } // end while
        
        if ( isDelimiter( nextChar ) ) columnIndex-- ; // delimiter不算此token，往前一個column 
        return tokenValue;
    } // readRemainingToken() 

    char getNextNonWhiteSpaceChar() {
    //getNextNonWhiteSpaceChar():取得下一個非空白字符並跳過註解
    //讀下一個char，如果今天為 
    //1.whitespace，則繼續讀，讀到非 whitespace
    //2.#，則把跳過註解到行末
    
        char nextChar = getNextChar();
        while ( isspace( nextChar ) || nextChar == '#' ) {
            if ( nextChar == '#' ) {    
	  // 跳過註解直到行末
                while ( nextChar != '\n' && nextChar != '\0' ) {
                    nextChar = getNextChar();
                } // end while
            } // end if
            
            nextChar = getNextChar();
        } // end while
        
        return nextChar;
    } // end getNextNonWhiteSpaceChar
   
    char getNextChar() {
    //char getNextChar():取得下一個字符並追蹤行號和列號
    //如果沒有超出input的大小，則讀取下一個char，並且記錄row和col 
    //超過col大小則換行 
        //cout << "lineIndex:" << lineIndex << " columnIndex" << columnIndex << endl ;
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
        
        return '\0'; // 若達到輸入結尾則返回空字符
    } // end getNextChar()

};

#endif
