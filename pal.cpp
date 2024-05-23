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
  vector<string> content;
};

map<string, Type> gsymbolTable;
vector<string> gIdToeknName;

struct Variable {
  string typeName;
  string name;
};

struct Function {
  string typeName;
  vector<Variable> parameter;
  vector<string> body;
};

vector<Variable> gTempVariable;

// 使用 map 來存儲函數名稱與其多行定義
map<string, Function> functionMap;

map<Type, string> TypeNameMap = {
  { INT, "int" }, {FLOAT, "float"}, {CHAR, "char"}, {BOOL, "bool"}, {STRING, "string"}, {VOID, "void"},
  {IF, "if"}, {ELSE, "else"}, {WHILE, "while"}, {DO, "do"}, {RETURN, "return"},
  {LPAREN, "("}, {RPAREN, ")"}, {LBRACKET, "["}, {RBRACKET, "]"},
  {LBRACE, "{"}, {RBRACE, "}"},
  {PLUS, "+"}, {MINUS, "-"}, {MUL, "*"}, {DIV, "/"}, {MOD, "%"}, 
  {BIT_XOR, "^"}, {GT, ">"}, {LT, "<"}, {GE, ">="}, {LE, "<="}, {EQ, "=="}, {NEQ, "!="},
  {BIT_AND, "&"}, {BIT_OR, "|"},
  {ASSIGN, "="}, {NOT, "!"}, {AND, "&&"}, {OR, "||"},
  {PE, "+="}, {ME, "-="}, {TE, "*="}, {DE, "/="}, {RE, "%="},
  {PP, "++"}, {MM, "--"}, {RS, ">>"}, {LS, "<<"},
  {SEMICOLON, ";"}, {COMMA, ","},
  {QUESTION, "?"}, {COLON, ":"}
};

// 函數定義，並存儲到 map 中
  void DefineFunction( string name, string returnType, vector<Variable> params, vector<string> body) {
    Function func;
    func.typeName = returnType;
    func.parameter = params;
    func.body = body;
    functionMap[name] = func;
  }

// 列出所有函數名稱
void ListAllFunctions() {
    cout << "Listing all functions:" << endl;
    map<string, Function>::const_iterator it;
    for ( it = functionMap.begin(); it != functionMap.end(); ++it ) {
        cout << it->first << "()" << endl;
    }
}

// 列出指定函數的定義
void ListFunction( string name ) {
    map<string, Function>::const_iterator it = functionMap.find(name);
    if (it != functionMap.end()) {
    	cout << "> " ;
        for (size_t i = 0; i < it->second.body.size(); ++i) {
            cout << it->second.body[i] ;
            if ( it->second.body[i].compare( ";" ) == 0 || it->second.body[i].compare( "{" ) == 0 || it->second.body[i].compare( "}" ) == 0 ) cout << endl;
            else if ( i+1 < it->second.body.size() && it->second.body[i+1].compare( "(" ) == 0 ) {}
	  else cout << ' ';
        }
    } else {
        cout << "Function " << name << " not found." << endl;
    }
}

string toString(Type t) {
  map<Type, string>::const_iterator it = TypeNameMap.find(t);
  if (it != TypeNameMap.end()) {
    return it->second;
  } else {
    return "[Unknown Type]";
  }
}

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


class Parser {
public:
  explicit Parser( Tokenizer& tokenizer ) : tokenizer( tokenizer ), nextToken( tokenizer.GetNextToken() ) {}

  void Parse() {
    user_input();
  } // end parse() 

private:
    Tokenizer& tokenizer;
    Token nextToken;

