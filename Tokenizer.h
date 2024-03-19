#ifndef Tokenizer_H
#define Tokenizer_H

# include <iostream>
# include <stdio.h>
# include <vector>
# include <cctype> // �B�z�r��  
# include <string>
// #include <fstream>

using namespace std;
typedef char String200[200];

// �w�q���P�������аO�βŸ�
enum Type {
  NONE,           // ��ܨS�������Υ�������
  SEMICOLON,      // ��ܤ����Ÿ� ';'
  ASSIGN,         // ��ܽ�ȲŸ� ':='
  LESSEQUAL,      // ��ܤp�󵥩�Ÿ� '<='
  GREATEREQUAL,   // ��ܤj�󵥩�Ÿ� '>='
  LESS,           // ��ܤp��Ÿ� '<'
  GREATER,        // ��ܤj��Ÿ� '>'
  NOTEQUAL,       // ��ܤ�����Ÿ� '<>'
  EQUAL,          // ��ܵ���Ÿ� '='
  MINUS,          // ��ܴ '-'
  PLUS,           // ��ܥ[�� '+'
  DIVIDE,         // ��ܰ��� '/'
  MULTIPLY,       // ��ܭ��� '*'
  RPAREN,         // ��ܥk�A�� ')'
  LPAREN,         // ��ܥ��A�� '('
  IDENT,          // ��ܼ��Ѳ�
  INT,            // ��ܾ��
  FLOAT,          // ��ܯB�I��
  BOOL,           // ��ܥ�������
  SIGN,           // ��ܲŸ��]�i��Ω󥼨��X�i�^
  ERROR,          // ��ܿ��~
  QUIT            // ��ܰh�X
};

// �w�q���k���R���q�����~����
enum ErrorType {
  LEXICALERROR,      // ����k���R���J��@�ӥ��L�k�ѧO�����󦳮ĲŸ����r�Ůɳ��i�����~�C
  SYNTACTICALERROR,  // ��y�k���R���b���R�Ÿ��ǦC�ɹJ��@�Ӥ��ŦX��e�y�k�W�h���Ÿ��ɳ��i�����~�C
  SEMANTICERROR,     // ��y�q���R���b�ˬd�{�����y�q�ɵo�{�ϥΤF���w�q�����ѲŮɳ��i�����~�C
  OTHERERROR         // ��ܨ�L���~
};

// �w�q���~���c��A�]�t���~�����M���~��
struct Error {
  ErrorType type;    // ���~����
  string errorValue; // ���~�H���έ�
};

// �w�q�аO���c��A�]�t�аO�W�١B�����B�Ҧb��ƩM���~�H��
struct Token {
  string tokenName;  // �аO�W��
  Type type;         // �аO����
  int line;          // �аO�Ҧb�����
  Error error;       // ���p�����~�H��
};

// �w�q�@�Ө�ơA�Ω�Ыؤ@�ӷs��Token��ҡC
Token CreateToken( string name, Type type, Error error ) {
  Token token;               // �Ыؤ@��Token�������ܶq�C
  token.tokenName = name;    // �]�mToken���W�١C
  token.type = type;         // �]�mToken�������C
  token.error = error;       // �]�mToken�����~�H���C
  return token;              // ��^��Token��ҡC
} // end CreateToken()

// �w�q�@�Ө�ơA�N����ഫ���r�Ŧ�C
string AnyToString( int num ) {
  String200 buffer;        
  sprintf( buffer, "%d", num );  
  return string( buffer );     
} // end AnyToString()

// �w�q�@�Ө�ơA�N�B�I�ơ]float�^�ഫ���r�Ŧ�C
string AnyToString( float num ) {
  String200 buffer;        
  sprintf( buffer, "%.4f", num );
  return string( buffer );     
} // end AnyToString()

