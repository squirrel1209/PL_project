#ifndef Tokenizer_H
#define Tokenizer_H

# include <iostream>
# include <stdio.h>
# include <vector>
# include <cctype> 
# include <string>
 #include <fstream>

using namespace std;
typedef char String200[200];

// �w�q���P�������аO�βŸ�
enum Type {
  QUIT,
  ERROR,
  IDENTIFIER,
  CONSTANT,
  INT,
  FLOAT,
  CHAR,
  BOOL,
  STRING,
  VOID,
  IF,
  ELSE,
  WHILE,
  DO,
  RETURN,
  LPAREN,    // '('
  RPAREN,    // ')'
  LBRACKET,  // '['
  RBRACKET,  // ']'
  LBRACE,    // '{'
  RBRACE,    // '}'
  PLUS,      // '+'
  MINUS,     // '-'
  ASTERISK,  // '*'
  SLASH,     // '/'
  PERCENT,   // '%'
  CARET,     // '^'
  GT,        // '>'
  LT,        // '<'
  GE,        // >= 
  LE,        // <= 
  EQ,        // == 
  NEQ,       // != 
  AMPERSAND, // '&'
  PIPE,      // '|'
  ASSIGN,    // '='
  EXCLAMATION, // '!'
  AND,       // &&
  OR,        // ||
  PE,        // +=
  ME,        // -=
  TE,        // *=
  DE,        // /=
  RE,        // %=
  PP,        // ++
  MM,        // --
  RS,        // >>
  LS,        // <<
  SEMICOLON, // ';'
  COMMA,     // ','
  QUESTION,  // '?'
  COLON      // ':'
};

enum ErrorType {
  LEXICALERROR,      // ����k���R���J��@�ӥ��L�k�ѧO�����󦳮ĲŸ����r�Ůɳ��i�����~�C
  SYNTACTICALERROR,  // ��y�k���R���b���R�Ÿ��ǦC�ɹJ��@�Ӥ��ŦX��e�y�k�W�h���Ÿ��ɳ��i�����~�C
  SEMANTICERROR,     // ��y�q���R���b�ˬd�{�����y�q�ɵo�{�ϥΤF���w�q�����ѲŮɳ��i�����~�C
  OTHERERROR         // ��ܨ�L���~
};

// �w�q�аO���c��A�]�t�аO�W�١B�����B�Ҧb��ƩM���~�H��
struct Token {
  string tokenName;  // �аO�W��
  Type type;         // �аO����
  int line;          // �аO�Ҧb�����
  ErrorType error;       // ���p�����~�H��
};

string AnyToString( int num ) {
  String200 buffer;        
  sprintf( buffer, "%d", num );  
  return string( buffer );     
} // end AnyToString()

// �w�q�@�Ө�ơA�N�B�I�ơ]float�^�ഫ���r�Ŧ�C
string AnyToString( float num ) {
  String200 buffer;        
  sprintf( buffer, "%f", num );
  return string( buffer );     
} // end AnyToString()

// �w�q�@�Ө�ơA�N����ׯB�I�ơ]double�^�ഫ���r�Ŧ�C
string AnyToString( double num ) {
  String200 buffer;        
  sprintf( buffer, "%f", num );
  return string( buffer );     
} // end AnyToString()

// �w�q�@�Ө�ơA�N�r���ഫ���r�Ŧ�C
string AnyToString( char ch ) {
  String200 buffer;        
  buffer[0] = ch;          
  buffer[1] = '\0';        
  return string( buffer );     
} // end AnyToString()

class Tokenizer {
private:
  vector<string> minput;
  int mlineIndex;
  int mcolumnIndex;
  int mlastmlineIndex;
public:
  Tokenizer( vector<string> input ) {
    minput = input;
    mlineIndex = 0;
    mcolumnIndex = 0;
  } // end Tokenizer()

  // �B�z�M���R�Ӧۿ�J�� tokens�A�ᤩ�C�� token �����������C
  vector<Token> ProcessTokens() {
  // - **Token �B�z�G** �v�@Ū���ä��R tokens�A����J��פ�šC
  // - **�������R�G** �ϥ� `AnalyzeToken` ��ƨӽT�w�C�� token �������C
  // - **���~�B�z�G** �Y token ������ ERROR�A�i���B�~���ˬd�M�B�z�C
  // - **�h�B�I�ƳB�z�G** �S���p�U�A�p token ���h�ӯB�I�ơA�h�ϥ� `SplitString` �i����ΡC
    vector<Token> tokens;
    Token token = GetNextToken();
    
    // �j��B�z�C��token�A�õ������A 
    while ( token.type != QUIT ) {
      tokens.push_back( token );
      token = GetNextToken() ;
    }  // end while

    tokens.push_back( token );
    return tokens;
  }  // end ProcessTokens()

