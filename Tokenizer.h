#ifndef Tokenizer_H
#define Tokenizer_H

#include <iostream>
#include <vector>
#include <cctype> // �B�z�r��  
#include <string>
#include <fstream>

using namespace std;

enum Type {
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
    SIGN,
    ERROR,
    QUIT,
    
//-------��k--------
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
};

struct Token {
    string tokenName;
    Type type;
    Error error;
};

class Tokenizer {
private:
    vector<string> input;
    int lineIndex;
    int columnIndex;
    
public:
    Tokenizer( vector<string> input) : input(input), lineIndex(0), columnIndex(0) {}

    // ��ơG�B�ztoken�A�N�C��token�[�W���A�H�ο��~ 
    vector<Token> processTokens() {
        vector<Token> tokens;
        string tokenName = getNextToken() ;
        
        // �j��B�z�C��token�A�õ������A 
        while ( tokenName != "quit" ) {
            Token token;
            token.tokenName = tokenName;

            // ���]����ƪ�^token�����A
            token.type = analyzeToken( tokenName );
            
            if ( token.type == ERROR ) {
                // �B�z�ҥ~�A�|�������㪺token �Ҧp:floatfloat 1.23.23

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
	          // ���~�B�z 
                    token.error.errorValue = string( 1, checkError( tokenName ) );
                } // end else
	  } // end if
	  
            tokens.push_back(token);
            tokenName = getNextToken() ;
        }  // end while

        return tokens;
    }  // end processTokens()

    char checkError( string str ) {
        Error error ;
        char c;
        int i = 0 ;
        if ( isalpha( str[0] ) ) {  // �OIDENT�����~ 
        
            while ( i < str.length() && ( !isalnum( str[i] ) && str[i] != '_' ) ) {
                if ( !isalnum( str[i] ) && str[i] != '_' ) {   // ��l�r�������O�Ʀr�B�r���Ω��u
                    return str[i] ;
                } // end if
                
                i++ ;
            } // end while
        } // end if
        
        else {
            bool hasDot = false; // �аO�O�_���p���I
            bool hasDigit = false; // �аO�O�_���Ʀr

            // �ˬd�C�Ӧr��  
            for ( char c : str ) {
                if (c == '.' && !hasDot) {
                    hasDot = true;
                } // end if
	      
	      else if (c == '+' || c == '-') {
                // �p�G�w�g���Ʀr�X�{�L�A�B���t�����b�Ĥ@��A�h��^ false
                    if (hasDigit || &c != &str[0]) {
                        return c ;
                    } // end if
                } // end else if
                
	      else if (isdigit(c)) {
                    hasDigit = true;
                } // end else if
	      
	      else {
                    // �p�G���O�Ʀr�]���O�p���I�A��^ false
                    return c ;
                } // end else
            } // end for
            
            if ( hasDot == true && str.size() == 1 ) return '.';
        } // end else
    } // end checkError()

    // ��ơG���Rtoken�����A�A���ˬd�O�_��Error 
    Type analyzeToken( string token ) {
        
        if ( isIDENT( token ) ) Type::IDENT;
        else if ( isNUM( token ) ) {
            if ( isInt( token ) ) return Type::INT;
	  else return Type::FLOAT;
        } // end else if
        else if ( isSIGN( token ) ) return Type::SIGN;
        else if ( token.compare( "(" ) == 0 ) return Type::LPAREN;
        else if ( token.compare( ")" ) == 0 ) return Type::RPAREN;
        else if ( token.compare( "*" ) == 0 ) return Type::MULTIPLY;
        else if ( token.compare( "/" ) == 0 ) return Type::DIVIDE;
        else if ( token.compare( "+" ) == 0 ) return Type::PLUS;
        else if ( token.compare( "-" ) == 0 ) return Type::MINUS;
        else if ( token.compare( "=" ) == 0 ) return Type::EQUAL;
        else if ( token.compare( "<>" ) == 0 ) return Type::NOTEQUAL;
        else if ( token.compare( ">" ) == 0 ) return Type::GREATER;
        else if ( token.compare( "<" ) == 0 ) return Type::LESS;
        else if ( token.compare( ">=" ) == 0 ) return Type::GREATEREQUAL;
        else if ( token.compare( "<=" ) == 0 ) return Type::LESSEQUAL;
        else if ( token.compare( ":=" ) == 0 ) return Type::ASSIGN;
        else if ( token.compare( ";" ) == 0 ) return Type::SEMICOLON;
        else if ( token.compare( "\0" ) == 0 ) return Type::QUIT;
        else return Type::ERROR;
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

        for ( char c : str ) {
            if ( !isdigit(c) ) {
                return false;
            } // end if
       } // end for

        return true;
    } // end isInt()

    bool isNUM( string str ) {
    // �Ŧr�Ŧꤣ�O�Ʀr
    if ( str.empty() ) return false;

    bool hasDot = false; // �аO�O�_���p���I
    bool hasDigit = false; // �аO�O�_���Ʀr

    // �ˬd�C�Ӧr��
    for ( char c : str ) {
        if ( c == '.' ) {
            if ( hasDot ) {
                // �w�g���@�Ӥp���I�A�o���O�X�k���B�I��
                return false;
            } // end if
            
            hasDot = true;
        } // end if 
        
        else if ( isdigit(c) ) hasDigit = true;
       
        else {
            // �p�G���O�Ʀr�]���O�p���I�A��^ false
            return false;
        } // end else
    } // end for

        // �̲סA�ݭn�ܤ֦��@�ӼƦr�~��T�w�O���Ī��Ʀr���
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
	      i-- ; // �^��W�@�� 
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
      if ( str.empty() ) // �Ŧr�ꤣ�O�X�k�����Ѳ�
        return false;

      if ( !isalpha( str[0] ) ) // �Ĥ@�Ӧr�������O�r��
        return false;

      for ( size_t i = 1; i < str.length(); i++ ) {
        if ( !isalnum( str[i] ) && str[i] != '_' ) // ��l�r�������O�Ʀr�B�r���Ω��u
            return false;
      } // end for

        return true;
    } // end isIDENT
    
    bool isDelimiter( char ch ) {
        if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
             ch == '=' || ch == '<' || ch == '>' || ch == ';' )
            return true ;
            
        else if ( ch == ':' ) {
        	  char nextChar ;
        	  
        	  nextChar = getNextChar() ;
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
        char nextChar ;
        if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
             ch == '='  || ch == ';' ) {
            return string(1, ch);
        } // end if
        
        else if ( ch == '<' ) {
            nextChar = getNextChar() ;
            if ( ch == '>' ) return "<>" ;
            else if ( ch == '=' ) return "<=" ;
            else {
                columnIndex-- ;
                return "<" ;
	  } // end else
        } // end else if
        
        else if ( ch == '>' ) {
            nextChar = getNextChar() ;
            if ( ch == '=' ) return ">=" ;
            else {
                columnIndex-- ;
                return ">" ;
	  } // end else
        } // end else if
        
        else if ( ch == ':' ) {
            nextChar = getNextChar() ;
            if ( ch == '=' ) return ":=" ;
        } // end else if
    } // end getDelimiter()