// �w�q�@�Ө�ơA�N����ׯB�I�ơ]double�^�ഫ���r�Ŧ�C
string AnyToString( double num ) {
  String200 buffer;        
  sprintf( buffer, "%.4f", num );
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
  vector<string> input;
  int lineIndex;
  int columnIndex;
  int lastlineIndex;
public:
    Tokenizer( vector<string> input) : input(input), lineIndex(0), columnIndex(0) {}

  // �B�z�M���R�Ӧۿ�J�� tokens�A�ᤩ�C�� token �����������C
  vector<Token> processTokens() {
  // - **Token �B�z�G** �v�@Ū���ä��R tokens�A����J��פ�šC
  // - **�������R�G** �ϥ� `analyzeToken` ��ƨӽT�w�C�� token �������C
  // - **���~�B�z�G** �Y token ������ ERROR�A�i���B�~���ˬd�M�B�z�C
  // - **�h�B�I�ƳB�z�G** �S���p�U�A�p token ���h�ӯB�I�ơA�h�ϥ� `splitString` �i����ΡC
    vector<Token> tokens;
    string tokenName = getNextToken() ;
        
    // �j��B�z�C��token�A�õ������A 
    while ( tokenName != "\0" ) {
      Token token;
      token.tokenName = tokenName;
      token.line = lastlineIndex ;
      
      // ���]����ƪ�^token�����A
      token.type = analyzeToken( tokenName );
      if ( token.type == ERROR ) {
      // �B�z�ҥ~�A�|�������㪺token �Ҧp:floatfloat 1.23.23

        if ( isMultiFloat( tokenName ) ) {
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
          token.error = checkError( tokenName );
        } // end else
      } // end if
	  
      tokens.push_back(token);
      tokenName = getNextToken() ;
    }  // end while

    return tokens;
  }  // end processTokens()

  // �ˬd���w�r�Ŧꪺ���~�A��^�]�t���~�����M���~�Ȫ� Error ���c��C
  Error checkError( string str ) {
  // - **���Ѳſ��~�ˬd�G** �p�G�r�Ŧ�O�H�r���}�Y�A�ˬd��O�_�ŦX���ѲŪ��W�h�C
  // - **�Ʀr���~�ˬd�G** �p�G�r�Ŧꤣ�O���ѲšA�h�ˬd��O�_���X�k���Ʀr��ܡC
  // - **���~�H���G** ��o�{�D�k�r�ŮɡA�N�Ӧr�ũM���~�����s�x�b `Error` ���c�餤�C
    Error error ;
    int i = 0 ;
    
    if ( isalpha( str[0] ) ) {  // �OIDENT�����~ 	
      while ( i < str.length() && ( isalnum( str[i] ) || str[i] == '_' ) ) {      
        i++ ;
      } // end while

      error.errorValue = str[i] ;
      error.type = LEXICALERROR;
      return error;
    } // end if
        
    else {
      bool hasDot = false; // �аO�O�_���p���I
      bool hasDigit = false; // �аO�O�_���Ʀr

      // �ˬd�C�Ӧr��  
      for ( size_t i = 0; i < str.length(); i++ ) {
        char c = str[i];
        if ( c == '.' && !hasDot ) {
          hasDot = true;
        } // end if
	      
        else if ( c == '+' || c == '-' ) {
          // �p�G�w�g���Ʀr�X�{�L�A�B���t�����b�Ĥ@��A�h��^ false
          if ( hasDigit || &c != &str[0] ) {
            error.errorValue = AnyToString(c);
            error.type = LEXICALERROR;
            return error;
          } // end if
        } // end else if
                
        else if ( isdigit( c ) ) {
          hasDigit = true;
        } // end else if
	      
        else if ( !isalpha( c ) ) {
          error.errorValue = str[i] ;
          error.type = LEXICALERROR;
          return error;
        } // end else if
	      
        else {
          error.errorValue = error.errorValue + str[i] ;
          error.type = LEXICALERROR;
        } // end else
      } // end for
            
      return error;
    } // end else
  } // end checkError()

  // �ھڵ��w�� token �r��Ӥ��R�ê�^������ Type�C
  Type analyzeToken( string token ) {
  // - **���ѲŧP�_�G** �p�G token �O���ѲšA��^ `IDENT`�C
  // - **�Ʀr�����P�_�G** �p�G token �O�Ʀr�A�i�@�B�P�_�O��� (`INT`) �٬O�B�I�� (`FLOAT`)�C
  // - **�B��ũM���j�ŧP�_�G** �ھ� token ������Ȩ��ѧO�ê�^�����������A�p `PLUS`�B`MINUS`�B`MULTIPLY` ���C
  // - **���~�B�z�G** �p�G token ���ǰt����w�������A��^ `ERROR`�C
    	
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

  // �N�r�Ŧ�ھڲĤG���I('.')���Φ��ⳡ���C
  void splitString( string str, string& left, string& right) {
  // - **���μзǡG** �ھڲĤG�ӥX�{���I ('.') �i����ΡC
  // - **�����r�Ŧ�G** �ĤG���I���e���Ҧ��r�š]�]�t�Ĥ@���I�^�C
  // - **�k���r�Ŧ�G** �ĤG���I���᪺�r�šA����J��ĤT���I�ΫD�Ʀr�r�šC
  // - **�߱����G** �p�G�b�ĤG���I����X�{�D�Ʀr�r�ũβĤT���I�A�h�߱�o���᪺�Ҧ��r�šC
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
    
  // �ˬd���w���r�Ŧ�O�_�����Ī��ƾǲŸ��G�[�� ("+") �δ ("-")�C 
  bool isSIGN( string str ) {
    if ( str.compare( "+" ) == 0 || str.compare( "-" ) == 0 )
      return true ;
    else {
      return false ;
    } // end else
  } // end isSIGN

  // �ˬd���w���r�Ŧ�O�_�O�@�Ӧ��Ī���ơC 
  bool isInt( string str ) {
    if ( str.empty() ) return false;

    for ( size_t i = 0 ; i < str.length() ; i++ ) {
      char c = str[i];
      if ( !isdigit( c ) ) {
        return false;
      } // end if
    } // end for

    return true;
  } // end isInt()

  // �ˬd���w���r�Ŧ�O�_�O�@�Ӧ��Ī��Ʀr�]��ƩίB�I�ơ^�C
  bool isNUM( string str ) {
  // - **�Ŧr�Ŧ��ˬd�G** �Ŧr�Ŧꤣ�������Ī��Ʀr�C
  // - **�p���I�ˬd�G** �r�Ŧꤤ�i�H�]�t�̦h�@�Ӥp���I�Ӫ�ܯB�I�ơC
  // - **�Ʀr�ˬd�G** �r�Ŧ����ܤ֥]�t�@�ӼƦr�C
  // - **�D�Ʀr�r���ˬd�G** �p�G�r�Ŧ�]�t���Ʀr�M�p���I���~������r�šA�h�Ӧr�Ŧꤣ�������Ī��Ʀr�C 

    if ( str.empty() ) return false;

    bool hasDot = false; 
    bool hasDigit = false;

    // �ˬd�C�Ӧr��
    for ( size_t i = 0 ; i < str.length() ; i++ ) {
      char c = str[i];
      if ( c == '.' ) {
        if ( hasDot ) {
          // �w�g���@�Ӥp���I�A�o���O�X�k���B�I��
          return false;
        } // end if
            
            hasDot = true;
      } // end if 
        
      else if ( isdigit( c ) ) hasDigit = true;
       
      else {
        return false;
      } // end else
    } // end for

    return hasDigit;
  } // end isNUM() 
 
  // �ˬd���w���r�Ŧ�O�_��ܦh�ӯB�I�ơC 
  bool isMultiFloat( string str ) {
  // - **�Ʀr�M�I���έp�G** �έp�r�Ŧꤤ���Ʀr�M�p���I�ƶq�C
  // - **�����ˬd�G** �T�O�r�Ŧ꥽�����O�p���I�C
  // - **�h�ӯB�I���ˬd�G** �ھڼƦr�M�p���I���ƶq�P�_�O�_���h�ӯB�I�ơC 
  
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
        i-- ; 
      } // end if
	  
      else if ( str[i] == '.' ) dot++ ;
	  
      lastDigit = false ;
      i++ ;
        	  
      if ( ( dot == digit - 1 ) && digit > 2 ) return true ;
    } // end while
        
    if ( digit == 2 && dot == 2 ) return true ;
    else return false ;
  } // end isMultiFloat

  // �ˬd���w���r�Ŧ�O�_�O�@�ӦX�k�����ѲšC 
  bool isIDENT( string str ) {
  // - **�D���ˬd�G** �Ŧr�ꤣ�����X�k�����ѲšC
  // - **���r���ˬd�G** ���ѲŪ��Ĥ@�Ӧr�ť����O�r���C
  // - **����r���ˬd�G** ���ѲŪ�����r�ťi�H�O�r���B�Ʀr�Ω��u�C 
  
    if ( str.empty() )// �Ŧr�ꤣ�O�X�k�����Ѳ�
      return false;

    if ( !isalpha( str[0] ) )  // �Ĥ@�Ӧr�������O�r��
      return false;

    for ( size_t i = 1; i < str.length(); i++ ) {
      if ( !isalnum( str[i] ) && str[i] != '_' )  // ��l�r�������O�Ʀr�B�r���Ω��u
        return false;
    } // end for

    return true;
  } // end isIDENT

  // �ˬd���w���r�ŬO�_�����j�šC 
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
  } // end isDelimiter() 
    
  // �ھڵ��w���r���ѧO�ê�^���������j�ũξާ@�šC 
  string getDelimiter( char ch ) {
  // - **��r�ųB�z�G** ������^��r�Ť��j�ũξާ@�šC
  // - **���r�ųB�z�G** ��� `<`�B`>` �� `:` �o�˪��r�šA��Ʒ|�ˬd�U�@�Ӧr�ťH�T�w�O�_�Φ����r�žާ@�šC
  // - **�^�h����G** �p�G�T�w�r�Ť��Φ����r�žާ@�šA��Ʒ|�NŪ�����w�^�h�@��C 
  
    if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
         ch == '='  || ch == ';' ) {
         return string( 1, ch );
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
      else return ":";
    } // end else if
        
    else return string( 1, ch );
  } // end getDelimiter()