  void user_input() {
    Token parsedResult ;
    int startLine;
    
    while ( nextToken.type != QUIT && parsedResult.type != QUIT ) {
    	
      startLine = nextToken.line;
      parsedResult = nextToken;
      
      if ( nextToken.type == VOID || type_specifier() ) {
        definition( parsedResult );
      } // end if
      
      else if ( StartExpression() || nextToken.type == SEMICOLON || nextToken.type == IF ||
                nextToken.type == WHILE || nextToken.type == DO || nextToken.type == RETURN ||
	      nextToken.type == ELSE ) {
        bool istatement = statement( parsedResult );
        if ( parsedResult.type == QUIT ) {}
        else if ( istatement ) cout << "Statement executed ...\n"; 
      } // end else if

      if ( parsedResult.type == ERROR ) {
        if ( parsedResult.error == LEXICALERROR )
          printf( "Line %d : unrecognized token with first char : '%s'\n", parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SYNTACTICALERROR )
          printf( "Line %d : Unexpected token : '%s'\n", parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );
	 
        else if ( parsedResult.error == SEMANTICERROR )
          printf( "Line %d : Undefined identifier : '%s'\n", parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() ); 
        else cout << "> Error" << endl ;
        
        while ( parsedResult.line >= nextToken.line ) nextToken = tokenizer.GetNextToken();
      } // end if
      
    } // end while
    
    gsymbolTable.clear();
  } // end user_input()
  
  bool definition( Token &parsedResult ) {
    Type type = nextToken.type;
    vector<Variable> variable;
    Variable temp;
    if ( nextToken.type == VOID ) {
      Match( VOID, parsedResult );
      gIdToeknName.push_back( nextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        parsedResult.type = VOID;
        parsedResult.tokenName = nextToken.tokenName;
        parsedResult.content.clear(); 
        if ( function_definition_without_ID( parsedResult ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) 
        	    printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
        	  else printf( "New definition of %s() entered...\n", gIdToeknName[i].c_str() );

        	  temp.typeName = "void";
        	  temp.name = "void";
        	  variable.push_back( temp );
        	  DefineFunction( parsedResult.tokenName, "void", variable, parsedResult.content );
        	  gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
        } // end if
      } // end if
    } // end if
    
    else {
      bool function = false;
      parsedResult.type = nextToken.type;
      parsedResult.content.clear();
      gTempVariable.clear();
      
      Match( nextToken.type, parsedResult );
      gIdToeknName.push_back( nextToken.tokenName );
      parsedResult.tokenName = nextToken.tokenName;
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( function_definition_or_declarators( parsedResult, function ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        		
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) {
        	    if ( !function ) {
        	      printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
        	      
              } // end if
        	    else {
	      DefineFunction( parsedResult.tokenName, toString(parsedResult.type), gTempVariable, parsedResult.content );
	      printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
              }
            } // end if
            
        	  else {
	    if ( !function )
	      printf( "New definition of %s entered...\n", gIdToeknName[i].c_str() );
              else {
                DefineFunction( parsedResult.tokenName, toString(parsedResult.type), gTempVariable, parsedResult.content );
	      printf( "New definition of %s() entered...\n", gIdToeknName[i].c_str() );
              }
            } // end else
        	  gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
          return true;
        } // end if
      }  // end if
    } // end else
    
