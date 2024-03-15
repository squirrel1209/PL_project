#ifndef Tokenizer_H
#define Tokenizer_H

#include <iostream>
#include <vector>
#include <cctype> // 處理字符  
#include <string>
#include <fstream>

using namespace std;
typedef char string200[200];

enum Type {
    NONE,
    SEMICOLON,    // ';' 
    ASSIGN,       // ':='
    LESSEQUAL,    // '<='
    GREATEREQUAL, // '>='
    LESS,      // '<'
    GREATER,   // '>'
    NOTEQUAL,  // '<>'	
    EQUAL,     // '='
    MINUS,     // '-'
    PLUS,      // '+'
    DIVIDE,    // '/'
    MULTIPLY,  // '*'
    RPAREN,    // ')'
    LPAREN,    // '('
    IDENT,
    INT,       // NUM == INT || FLOAT
    FLOAT,
    BOOL,
    SIGN,
    ERROR,
    QUIT,
    
//-------文法--------
    ARITHEXP,
    FACTOR,
    TERM,
    STATEMENT,
    COMMAND,
    BOOLEANEXP
};

enum ErrorType {
    lexicalError,
    syntacticalError,
    semanticError
};

struct Error {
    ErrorType type;
    int line;
    string errorValue;

    // constructor
    Error() : type(lexicalError), line(0), errorValue("") {}

};

struct Token {
    string tokenName;
    Type type;
    Error error;
};

Token createToken( string name, Type type, Error error = Error() ) {
    Token token;
    token.tokenName = name;
    token.type = type;
    token.error = error;
    return token;
}

string anyToString(int num) {
    string200 buffer; // Assuming a maximum of 200 characters for the string
    sprintf(buffer, "%d", num);
    return string(buffer);
}

string anyToString(float num) {
    string200 buffer; // Assuming a maximum of 200 characters for the string
    sprintf(buffer, "%.3f", num);
    return string(buffer);
}

string anyToString(double num) {
    string200 buffer; // Assuming a maximum of 200 characters for the string
    sprintf(buffer, "%.3f", num);
    return string(buffer);
}

string anyToString(char ch) {
    string200 buffer; // Single character plus null terminator
    buffer[0] = ch;
    buffer[1] = '\0';
    return string(buffer);
}

class Tokenizer {
private:
    vector<string> input;
    int lineIndex;
    int columnIndex;
    
public:
    Tokenizer( vector<string> input) : input(input), lineIndex(0), columnIndex(0) {}

    // 函數：處理token，將每個token加上型態以及錯誤 
    vector<Token> processTokens() {
        vector<Token> tokens;
        string tokenName = getNextToken() ;
        
        // 迴圈處理每個token，並給予型態 
        while ( tokenName != "quit" ) {
            Token token;
            token.tokenName = tokenName;

            // 假設此函數返回token的型態
            token.type = analyzeToken( tokenName );
            if ( token.type == ERROR ) {
                // 處理例外，尚未切完整的token 例如:floatfloat 1.23.23

	      if ( isMultiFloat(tokenName) ) {
	      	string left = "";
	      	string right= "";
	          splitString( tokenName, left, right) ;
	          token.tokenName = left;
	          token.type = FLOAT;
	          tokens.push_back(token);
	          token.tokenName = right;
	      } // end if
	       
	      else {
	          // 錯誤處理 
                    token.error = checkError( tokenName );
                } // end else
	  } // end if
	  
            tokens.push_back(token);
            tokenName = getNextToken() ;
        }  // end while

        return tokens;
    }  // end processTokens()

    Error checkError( string str ) {
        Error error ;

        int i = 0 ;
        if ( isalpha( str[0] ) ) {  // 是IDENT的錯誤 
            while ( i < str.length() && ( !isalnum( str[i] ) && str[i] != '_' ) ) {
                if ( !isalnum( str[i] ) && str[i] != '_' )    // 其餘字元必須是數字、字母或底線
                    error.errorValue = error.errorValue + str[i] ;

                i++ ;
            } // end while
            
            error.type = syntacticalError;
        } // end if
        
        else {
            bool hasDot = false; // 標記是否有小數點
            bool hasDigit = false; // 標記是否有數字

            // 檢查每個字符  
            for (size_t i = 0; i < str.length(); i++) {
                char c = str[i];
                if (c == '.' && !hasDot) {
                    hasDot = true;
                } // end if
	      
	      else if (c == '+' || c == '-') {
                // 如果已經有數字出現過，且正負號不在第一位，則返回 false
                    if (hasDigit || &c != &str[0]) {
                        error.errorValue = anyToString(c);
                        error.type = lexicalError;
                        return error;
                    } // end if
                } // end else if
                
	      else if (isdigit(c)) {
                    hasDigit = true;
                } // end else if
	      
	      else {
                    // 如果不是數字也不是小數點，返回 false
                    error.errorValue = error.errorValue + str[i] ;
                    error.type = syntacticalError;
                } // end else
            } // end for
            
	  return error;
        } // end else
    } // end checkError()

