# include <iostream>
# include <stdio.h>
# include <cstdlib>
# include <vector>
# include <cctype> 
# include <string>
# include <map>
# include <cmath>

using namespace std;

using namespace std;
typedef char String200[200];
string gtestnum ;

vector<string> Read_lines() {
  vector<string> minput_lines ;
    
  
  string line;   
  char ch;
  bool isFirstLine = true;
  while ( cin.get( ch ) ) {               // 逐行讀取文件內容
    line += ch;
    
    if ( ch == '\n' ) {
      if ( isFirstLine ) {
        // 如果是第一行，則重置line並更改標誌狀態，不將其添加到minput_lines
        gtestnum = line ;
        line.clear();
        isFirstLine = false;
      } // end if
      
      else {
        minput_lines.push_back( line );
        line.clear();  // 清空line以用於下一行的讀取
      } // end else
    } // end if 
  } // end while

  // 檢查最後一行是否因為文件結束而沒有換行符結尾
  if ( !line.empty() ) {
    minput_lines.push_back( line );
  } // end if
  
  return minput_lines;
} // end Read_lines()

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
  MUL,  // '*'
  DIV,     // '/'
  MOD,   // '%'
  BIT_XOR,     // '^'
  GT,        // '>'
  LT,        // '<'
  GE,        // >= 
  LE,        // <= 
  EQ,        // == 
  NEQ,       // != 
  BIT_AND, // '&'
  BIT_OR,      // '|'
  ASSIGN,    // '='
  NOT, // '!'
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

string AnyToString( float num ) {
  String200 buffer;        
  sprintf( buffer, "%f", num );
  return string( buffer );     
} // end AnyToString()

