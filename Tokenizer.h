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

// 定義不同類型的標記或符號
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
  LEXICALERROR,      // 當詞法分析器遇到一個它無法識別為任何有效符號的字符時報告的錯誤。
  SYNTACTICALERROR,  // 當語法分析器在分析符號序列時遇到一個不符合當前語法規則的符號時報告的錯誤。
  SEMANTICERROR,     // 當語義分析器在檢查程式的語義時發現使用了未定義的標識符時報告的錯誤。
  OTHERERROR         // 表示其他錯誤
};

// 定義標記結構體，包含標記名稱、類型、所在行數和錯誤信息
struct Token {
  string tokenName;  // 標記名稱
  Type type;         // 標記類型
  int line;          // 標記所在的行數
  ErrorType error;       // 關聯的錯誤信息
};

string AnyToString( int num ) {
  String200 buffer;        
  sprintf( buffer, "%d", num );  
  return string( buffer );     
} // end AnyToString()

// 定義一個函數，將浮點數（float）轉換為字符串。
string AnyToString( float num ) {
  String200 buffer;        
  sprintf( buffer, "%f", num );
  return string( buffer );     
} // end AnyToString()

// 定義一個函數，將雙精度浮點數（double）轉換為字符串。
string AnyToString( double num ) {
  String200 buffer;        
  sprintf( buffer, "%f", num );
  return string( buffer );     
} // end AnyToString()

// 定義一個函數，將字符轉換為字符串。
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

  // 處理和分析來自輸入的 tokens，賦予每個 token 相應的類型。
  vector<Token> ProcessTokens() {
  // - **Token 處理：** 逐一讀取並分析 tokens，直到遇到終止符。
  // - **類型分析：** 使用 `AnalyzeToken` 函數來確定每個 token 的類型。
  // - **錯誤處理：** 若 token 類型為 ERROR，進行額外的檢查和處理。
  // - **多浮點數處理：** 特殊情況下，如 token 為多個浮點數，則使用 `SplitString` 進行分割。
    vector<Token> tokens;
    Token token = GetNextToken();
    
    // 迴圈處理每個token，並給予型態 
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
    
    // 將下一個非識別符的字符放回
    PreviousChar();
    
    // 檢查是否為關鍵字
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

  // 檢查給定的字符串是否是一個有效的數字（整數或浮點數）。
  Token GetNUM( char nextChar ) {
    string tokenName;
    bool hasDot = false; 
    bool hasDigit = false;

    // 檢查每個字符
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

    // 如果字符不是數字或點，回退一個字符 
    PreviousChar();
    if ( tokenName == "." ) return CreatToken( ".", mlineIndex, ERROR, LEXICALERROR );
    else return CreatToken( tokenName, mlineIndex, CONSTANT, LEXICALERROR );
  } // end GetNUM() 

  // ---------------------------------------GetToken--------------------------------------- 
  // 獲取下一個 token，忽略空白，處理註釋，並識別分隔符和普通字符。 
  Token CreatToken( string TokenName, int line, Type type, ErrorType error ) {
    Token token;
    token.tokenName = TokenName;
    token.line = line;
    token.type = type;
    token.error = error;
    
    return token;
  } // end CreatToken()
  Token GetNextToken() {
  // - **跳過空白：** 自動跳過非有效字符，如空格和換行符。
  // - **處理註釋：** 檢測到註釋符號時，忽略其後的字符直至行尾。
  // - **識別分隔符：** 當遇到分隔符時，返回分隔符的 token。
  // - **識別普通字符序列：** 對於非分隔符的字符序列，將其作為一個完整的 token 返回。
  
    char nextChar = GetNextNonWhiteSpaceChar();
    if ( nextChar == '/' ) {    // 註解 
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
  // 獲取非空白字符： 此函數連續讀取字符，直到找到第一個非空白字符為止。
  // 自動忽略空白： 函數會自動忽略所有空白字符，包括空格、換行符、製表符等。
  // 返回值： 返回找到的第一個非空白字符。如果達到輸入的末尾而沒有找到非空白字符，則返回空字符('\0')。
  
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
  // GetNextChar() 從一個給定的輸入（通常是一個字符串向量，每個元素代表源代碼的一行）中順序讀取下一個字符。
  // 當函數達到當前行的末尾時，它會自動轉到下一行的開始。
  // 如果函數達到輸入文本的末尾，它會返回一個空字符('\0')表示沒有更多的字符可以讀取。
  
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