    return false;
  } // end definition()
  
  bool type_specifier() {
    return nextToken.type == INT || nextToken.type == FLOAT ||
    nextToken.type == CHAR || nextToken.type == BOOL || nextToken.type == STRING ;
  } // end type_specifier()
 
  bool function_definition_or_declarators( Token &parsedResult, bool &function ) {
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( nextToken.type == LPAREN ) {
      function = true;
      return function_definition_without_ID( parsedResult  );
    } // end if
    
    else {
    // 如果下一個符號不是左括號，則處理為變量宣告
      return rest_of_declarators( parsedResult );
    } // end else
  } // end function_definition_or_declarators()

  bool rest_of_declarators( Token &parsedResult ) {
    // 檢查是否存在 '[' Constant ']' 結構
    if ( nextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );   // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) 
        Match( RBRACKET, parsedResult );
      
      else return false;
    } // end if

    // 處理 { ',' Identifier [ '[' Constant ']' ] } 結構
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );
      
      if ( nextToken.type == IDENTIFIER ) {
        gIdToeknName.push_back( nextToken.tokenName );
        Match( IDENTIFIER, parsedResult ); // 匹配標識符
        
        // 檢查是否存在 '[' Constant ']' 結構
        if ( Match( LBRACKET, parsedResult ) ) {
          
          if ( Match( CONSTANT, parsedResult) ) 
            Match( RBRACKET, parsedResult );
          else return false ;
        } // end if
        else return false;
      } // end if 
      
      else {
        if ( nextToken.type != ERROR ) {
          parsedResult = nextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SYNTACTICALERROR;
        } // end if
        
        else parsedResult = nextToken;
        
        return false;
      } // end else
    } // end while

    return Match( SEMICOLON, parsedResult );   // 結束分號
  } // end rest_of_declarators()

  bool function_definition_without_ID( Token &parsedResult ) {
    if ( !Match( LPAREN, parsedResult ) ) return false; 

    if ( nextToken.type == VOID ) {
      Match( VOID, parsedResult ); // 匹配 VOID
    } // end if

    else if ( type_specifier() ) 
      if ( !formal_parameter_list( parsedResult ) ) return false;// 解析 formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return compound_statement( parsedResult ); // 解析 compound_statement
  } // end function_definition_without_ID()
 
  bool formal_parameter_list( Token &parsedResult ) {
    Variable variable;
    Type type = nextToken.type;
    
    Match( nextToken.type, parsedResult ) ;
    
    if ( nextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult ); // 匹配 '&'
    } // end if
    
    if ( nextToken.type == IDENTIFIER ) {
      gsymbolTable[nextToken.tokenName] = type;
      variable.typeName = toString( type );
      variable.name = nextToken.tokenName;
      gTempVariable.push_back( variable );
      Match( IDENTIFIER, parsedResult ); 
    } // end if  
    
    else return false;
    
    if ( nextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult ); // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) {
        if ( !Match( RBRACKET, parsedResult ) ) return false;
      } // end if 
      
      else return false;
    } // end if

    bool isformal_parameter_list = true;
    while ( nextToken.type == COMMA && isformal_parameter_list ) {
      Match( COMMA, parsedResult ); // 匹配 ','
      if ( type_specifier() ) {
        isformal_parameter_list = formal_parameter_list( parsedResult );
      } // end if

      else Match( ERROR, parsedResult );
    } // end while
    
    return isformal_parameter_list;
  } // end formal_parameter_list()

  bool compound_statement( Token &parsedResult ) {
    bool iscompound_statement = false;
    if ( !Match( LBRACE, parsedResult ) ) return false;
      iscompound_statement = StartStatement();
      if ( iscompound_statement ) iscompound_statement = statement( parsedResult );
      else iscompound_statement = declaration( parsedResult );

    
    while ( iscompound_statement ) {
      iscompound_statement = StartStatement();
      if ( iscompound_statement ) iscompound_statement = statement( parsedResult );
      else iscompound_statement = declaration( parsedResult );
    } // end while

    
    if ( !Match( RBRACE, parsedResult ) ) return false;
    return true;
  } // end  compound_statement()

  bool declaration( Token &parsedResult ) {
    if ( !type_specifier() ) return false;
    Type type = nextToken.type;
    Match( nextToken.type, parsedResult );
    
    string tokenName = nextToken.tokenName;
      
    if ( Match( IDENTIFIER, parsedResult ) ) {
      if ( rest_of_declarators( parsedResult ) ) {
        gsymbolTable[tokenName] = type;
        return true;
      } // end if
    }  // end if
    
    return false;
  } // end declaration()

  bool statement( Token &parsedResult ) {
    if ( nextToken.type == SEMICOLON ) {
      Match( SEMICOLON, parsedResult );
      return true;
    } // end if 
    
    else if ( StartExpression() ) {
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if
    
    else if ( nextToken.type == RETURN ) {
      Match( RETURN, parsedResult );
      if ( nextToken.type != SEMICOLON ) {
        if ( !expression( parsedResult ) ) return false;
      } // end if

      if ( !Match( SEMICOLON, parsedResult ) ) {
        return false; // 或者進行其他錯誤處理
      } // end if
      
      return true;
    } // end else if
    
    else if ( nextToken.type == LBRACE ) {
      if ( !compound_statement( parsedResult ) ) return false;
      return true;
    } // end if 
    
    else if ( nextToken.type == IF ) {
      Match( IF, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !statement( parsedResult ) ) return false;
      
      if ( nextToken.type == ELSE ) {
        Match( ELSE, parsedResult );
        if ( !statement( parsedResult ) ) return false;
      } // end if 
      
      return true;
    } // end else if 
    
    else if (nextToken.type == WHILE) {
      Match( WHILE, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !statement( parsedResult ) ) return false; 
      return true;
    } // end else if
    
    else if ( nextToken.type == DO ) {
      Match( DO, parsedResult );
      if ( !statement( parsedResult ) ) return false; 
      if ( !Match( WHILE, parsedResult ) ) return false;
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if

    else Match( ERROR, parsedResult );
    return false;
  } // end statement()

  bool StartStatement() {
    if ( StartExpression() || nextToken.type == SEMICOLON || nextToken.type == IF ||
         nextToken.type == WHILE || nextToken.type == DO || nextToken.type == RETURN ||
         nextToken.type == ELSE ) return true;

    else return false;	
  } // end StartStatement()

  bool StartExpression() {
    if ( nextToken.type == IDENTIFIER || nextToken.type == CONSTANT ||
         nextToken.type == PP || nextToken.type == MM || nextToken.type == LPAREN ||
         nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT )
      return true;
    else return false;
  } // end StartExpression()

  bool expression( Token &parsedResult ) {
    bool expression = true; 
    if ( basic_expression( parsedResult ) ) {

      while ( nextToken.type == COMMA && expression ) {
        Match( COMMA, parsedResult ); 
        expression = basic_expression( parsedResult );
      } // end while
      
      return expression;
    } // end if
    
    return false;
  } // end expression

  bool basic_expression( Token &parsedResult ) {

    if ( nextToken.type == IDENTIFIER ) { // undefine
      
      if ( nextToken.tokenName.compare( "cin" ) == 0 || nextToken.tokenName.compare( "cout" ) == 0 ) {
      } // end if
      
      else if ( nextToken.tokenName.compare( "ListAllFunctions" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        if ( !rest_of_Identifier_started_basic_exp( parsedResult ) ) return false;
        ListAllFunctions();
        return true;
      } // end if
      
      else if ( nextToken.tokenName.compare( "ListFunction" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        
        if ( !rest_of_Identifier_started_basic_exp( parsedResult ) ) return false;
        
        ListFunction( parsedResult.tokenName ); 
        return true;
      } // end if      

      else if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
        parsedResult = nextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;
        
        if ( nextToken.tokenName.compare( "Done" ) == 0 ) {
        	parsedResult.tokenName = nextToken.tokenName;
        	Match( IDENTIFIER, parsedResult );
        	if ( !rest_of_Identifier_started_basic_exp( parsedResult ) ) return false;
        	
        	return true;
        } // end if
        
        return false;
      }  // end if 
      
      if ( !Match( IDENTIFIER, parsedResult ) ) return false;
      return rest_of_Identifier_started_basic_exp( parsedResult );
    } // end if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
      if ( !Match( nextToken.type, parsedResult ) ) return false;
      if ( nextToken.type != IDENTIFIER ) return false;
      if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
          parsedResult = nextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SEMANTICERROR;
          return false;
      }  // end if 
      
      Match( IDENTIFIER, parsedResult );
      return  rest_of_PPMM_Identifier_started_basic_exp( parsedResult );
    } // end else if 
    
    else if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      sign( parsedResult );
      while ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        sign( parsedResult ); 
      } // end while

      return signed_unary_exp( parsedResult ) && romce_and_romloe( parsedResult );
    } // end else if 
    
    else if ( nextToken.type == CONSTANT ) {
      parsedResult.tokenName = nextToken.tokenName;
      return Match( CONSTANT, parsedResult ) && romce_and_romloe( parsedResult );
    } // end else if
    
    else if ( nextToken.type == LPAREN ) {
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return romce_and_romloe( parsedResult );
    } // end else if
    
    return false;
  } // end basic_expression()

  bool rest_of_Identifier_started_basic_exp( Token &parsedResult ) {
    if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false;
    } // end if

    if ( assignment_operator( parsedResult ) ) {
        if ( !basic_expression( parsedResult ) ) return false;
    } // end if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
        Match( nextToken.type, parsedResult );
        if ( !romce_and_romloe( parsedResult ) ) return false;
    } // end else if
    
    else romce_and_romloe( parsedResult );
    
    if ( nextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult );
      
      if ( nextToken.type == RPAREN ) {
        Match( RPAREN, parsedResult );
        if ( nextToken.type == SEMICOLON && parsedResult.tokenName.compare( "Done" ) == 0 ) {
        	parsedResult.type = QUIT;
        	return true;
        } // end if
        
        return romce_and_romloe( parsedResult );
      } // end if
       
      else if ( !actual_parameter_list( parsedResult ) ) return false;
      
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return romce_and_romloe( parsedResult );
    } // end if
    
    return true;
  } // end rest_of_Identifier_started_basic_exp()

  bool rest_of_PPMM_Identifier_started_basic_exp( Token &parsedResult ) {
    if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        if ( !expression(parsedResult) ) return false; 
        if ( !Match( RBRACKET, parsedResult ) ) return false; 
    } // end if

    return romce_and_romloe( parsedResult );  // 解析后?表?式
  } // end rest_of_PPMM_Identifier_started_basic_exp()

  bool actual_parameter_list( Token &parsedResult ) {
    if ( !basic_expression( parsedResult ) ) return false;
    
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // 消耗 ','
      if ( !basic_expression( parsedResult ) ) return false;  
    } // end while
    
    return true;
  } // end actual_parameter_list()

  bool assignment_operator( Token &parsedResult ) {
    if ( nextToken.type == ASSIGN || nextToken.type == TE || nextToken.type == DE || 
        nextToken.type == RE || nextToken.type == PE || nextToken.type == ME) {
      return Match( nextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end assignment_operator()

  bool romce_and_romloe( Token &parsedResult ) {
    
    if ( !rest_of_maybe_logical_OR_exp( parsedResult ) ) {
      return false; 
    }
    
    if ( nextToken.type == QUESTION ) {
        Match( QUESTION, parsedResult );  // 消耗 '?'
        if ( !basic_expression( parsedResult ) ) return false;  // 解析?件?真?的表?式
        if ( !Match( COLON, parsedResult ) ) return false;  // 消耗 ':'
        if ( !basic_expression( parsedResult ) ) return false;  // 解析?件?假?的表?式
    } // end if


    return true;
  } // end romce_and_romloe()

  bool rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?理??与表?式

    while ( nextToken.type == OR ) {
      Match( OR, parsedResult );  // 消耗 '||'
      if ( !maybe_logical_AND_exp( parsedResult ) ) return false;  // 解析??与表?式
    } // end while
    
    return true;
  } // end rest_of_maybe_logical_OR_exp()

  bool maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位或表?式

    while (nextToken.type == AND) {
        Match( AND, parsedResult );  // 消耗 '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end maybe_logical_AND_exp()

  bool rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理位或表?式的剩余部分

    while ( nextToken.type == AND ) {
        Match( AND, parsedResult );  // 消耗 '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end rest_of_maybe_logical_AND_exp()

  bool maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位异或表?式

    while ( nextToken.type == BIT_OR ) {
      Match( BIT_OR, parsedResult );  // 消耗 '|'
      if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理后?的位异或表?式
    } // end while

    return true;
  } // end maybe_bit_OR_exp()

  bool rest_of_maybe_bit_OR_exp(Token &parsedResult) {
    if ( !rest_of_maybe_bit_ex_OR_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_OR)  {
      Match (BIT_OR, parsedResult );
      if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_bit_OR_exp()

  bool maybe_bit_ex_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end maybe_bit_ex_OR_exp()

  bool rest_of_maybe_bit_ex_OR_exp(Token &parsedResult) {
    if ( !rest_of_maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_bit_ex_OR_exp()
  
  bool maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !maybe_equality_exp( parsedResult ) ) return false;

    while (nextToken.type == BIT_AND) {
      Match(BIT_AND, parsedResult);
      if (!maybe_equality_exp(parsedResult)) return false;
    } // end while

    return true;
  } // end maybe_bit_AND_exp()
  
  bool rest_of_maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_equality_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult );
      if ( !maybe_equality_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_bit_AND_exp()
  
  bool maybe_equality_exp(Token &parsedResult) {
    if ( !maybe_relational_exp( parsedResult ) ) return false;

    while ( nextToken.type == EQ || nextToken.type == NEQ ) {
      Match( nextToken.type, parsedResult );  // 消耗 EQ 或 NEQ
      if ( !maybe_relational_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end maybe_equality_exp()
  
  bool rest_of_maybe_equality_exp(Token &parsedResult) {
  	
    if (!rest_of_maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // 消耗 EQ 或 NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }
    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_equality_exp()

  bool maybe_relational_exp(Token &parsedResult) {
    if ( !maybe_shift_exp( parsedResult ) ) return false;

    while ( nextToken.type == GT || nextToken.type == LT || 
           nextToken.type == LE || nextToken.type == GE) {
      Match( nextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !maybe_shift_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end maybe_relational_exp()

  bool rest_of_maybe_relational_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_shift_exp( parsedResult ) ) return false;
    while (nextToken.type == GT || nextToken.type == LT || 
           nextToken.type == LE || nextToken.type == GE) {
      Match( nextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !maybe_shift_exp( parsedResult ) ) return false;
    } // end while
    return true;
  } // end rest_of_maybe_relational_exp()

  bool maybe_shift_exp( Token &parsedResult ) {
    if ( !maybe_additive_exp( parsedResult ) ) return false;

    while ( nextToken.type == LS || nextToken.type == RS ) {
      Match( nextToken.type, parsedResult );  // 消耗 << 或 >>
      if ( !maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // maybe_shift_exp()

  bool rest_of_maybe_shift_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_additive_exp( parsedResult ) ) return false;

    while ( nextToken.type == LS || nextToken.type == RS ) {
        Match( nextToken.type, parsedResult );  // 消耗 LS 或 RS
        if ( !maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_shift_exp()

  bool maybe_additive_exp( Token &parsedResult ) {
    if ( !maybe_mult_exp( parsedResult ) ) return false;

    while ( nextToken.type == PLUS || nextToken.type == MINUS ) {
      Match( nextToken.type, parsedResult );  // 消耗 '+' 或 '-'
      if ( !maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end maybe_additive_exp()

  bool rest_of_maybe_additive_exp(Token &parsedResult) {
    if (!rest_of_maybe_mult_exp(parsedResult)) return false;
    
    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // 消耗 '+' 或 '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_additive_exp()

  bool maybe_mult_exp( Token &parsedResult ) {
    // cout << nextToken.tokenName << endl;
    if ( !unary_exp( parsedResult ) ) return false;
    return rest_of_maybe_mult_exp( parsedResult );  
  } // end maybe_mult_exp()

  bool rest_of_maybe_mult_exp(Token &parsedResult) {
    while (nextToken.type == MUL || nextToken.type == DIV || nextToken.type == MOD) {
        Match(nextToken.type, parsedResult);  
        if (!unary_exp(parsedResult)) return false;
    }
    return true;  
  } // end rest_of_maybe_mult_exp()

  bool unary_exp( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      while ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        sign( parsedResult );
      } // end while
        
      return signed_unary_exp(parsedResult);
    } // end of
     
    else if ( nextToken.type == IDENTIFIER || nextToken.type == CONSTANT || nextToken.type == LPAREN ) {
      return unsigned_unary_exp(parsedResult);
    } // end else if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
      Match( nextToken.type, parsedResult ); // 消耗 PP 或 MM
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
          if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
              parsedResult = nextToken;
              parsedResult.type = ERROR;
              parsedResult.error = SEMANTICERROR;
              return false;
          }  // end if 
        
        if ( nextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult ); // 消耗 '['
          if (!expression(parsedResult)) return false; 
          if (!Match(RBRACKET, parsedResult)) return false; // 消耗 ']'
        } // end if
        return true;
      } // end if
        return false;
    } // end else if
    
    return false;
  } // end unary_exp()

  bool signed_unary_exp( Token &parsedResult ) {
    if ( nextToken.type == IDENTIFIER ) {
        if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
            parsedResult = nextToken;
            parsedResult.type = ERROR;
            parsedResult.error = SEMANTICERROR;
            return false;
        }  // end if 
        
      Match( IDENTIFIER, parsedResult );
      if ( nextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        
        if ( nextToken.type != RPAREN ) {
          if ( !actual_parameter_list( parsedResult ) ) return false;
        } // end if
         
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
      } // end else if
    
      return true;
    } // end if 
    
    else if ( nextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if (nextToken.type == LPAREN) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      if ( !expression( parsedResult ) ) return false;
      return Match(RPAREN, parsedResult); // 消耗 ')'
    } // end else if
    
    return false;
  }

  bool unsigned_unary_exp( Token &parsedResult ) {
    if ( nextToken.type == IDENTIFIER ) {
        if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
            parsedResult = nextToken;
            parsedResult.type = ERROR;
            parsedResult.error = SEMANTICERROR;
            return false;
        }  // end if 

      Match( IDENTIFIER, parsedResult );
      
      if ( nextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        if ( nextToken.type != RPAREN ) {
          if ( !actual_parameter_list( parsedResult ) ) return false;
        } // end if
 
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
        if ( nextToken.type == PP || nextToken.type == MM ) {
          Match( nextToken.type, parsedResult );
        } // end if
      } // end else if
    
      return true;
    } // end if 
    
    else if ( nextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if (nextToken.type == LPAREN) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      
      if ( !expression( parsedResult ) ) return false;
      
      return Match(RPAREN, parsedResult); // 消耗 ')'
    } // end else if
    
    return false;
  } // end unsigned_unary_exp

  bool sign( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      return Match( nextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end sign()

  bool Match( Type expected, Token &parsedResult ) {
    if ( nextToken.type == expected ) {
      parsedResult.content.push_back( nextToken.tokenName );
      nextToken = tokenizer.GetNextToken();
      return true;
    } // end if 
    
    else {
      if ( nextToken.type != ERROR ) {
        parsedResult = nextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SYNTACTICALERROR;
      } // end if
        
      else parsedResult = nextToken;
      nextToken = tokenizer.GetNextToken();
      return false;
    } // end else
  } // end Match()
 
};


int main() {
  vector<string> minput = Read_lines();
  Tokenizer tokenizer( minput );

  cout << "Our-C running ..." << endl ;
  
  // Parser parse( tokenizer ) ;
  // parse.Parse() ;
  cout << "Our-C exited ..." << endl ; 
  
} // end main()