string AnyToString( double num ) {
  String200 buffer;        
  sprintf( buffer, "%f", num );
  return string( buffer );     
} // end AnyToString()

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
  } // end GetKeywordType() 

  Token GetIdentOrKeyword( char nextChar ) {
    string id = "";
    
    while ( ( isalnum( nextChar ) || nextChar == '_' ) && nextChar != '\0' ) {
      id += AnyToString( nextChar );
      nextChar = GetNextChar();
    } // end while
    
    // 將下一個非識別符的字符放回
    PreviousChar();
    
    // 檢查是否為關鍵字
    if ( id.compare( "int" ) == 0 || id.compare( "float" ) == 0 || id.compare( "char" ) == 0 || 
         id.compare( "bool" ) == 0 || id.compare( "string" ) == 0 || id.compare( "void" ) == 0 || 
         id.compare( "if" ) == 0 || id.compare( "else" ) == 0 || id.compare( "while" ) == 0 || 
         id.compare( "do" ) == 0 || id.compare( "return" ) == 0 ) {

      return CreatToken( id, mlineIndex, GetKeywordType( id ), LEXICALERROR );
    } // end if
    
    else {
      return CreatToken( id, mlineIndex, IDENTIFIER, LEXICALERROR );
    } // end else 
  } // end GetIdentOrKeyword() 

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

  bool IsDelimiter( char ch ) {
    // 檢查是否為單字符分隔符或可能成為多字符操作符的第一個字符
    return ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
         ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' ||
         ch == '>' || ch == '<' || ch == '=' || ch == '!' || ch == '&' || ch == '|' ||
         ch == ';' || ch == ',' || ch == '?' || ch == ':';
  } // end IsDelimiter() 

  Token GetDelimiter( char ch ) {
    Type tokenType = ERROR;  // 預設為ERROR，在下面的條件語句中會被覆蓋
    
    string tokenValue = string( 1, ch );  // 初始設置token值為當前字符
    char nextChar;

    if ( ch == '(' ) tokenType = LPAREN;
    else if ( ch == ')' ) tokenType = RPAREN;
    else if ( ch == '[' ) tokenType = LBRACKET;
    else if ( ch == ']' ) tokenType = RBRACKET;
    else if ( ch == '{' ) tokenType = LBRACE;
    else if ( ch == '}' ) tokenType = RBRACE;
    
    else if ( ch == '+' ) {
      nextChar = GetNextChar();
    
      if ( nextChar == '=' ) {
        tokenType = PE;
        tokenValue += "=";
      } // end if 
      
      else if ( nextChar == '+' ) {
        tokenType = PP;
        tokenValue += "+";
      } // end else if
      
      else {
        PreviousChar();
        tokenType = PLUS;
      } // end else
    } // end else if
    
    else if ( ch == '-' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = ME;
        tokenValue += "=";
      } // end if
      
      else if ( nextChar == '-' ) {
        tokenType = MM;
        tokenValue += "-";
      } // end if
      
      else {
        PreviousChar();
        tokenType = MINUS;
      } // end else
    } // end else if
    
    else if ( ch == '*' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = TE;
        tokenValue += "=";
      } // end if 
      
      else {
        PreviousChar();
        tokenType = MUL;
      } // end else
    } // end else if
    
    else if ( ch == '/' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = DE;
        tokenValue += "=";
      } // end if 
      
      else {
        PreviousChar();
        tokenType = DIV;
      } // end else
    } // end else if
    
    else if ( ch == '%' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = RE;
        tokenValue += "=";
      } // end if 
      
      else {
        PreviousChar();
        tokenType = MOD;
      } // end else
    } // end else if
    
    else if ( ch == '=' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = EQ;
        tokenValue += "=";
      } // end if 
      
      else {
        PreviousChar();
        tokenType = ASSIGN;
      } // end else
    } // end else if
    
    else if ( ch == '!' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = NEQ;
        tokenValue += "=";
      } // end if
      
      else {
        PreviousChar();
        tokenType = NOT;
      } // end else
    } // end else if
    
    else if ( ch == '<' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = LE;
        tokenValue += "=";
      } // end if
      
      else if ( nextChar == '<' ) {
        tokenType = LS;
        tokenValue += "<";
      } // end else if 
      
      else {
        PreviousChar();
        tokenType = LT;
      } // end else
    } // end else if
    
    else if ( ch == '>' ) {
      nextChar = GetNextChar();
      if ( nextChar == '=' ) {
        tokenType = GE;
        tokenValue += "=";
      } // end if 
      
      else if ( nextChar == '>' ) {
        tokenType = RS;
        tokenValue += ">";
      } // end else if
      
      else {
        PreviousChar();
        tokenType = GT;
      } // end else
    } // end else if
    
    else if ( ch == '&' ) {
      nextChar = GetNextChar();
      if ( nextChar == '&' ) {
        tokenType = AND;
        tokenValue += "&";
      } // end if 
      
      else {
        PreviousChar();
        tokenType = BIT_AND;
      } // end else
    } // end else if
    
    else if ( ch == '|' ) {
      nextChar = GetNextChar();
      if ( nextChar == '|' ) {
        tokenType = OR;
        tokenValue += "|";
      } // end if 
      
      else {
        PreviousChar();
        tokenType = BIT_OR;
      } // end else
    } // end else if
    
    else if ( ch == ';' ) tokenType = SEMICOLON;
    else if ( ch == ',' ) tokenType = COMMA;
    else if ( ch == '?' ) tokenType = QUESTION;
    else if ( ch == ':' ) tokenType = COLON;
    else if ( ch == '^' ) tokenType = BIT_XOR;
    // 返回創建的Token
    return CreatToken( tokenValue, mlineIndex, tokenType, OTHERERROR );
  } // end GetDelimiter()

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
      
      return GetDelimiter( '/' ) ;
    } // end if 
    
    if ( mlineIndex >= minput.size() ) {
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
      
      if ( nextChar == '\0' ) 
        return CreatToken( "string missing terminating '\"", mlineIndex, ERROR, SYNTACTICALERROR );

      return CreatToken( str, mlineIndex, CONSTANT, LEXICALERROR );
    } // end else if
    
    else if ( isalpha( nextChar ) ) {  // IDENT
      return GetIdentOrKeyword( nextChar );
    } // end else if
    
    
    else if ( IsDelimiter( nextChar ) ) {
      return GetDelimiter( nextChar ) ;
    } // end else if
        
    
    else {
      string temp = string( 1, nextChar ) ;
      return CreatToken( temp, mlineIndex, ERROR, LEXICALERROR );
    } // end else 
    
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


int main() {
  vector<string> minput = Read_lines();
  Tokenizer tokenizer( minput );

  cout << "Our-C running ..." << endl ;
  
  // Parser parse( tokenizer ) ;
  // parse.Parse() ;
  cout << "Our-C exited ..." << endl ; 
  
} // end main()
