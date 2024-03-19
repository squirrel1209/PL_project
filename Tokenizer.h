#ifndef Tokenizer_H
#define Tokenizer_H

# include <iostream>
# include <stdio.h>
# include <vector>
# include <cctype> // 處理字符  
# include <string>
// #include <fstream>

using namespace std;
typedef char String200[200];

// 定義不同類型的標記或符號
enum Type {
  NONE,           // 表示沒有類型或未知類型
  SEMICOLON,      // 表示分號符號 ';'
  ASSIGN,         // 表示賦值符號 ':='
  LESSEQUAL,      // 表示小於等於符號 '<='
  GREATEREQUAL,   // 表示大於等於符號 '>='
  LESS,           // 表示小於符號 '<'
  GREATER,        // 表示大於符號 '>'
  NOTEQUAL,       // 表示不等於符號 '<>'
  EQUAL,          // 表示等於符號 '='
  MINUS,          // 表示減號 '-'
  PLUS,           // 表示加號 '+'
  DIVIDE,         // 表示除號 '/'
  MULTIPLY,       // 表示乘號 '*'
  RPAREN,         // 表示右括號 ')'
  LPAREN,         // 表示左括號 '('
  IDENT,          // 表示標識符
  INT,            // 表示整數
  FLOAT,          // 表示浮點數
  BOOL,           // 表示布爾類型
  SIGN,           // 表示符號（可能用於未來擴展）
  ERROR,          // 表示錯誤
  QUIT            // 表示退出
};

// 定義詞法分析階段的錯誤類型
enum ErrorType {
  LEXICALERROR,      // 當詞法分析器遇到一個它無法識別為任何有效符號的字符時報告的錯誤。
  SYNTACTICALERROR,  // 當語法分析器在分析符號序列時遇到一個不符合當前語法規則的符號時報告的錯誤。
  SEMANTICERROR,     // 當語義分析器在檢查程式的語義時發現使用了未定義的標識符時報告的錯誤。
  OTHERERROR         // 表示其他錯誤
};

// 定義錯誤結構體，包含錯誤類型和錯誤值
struct Error {
  ErrorType type;    // 錯誤類型
  string errorValue; // 錯誤信息或值
};

// 定義標記結構體，包含標記名稱、類型、所在行數和錯誤信息
struct Token {
  string tokenName;  // 標記名稱
  Type type;         // 標記類型
  int line;          // 標記所在的行數
  Error error;       // 關聯的錯誤信息
};

// 定義一個函數，用於創建一個新的Token實例。
Token CreateToken( string name, Type type, Error error ) {
  Token token;               // 創建一個Token類型的變量。
  token.tokenName = name;    // 設置Token的名稱。
  token.type = type;         // 設置Token的類型。
  token.error = error;       // 設置Token的錯誤信息。
  return token;              // 返回該Token實例。
} // end CreateToken()

// 定義一個函數，將整數轉換為字符串。
string AnyToString( int num ) {
  String200 buffer;        
  sprintf( buffer, "%d", num );  
  return string( buffer );     
} // end AnyToString()

// 定義一個函數，將浮點數（float）轉換為字符串。
string AnyToString( float num ) {
  String200 buffer;        
  sprintf( buffer, "%.4f", num );
  return string( buffer );     
} // end AnyToString()