//---------------------------------------GetToken--------------------------------------- 
    string getNextToken() {
    //getNextToken():���o�U�@�� token
    //���o�U�@�ӫDwhitespace��char
    //1.�p�G�U�@��char��delimiter�N�����^��delimiter
    //2.�_�h���o�Ѿl��token ( �I�s readRemainingToken )
    
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
    //readRemainingToken():Ū�� token ���Ѿl����
    //Ū�U�@��token
    //�p�G�U�@��token�ëDwhitespace�Ϊ�delimiter�A�h��char����token 

        string tokenValue;
        char nextChar = getNextChar();
        bool hasDot = false;
        while ( !isspace( nextChar ) && !isDelimiter( nextChar ) ) { // �p�G���Owritespace �M delimiter 
        
            if ( nextChar == '/' ) {       // �B�z�s�򪺵��� 
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
        
        if ( isDelimiter( nextChar ) ) columnIndex-- ; // delimiter���⦹token�A���e�@��column 
        return tokenValue;
    } // readRemainingToken() 

    char getNextNonWhiteSpaceChar() {
    //getNextNonWhiteSpaceChar():���o�U�@�ӫD�ťզr�Ũø��L����
    //Ū�U�@��char�A�p�G���Ѭ� 
    //1.whitespace�A�h�~��Ū�AŪ��D whitespace
    //2.#�A�h����L���Ѩ�楽
    
        char nextChar = getNextChar();
        while ( isspace( nextChar ) || nextChar == '#' ) {
            if ( nextChar == '#' ) {    
	  // ���L���Ѫ���楽
                while ( nextChar != '\n' && nextChar != '\0' ) {
                    nextChar = getNextChar();
                } // end while
            } // end if
            
            nextChar = getNextChar();
        } // end while
        
        return nextChar;
    } // end getNextNonWhiteSpaceChar
   
    char getNextChar() {
    //char getNextChar():���o�U�@�Ӧr�Ũðl�ܦ渹�M�C��
    //�p�G�S���W�Xinput���j�p�A�hŪ���U�@��char�A�åB�O��row�Mcol 
    //�W�Lcol�j�p�h���� 
        //cout << "lineIndex:" << lineIndex << " columnIndex" << columnIndex << endl ;
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
        
        return '\0'; // �Y�F���J�����h��^�Ŧr��
    } // end getNextChar()

};

#endif