  Type GetKeywordType( string keyword ) {
    if ( keyword == "int" ) {
      return INT;
    } // end if

    else if ( keyword == "float" ) {
      return FLOAT;
    } // end else if

    else if ( keyword == "char" ) {
      return CHAR;
    } // end else if

    else if ( keyword == "bool" ) {
      return BOOL;
    } // end else if

    else if ( keyword == "string" ) {
      return STRING;
    } // end else if

    else if ( keyword == "void" ) {
      return VOID;
    } // end else if

    else if ( keyword == "if" ) {
      return IF;
    } // end else if

    else if ( keyword == "else" ) {
      return ELSE;
    } // end else if

    else if ( keyword == "while" ) {
      return WHILE;
    } // end else if

    else if ( keyword == "do" ) {
      return DO;
    } // end else if

    else if ( keyword == "return" ) {
      return RETURN;
    } // end else if

    return ERROR;
  } // end GetKeywordType


  Token GetIdentOrKeyword( char nextChar ) {
    string id = "";
    
    while ( ( isalnum( nextChar ) || nextChar == '_' ) && nextChar != '\0' ) {
      id += AnyToString( nextChar );
      nextChar = GetNextChar();
    } // end while
    
    // �N�U�@�ӫD�ѧO�Ū��r�ũ�^
    PreviousChar();
    
    // �ˬd�O�_������r
    if ( id == "INT" || id == "FLOAT" || id == "CHAR" || 
      id == "BOOL" || id == "string" || id == "VOID" || 
      id == "IF" || id == "ELSE" || id == "WHILE" || 
      id == "DO" || id == "RETURN") {
      return CreatToken( id, mlineIndex, GetKeywordType( id ), LEXICALERROR );
    } // end if
    
    else {
      return CreatToken( id, mlineIndex, IDENTIFIER, LEXICALERROR );
    } // end else 
  } // end GetIdentOrKeyword

  // �ˬd���w���r�Ŧ�O�_�O�@�Ӧ��Ī��Ʀr�]��ƩίB�I�ơ^�C
  Token GetNUM( char nextChar ) {
    string tokenName;
    bool hasDot = false; 
    bool hasDigit = false;

    // �ˬd�C�Ӧr��
    while ( nextChar != '\0' && ( isdigit( nextChar ) || nextChar == '.' ) ) {
      if ( nextChar == '.' ) {
        if ( hasDot && hasDigit ) {
          PreviousChar();
          return CreatToken( tokenName, mlineIndex, CONSTANT, LEXICALERROR );
        } // end if
        
        else if ( hasDot && !hasDigit ) { // Error 
        	return CreatToken( ".", mlineIndex, ERROR, LEXICALERROR );
        } // end else if
        
        hasDot = true;
      } // end if 
      
      else if ( isdigit( nextChar ) ) hasDigit = true;
      
      tokenName += nextChar ;
      nextChar = GetNextChar();
    } // end while

    // �p�G�r�Ť��O�Ʀr���I�A�^�h�@�Ӧr�� 
    PreviousChar();
    if ( tokenName == "." ) return CreatToken( ".", mlineIndex, ERROR, LEXICALERROR );
    else return CreatToken( tokenName, mlineIndex, CONSTANT, LEXICALERROR );
  } // end GetNUM() 