// 定義一個函數，將雙精度浮點數（double）轉換為字符串。
string AnyToString( double num ) {
  String200 buffer;        
  sprintf( buffer, "%.4f", num );
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
  vector<string> input;
  int lineIndex;
  int columnIndex;
  int lastlineIndex;
public:
    Tokenizer( vector<string> input) : input(input), lineIndex(0), columnIndex(0) {}

  // 處理和分析來自輸入的 tokens，賦予每個 token 相應的類型。
  vector<Token> processTokens() {
  // - **Token 處理：** 逐一讀取並分析 tokens，直到遇到終止符。
  // - **類型分析：** 使用 `analyzeToken` 函數來確定每個 token 的類型。
  // - **錯誤處理：** 若 token 類型為 ERROR，進行額外的檢查和處理。
  // - **多浮點數處理：** 特殊情況下，如 token 為多個浮點數，則使用 `splitString` 進行分割。
    vector<Token> tokens;
    string tokenName = getNextToken() ;
        
    // 迴圈處理每個token，並給予型態 
    while ( tokenName != "\0" ) {
      Token token;
      token.tokenName = tokenName;
      token.line = lastlineIndex ;
      
      // 假設此函數返回token的型態
      token.type = analyzeToken( tokenName );
      if ( token.type == ERROR ) {
      // 處理例外，尚未切完整的token 例如:floatfloat 1.23.23

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
          // 錯誤處理 
          token.error = checkError( tokenName );
        } // end else
      } // end if
	  
      tokens.push_back(token);
      tokenName = getNextToken() ;
    }  // end while

    return tokens;
  }  // end processTokens()

  // 檢查給定字符串的錯誤，返回包含錯誤類型和錯誤值的 Error 結構體。
  Error checkError( string str ) {
  // - **標識符錯誤檢查：** 如果字符串是以字母開頭，檢查其是否符合標識符的規則。
  // - **數字錯誤檢查：** 如果字符串不是標識符，則檢查其是否為合法的數字表示。
  // - **錯誤信息：** 當發現非法字符時，將該字符和錯誤類型存儲在 `Error` 結構體中。
    Error error ;
    int i = 0 ;
    
    if ( isalpha( str[0] ) ) {  // 是IDENT的錯誤 	
      while ( i < str.length() && ( isalnum( str[i] ) || str[i] == '_' ) ) {      
        i++ ;
      } // end while

      error.errorValue = str[i] ;
      error.type = LEXICALERROR;
      return error;
    } // end if
        
    else {
      bool hasDot = false; // 標記是否有小數點
      bool hasDigit = false; // 標記是否有數字

      // 檢查每個字符  
      for ( size_t i = 0; i < str.length(); i++ ) {
        char c = str[i];
        if ( c == '.' && !hasDot ) {
          hasDot = true;
        } // end if
	      
        else if ( c == '+' || c == '-' ) {
          // 如果已經有數字出現過，且正負號不在第一位，則返回 false
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

  // 根據給定的 token 字串來分析並返回對應的 Type。
  Type analyzeToken( string token ) {
  // - **標識符判斷：** 如果 token 是標識符，返回 `IDENT`。
  // - **數字類型判斷：** 如果 token 是數字，進一步判斷是整數 (`INT`) 還是浮點數 (`FLOAT`)。
  // - **運算符和分隔符判斷：** 根據 token 的具體值來識別並返回相應的類型，如 `PLUS`、`MINUS`、`MULTIPLY` 等。
  // - **錯誤處理：** 如果 token 不匹配任何已知類型，返回 `ERROR`。
    	
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

  // 將字符串根據第二個點('.')分割成兩部分。
  void splitString( string str, string& left, string& right) {
  // - **分割標準：** 根據第二個出現的點 ('.') 進行分割。
  // - **左側字符串：** 第二個點之前的所有字符（包含第一個點）。
  // - **右側字符串：** 第二個點之後的字符，直到遇到第三個點或非數字字符。
  // - **拋棄條件：** 如果在第二個點之後出現非數字字符或第三個點，則拋棄這之後的所有字符。
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
    
  // 檢查給定的字符串是否為有效的數學符號：加號 ("+") 或減號 ("-")。 
  bool isSIGN( string str ) {
    if ( str.compare( "+" ) == 0 || str.compare( "-" ) == 0 )
      return true ;
    else {
      return false ;
    } // end else
  } // end isSIGN

  // 檢查給定的字符串是否是一個有效的整數。 
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

  // 檢查給定的字符串是否是一個有效的數字（整數或浮點數）。
  bool isNUM( string str ) {
  // - **空字符串檢查：** 空字符串不視為有效的數字。
  // - **小數點檢查：** 字符串中可以包含最多一個小數點來表示浮點數。
  // - **數字檢查：** 字符串應至少包含一個數字。
  // - **非數字字符檢查：** 如果字符串包含除數字和小數點之外的任何字符，則該字符串不視為有效的數字。 

    if ( str.empty() ) return false;

    bool hasDot = false; 
    bool hasDigit = false;

    // 檢查每個字符
    for ( size_t i = 0 ; i < str.length() ; i++ ) {
      char c = str[i];
      if ( c == '.' ) {
        if ( hasDot ) {
          // 已經有一個小數點，這不是合法的浮點數
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
 
  // 檢查給定的字符串是否表示多個浮點數。 
  bool isMultiFloat( string str ) {
  // - **數字和點的統計：** 統計字符串中的數字和小數點數量。
  // - **末尾檢查：** 確保字符串末尾不是小數點。
  // - **多個浮點數檢查：** 根據數字和小數點的數量判斷是否為多個浮點數。 
  
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

  // 檢查給定的字符串是否是一個合法的標識符。 
  bool isIDENT( string str ) {
  // - **非空檢查：** 空字串不視為合法的標識符。
  // - **首字符檢查：** 標識符的第一個字符必須是字母。
  // - **後續字符檢查：** 標識符的後續字符可以是字母、數字或底線。 
  
    if ( str.empty() )// 空字串不是合法的標識符
      return false;

    if ( !isalpha( str[0] ) )  // 第一個字元必須是字母
      return false;

    for ( size_t i = 1; i < str.length(); i++ ) {
      if ( !isalnum( str[i] ) && str[i] != '_' )  // 其餘字元必須是數字、字母或底線
        return false;
    } // end for

    return true;
  } // end isIDENT

  // 檢查給定的字符是否為分隔符。 
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
    
  // 根據給定的字符識別並返回對應的分隔符或操作符。 
  string getDelimiter( char ch ) {
  // - **單字符處理：** 直接返回單字符分隔符或操作符。
  // - **雙字符處理：** 對於像 `<`、`>` 或 `:` 這樣的字符，函數會檢查下一個字符以確定是否形成雙字符操作符。
  // - **回退機制：** 如果確定字符不形成雙字符操作符，函數會將讀取指針回退一位。 
  
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
  // 獲取下一個 token，忽略空白，處理註釋，並識別分隔符和普通字符。  
  string getNextToken() {
  // - **跳過空白：** 自動跳過非有效字符，如空格和換行符。
  // - **處理註釋：** 檢測到註釋符號時，忽略其後的字符直至行尾。
  // - **識別分隔符：** 當遇到分隔符時，返回分隔符的 token。
  // - **識別普通字符序列：** 對於非分隔符的字符序列，將其作為一個完整的 token 返回。
  
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

  // 讀取當前位置之後的連續字符，直到遇到空格或分隔符，形成一個token。
  string readRemainingToken() {
  // **連續讀取：** 連續讀取字符直到遇到空白或分隔符。
  // **註釋處理：** 遇到兩個連續的斜線 (`//`) 會將其視為註釋，忽略直到換行符的所有字符。
  // **Token 累積：** 累積讀取的字符，形成 token 字串。
  
    string tokenValue = "";
    char nextChar = getNextChar();
    bool hasDot = false;
    
    while ( !isspace( nextChar ) && !isDelimiter( nextChar ) ) { 
    
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

  // 獲取下一個非空白字符。
  char getNextNonWhiteSpaceChar() {
  // 獲取非空白字符： 此函數連續讀取字符，直到找到第一個非空白字符為止。
  // 自動忽略空白： 函數會自動忽略所有空白字符，包括空格、換行符、製表符等。
  // 返回值： 返回找到的第一個非空白字符。如果達到輸入的末尾而沒有找到非空白字符，則返回空字符('\0')。
  
    char nextChar = getNextChar();
    
    while ( isspace( nextChar ) ) 
      nextChar = getNextChar();

    return nextChar; 
  } // end getNextNonWhiteSpaceChar()

  // 定義一個函數來獲取下一個字符從輸入文本中。
  char getNextChar() {
  // getNextChar() 從一個給定的輸入（通常是一個字符串向量，每個元素代表源代碼的一行）中順序讀取下一個字符。
  // 當函數達到當前行的末尾時，它會自動轉到下一行的開始。
  // 如果函數達到輸入文本的末尾，它會返回一個空字符('\0')表示沒有更多的字符可以讀取。
  
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