// ---------------------------------------GetToken--------------------------------------- 
  // ����U�@�� token�A�����ťաA�B�z�����A���ѧO���j�ũM���q�r�šC  
  string getNextToken() {
  // - **���L�ťաG** �۰ʸ��L�D���Ħr�šA�p�Ů�M����šC
  // - **�B�z�����G** �˴�������Ÿ��ɡA������᪺�r�Ū��ܦ���C
  // - **�ѧO���j�šG** ��J����j�ŮɡA��^���j�Ū� token�C
  // - **�ѧO���q�r�ŧǦC�G** ���D���j�Ū��r�ŧǦC�A�N��@���@�ӧ��㪺 token ��^�C
  
    char nextChar = getNextNonWhiteSpaceChar();
    if ( lineIndex == input.size() ) {
      return "\0";
    } // end if 
        
    else if ( nextChar == '/' ) {
      nextChar = getNextChar();
            
      if ( nextChar == '/' ) {
        while ( nextChar != '\n' ) nextChar = getNextChar() ;
        return getNextToken();
      } // end if
            
      else {
        columnIndex-- ;
        return "/" ;
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

  // Ū����e��m���᪺�s��r�šA����J��Ů�Τ��j�šA�Φ��@��token�C
  string readRemainingToken() {
  // **�s��Ū���G** �s��Ū���r�Ū���J��ťթΤ��j�šC
  // **�����B�z�G** �J���ӳs�򪺱׽u (`//`) �|�N����������A�������촫��Ū��Ҧ��r�šC
  // **Token �ֿn�G** �ֿnŪ�����r�šA�Φ� token �r��C
  
    string tokenValue = "";
    char nextChar = getNextChar();
    bool hasDot = false;
    
    while ( !isspace( nextChar ) && !isDelimiter( nextChar ) ) { 
    
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

  // ����U�@�ӫD�ťզr�šC
  char getNextNonWhiteSpaceChar() {
  // ����D�ťզr�šG ����Ƴs��Ū���r�šA������Ĥ@�ӫD�ťզr�Ŭ���C
  // �۰ʩ����ťաG ��Ʒ|�۰ʩ����Ҧ��ťզr�šA�]�A�Ů�B����šB�s��ŵ��C
  // ��^�ȡG ��^��쪺�Ĥ@�ӫD�ťզr�šC�p�G�F���J�������ӨS�����D�ťզr�šA�h��^�Ŧr��('\0')�C
  
    char nextChar = getNextChar();
    
    while ( isspace( nextChar ) ) 
      nextChar = getNextChar();

    return nextChar; 
  } // end getNextNonWhiteSpaceChar()

  // �w�q�@�Ө�ƨ�����U�@�Ӧr�űq��J�奻���C
  char getNextChar() {
  // getNextChar() �q�@�ӵ��w����J�]�q�`�O�@�Ӧr�Ŧ�V�q�A�C�Ӥ����N���N�X���@��^������Ū���U�@�Ӧr�šC
  // ���ƹF���e�檺�����ɡA���|�۰����U�@�檺�}�l�C
  // �p�G��ƹF���J�奻�������A���|��^�@�ӪŦr��('\0')��ܨS����h���r�ťi�HŪ���C
  
    lastlineIndex = lineIndex;  

    if ( lineIndex < input.size() ) {
      if ( columnIndex < input[lineIndex].size() ) {
        char nextChar = input[lineIndex][columnIndex++];
        return nextChar; 
      } // end if
      
      else {
        lineIndex++;
        columnIndex = 0;  
        return '\n';  
      } // end else    
    } // end if
  
    else return '\0';
  } // end getNextChar()
};

#endif