  // ---------------------------------------GetToken--------------------------------------- 
  // ����U�@�� token�A�����ťաA�B�z�����A���ѧO���j�ũM���q�r�šC 
  Token CreatToken( string TokenName, int line, Type type, ErrorType error ) {
    Token token;
    token.tokenName = TokenName;
    token.line = line;
    token.type = type;
    token.error = error;
    
    return token;
  } // end CreatToken()
  Token GetNextToken() {
  // - **���L�ťաG** �۰ʸ��L�D���Ħr�šA�p�Ů�M����šC
  // - **�B�z�����G** �˴�������Ÿ��ɡA������᪺�r�Ū��ܦ���C
  // - **�ѧO���j�šG** ��J����j�ŮɡA��^���j�Ū� token�C
  // - **�ѧO���q�r�ŧǦC�G** ���D���j�Ū��r�ŧǦC�A�N��@���@�ӧ��㪺 token ��^�C
  
    char nextChar = GetNextNonWhiteSpaceChar();
    if ( nextChar == '/' ) {    // ���� 
      nextChar = GetNextChar();
            
      if ( nextChar == '/' ) {
        while ( nextChar != '\n' ) nextChar = GetNextChar() ;
        return GetNextToken();
      } // end if
            
      else {
        PreviousChar() ;
      } // end else
      
      PreviousChar();
    } // end if 
    
    
    if ( mlineIndex == minput.size() ) {
      return CreatToken( "\0", mlineIndex, QUIT, LEXICALERROR );
    } // end if 
        
    else if ( nextChar == '.' || isdigit( nextChar ) ) // NUM
      return GetNUM( nextChar ) ;
    
    else if ( nextChar == '\'' ) {  // char
      int charline = mlineIndex;
      string ch = AnyToString( GetNextChar() );

      nextChar = GetNextChar();
      if ( nextChar == '\'' ) return CreatToken( ch, charline, CONSTANT, LEXICALERROR );
      else return CreatToken( "'", charline, ERROR, LEXICALERROR );
    } // end else if 
    
    else if ( nextChar == '\"' ) {  // string
      string str = "";
      nextChar = GetNextChar();  
      while ( nextChar != '\"' && nextChar != '\0' ) {
        str += AnyToString( nextChar );
        nextChar = GetNextChar();
      } // end while
      
      //if ( nextChar == '\0' ) // error
      return CreatToken( str, mlineIndex, CONSTANT, LEXICALERROR );
    } // end else if
    
    else if ( isalpha( nextChar ) ) {  // IDENT
      return GetIdentOrKeyword( nextChar );
    } // end else if
    
    /*
    else if ( IsDelimiter( nextChar ) ) {
      return GetDelimiter( nextChar ) ;
    } // end else if
        
    else {
      string tokenValue = ReadRemainingToken();
      return nextChar + tokenValue;
    } // end else 
    */
  } // end GetNextToken()

  char GetNextNonWhiteSpaceChar() {
  // ����D�ťզr�šG ����Ƴs��Ū���r�šA������Ĥ@�ӫD�ťզr�Ŭ���C
  // �۰ʩ����ťաG ��Ʒ|�۰ʩ����Ҧ��ťզr�šA�]�A�Ů�B����šB�s��ŵ��C
  // ��^�ȡG ��^��쪺�Ĥ@�ӫD�ťզr�šC�p�G�F���J�������ӨS�����D�ťզr�šA�h��^�Ŧr��('\0')�C
  
    char nextChar = GetNextChar();
    
    while ( isspace( nextChar ) ) 
      nextChar = GetNextChar();

    return nextChar; 
  } // end GetNextNonWhiteSpaceChar()

  void PreviousChar() {
    if ( mcolumnIndex == 0 && mlineIndex >= 1 ) {
      mlineIndex-- ;
      mcolumnIndex = minput[mlineIndex].size() ;
    } // end if
    
    else mcolumnIndex-- ;
  } // end PreviousChar()

  char GetNextChar() {
  // GetNextChar() �q�@�ӵ��w����J�]�q�`�O�@�Ӧr�Ŧ�V�q�A�C�Ӥ����N���N�X���@��^������Ū���U�@�Ӧr�šC
  // ���ƹF���e�檺�����ɡA���|�۰����U�@�檺�}�l�C
  // �p�G��ƹF���J�奻�������A���|��^�@�ӪŦr��('\0')��ܨS����h���r�ťi�HŪ���C
  
    mlastmlineIndex = mlineIndex;  

    if ( mlineIndex < minput.size() ) {
      if ( mcolumnIndex < minput[mlineIndex].size() ) {
        char nextChar = minput[mlineIndex][mcolumnIndex++];
        return nextChar; 
      } // end if
      
      else {
        mlineIndex++;
        mcolumnIndex = 0;  
        return '\n';  
      } // end else    
    } // end if
  
    else return '\0';
  } // end GetNextChar()
};

#endif
