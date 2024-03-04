#ifndef Tokenizer_H
#define Tokenizer_H

#include <iostream>
#include <vector>
#include <cctype> // �B�z�r��  
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

    // ��ơG�B�ztoken�A�N�C��token�[�W���A
    vector<TokenWithType> processTokens() {
        vector<TokenWithType> tokensWithType;
        string token = getNextToken() ;
        
        // �j��B�z�C��token�A�õ������A 
        while ( token != "quit" ) {
            TokenWithType tokenWithType;
            tokenWithType.token = token;

            // ���]����ƪ�^token�����A
            tokenWithType.type = getTokenType( token ); 
            tokensWithType.push_back(tokenWithType);
            token = getNextToken() ;
        }  // end while

        return tokensWithType;
    }  // end processTokens()

    // ��ơG��^token�����A
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
            }
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