    // 函數：分析token的型態，並檢查是否有Error 
    Type analyzeToken( string token ) {
    	
        if ( isIDENT( token ) ) return IDENT;
        
        else if ( isNUM( token ) ) {
            if ( isInt( token ) ) return INT;
	  else return FLOAT;
        } // end else if
        else if ( isSIGN( token ) ) return SIGN;
        else if ( token.compare( "(" ) == 0 ) return LPAREN;
        else if ( token.compare( ")" ) == 0 ) return RPAREN;
        else if ( token.compare( "*" ) == 0 ) return MULTIPLY;
        else if ( token.compare( "/" ) == 0 ) return DIVIDE;
        else if ( token.compare( "+" ) == 0 ) return PLUS;
        else if ( token.compare( "-" ) == 0 ) return MINUS;
        else if ( token.compare( "=" ) == 0 ) return EQUAL;
        else if ( token.compare( "<>" ) == 0 ) return NOTEQUAL;
        else if ( token.compare( ">" ) == 0 ) return GREATER;
        else if ( token.compare( "<" ) == 0 ) return LESS;
        else if ( token.compare( ">=" ) == 0 ) return GREATEREQUAL;
        else if ( token.compare( "<=" ) == 0 ) return LESSEQUAL;
        else if ( token.compare( ":=" ) == 0 ) return ASSIGN;
        else if ( token.compare( ";" ) == 0 ) return SEMICOLON;
        else if ( token.compare( "\0" ) == 0 ) return QUIT;
        else return ERROR;
    } // end getTokenType()

    void splitString( string str, string& left, string& right) {
        bool firstDot = false;
        bool secondDot = false;
        bool thirdThrow = false;
        
        for ( int i = 0 ; i < str.size() ; i++ ) {
            if ( str[i] == '.' && firstDot == false ) firstDot = true;
            else if ( secondDot == true && ( !isdigit(str[i]) ) ) thirdThrow = true;
            else if ( firstDot == true && str[i] == '.' ) secondDot = true;
	      
	  if ( thirdThrow == false ) {
	      if ( secondDot == true ) {
	          right = right + str[i];
	      } // end if
	  
	      else left = left + str[i];
            } // end if
        } // end for
    } // end splitString()
    
    bool isSIGN( string str ) {
        if ( str.compare( "+" ) == 0 || str.compare( "-" ) == 0 )
            return true ;
        else {
            return false ;
        } // end else
    } // end isSIGN

    bool isInt( string str ) {
        if ( str.empty() ) return false;

        for (size_t i = 0; i < str.length(); i++) {
            char c = str[i];
            if ( !isdigit(c) ) {
                return false;
            } // end if
       } // end for

        return true;
    } // end isInt()

    bool isNUM( string str ) {
    // 空字符串不是數字
    if ( str.empty() ) return false;

    bool hasDot = false; // 標記是否有小數點
    bool hasDigit = false; // 標記是否有數字

    // 檢查每個字符
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if ( c == '.' ) {
            if ( hasDot ) {
                // 已經有一個小數點，這不是合法的浮點數
                return false;
            } // end if
            
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
 
    bool isMultiFloat( string str ) {
        bool lastDigit = false ;
        int dot = 0 ;
        int digit = 0 ;
        int i = 0 ;
        if ( str[ str.size() -1 ] == '.' ) return false ;
        
        while ( i < str.size() ) {
        	  while ( isdigit( str[i] ) && i < str.size() ) {
        	      lastDigit = true ;
        	      i++;
	  } // end while
	  
	  if ( lastDigit == true ) {
	      digit++ ;
	      i-- ; // 回到上一個 
	  } // end if
	  
	  else if ( str[i] == '.' ) dot++ ;
	  
	  lastDigit = false ;
        	  i++ ;
        	  
        	  if ( ( dot == digit - 1 ) && digit > 2 ) return true ;
        } // end while
        
        if ( digit == 2 && dot == 2 ) return true ;
        else return false ;
    } // end isMultiFloat

    bool isIDENT( string str ) {
        if ( str.empty() )// 空字串不是合法的標識符
            return false;

        if ( !isalpha( str[0] ) )  // 第一個字元必須是字母
            return false;

        if ( str.compare( "quit" ) == 0 ) // 檢查是否為 "quit"
            return false;

        for ( size_t i = 1; i < str.length(); i++ ) {
            if ( !isalnum( str[i] ) && str[i] != '_' )  // 其餘字元必須是數字、字母或底線
                return false;
        } // end for

        return true;
    } // end isIDENT

    bool isDelimiter( char ch ) {
        if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
             ch == '=' || ch == '<' || ch == '>' || ch == ';' )
            return true ;
            
        else if ( ch == ':' ) {
        	  ch = getNextChar() ;
        	  if ( ch == '=' ) {
        	      columnIndex-- ;
        	      return true ;
	  } // end if
	  
	  else {
	      columnIndex-- ;
	      return false ;
	  } // end else
        } // end else if
        else return false ;
    	
    } // end isDelimiter
    
    string getDelimiter( char ch ) {
        if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
             ch == '='  || ch == ';' ) {
            return string(1, ch);
        } // end if
        
        else if ( ch == '<' ) {
            ch = getNextChar() ;
            if ( ch == '>' ) return "<>" ;
            else if ( ch == '=' ) return "<=" ;
            else {
                columnIndex-- ;
                return "<" ;
	  } // end else
        } // end else if
        
        else if ( ch == '>' ) {
            ch = getNextChar() ;
            if ( ch == '=' ) return ">=" ;
            else {
                columnIndex-- ;
                return ">" ;
	  } // end else
        } // end else if
        
        else if ( ch == ':' ) {
            ch = getNextChar() ;
            if ( ch == '=' ) return ":=" ;
            else return "quit";
        } // end else if
        
        else return "quit";
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
        
        else if ( isDelimiter( nextChar ) ) {  // 待修 
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
        bool hasDot = false;
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
