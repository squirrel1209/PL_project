# include <iostream>
# include <stdio.h>
# include <cstdlib>
# include <vector>
# include <cctype> 
# include <string>
# include <map>
# include <cmath>
# include <fstream>

using namespace std;
typedef char String200[200];
string gtestnum ;

vector<string> Read_lines() {
  vector<string> ginput_lines ;
    
  
  string line;   
  char ch;
  bool isFirstLine = true;
  while ( cin.get( ch ) ) {               // 逐行讀取文件內容
    line += ch;
    
    if ( ch == '\n' ) {
      if ( isFirstLine ) {
        // 如果是第一行，則重置line並更改標誌狀態，不將其添加到ginput_lines
        gtestnum = line ;
        line.clear();
        isFirstLine = false;
      } // end if
      
      else {
        ginput_lines.push_back( line );
        line.clear();  // 清空line以用於下一行的讀取
      } // end else
    } // end if 
  } // end while

  // 檢查最後一行是否因為文件結束而沒有換行符結尾
  if ( !line.empty() ) {
    ginput_lines.push_back( line );
  } // end if
  
  return ginput_lines;
} // end Read_lines()

vector<string> read_lines_from_file( string filename ) {

  vector<string> input_lines ;
  ifstream file(filename.c_str()) ;                            // 打開文件
    
  if ( file.is_open() ) {
    string line;         
    while ( getline( file, line ) ) {               // 逐行讀取文件內容
      input_lines.push_back( line );
    } // end while
         
    file.close();                                  // 關閉文件
  } // end if
    
  else cerr << "Unable to open file: " << filename << endl;
  return input_lines;
} // end read_lines_from_file

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
map<string, Type> gLocalsymbolTable;
vector<string> gIdToeknName;
vector<string> ginput;

struct Variable {
  string typeName;
  string name;
  vector<string> body;
};

struct Function {
  string typeName;
  vector<Variable> parameter;
  vector<string> body;
};

vector<Variable> gTempVariable;

// 使用 map 來存儲函數名稱與其多行定義
map<string, Function> gfunctionMap;
map<string, Variable> gvariableMap;
map<Type, string> gTypeNameMap;

void InitializegTypeNameMap() {
  gTypeNameMap[INT] = "int";
  gTypeNameMap[FLOAT] = "float";
  gTypeNameMap[CHAR] = "char";
  gTypeNameMap[BOOL] = "bool";
  gTypeNameMap[STRING] = "string";
  gTypeNameMap[VOID] = "void";
  gTypeNameMap[IF] = "if";
  gTypeNameMap[ELSE] = "else";
  gTypeNameMap[WHILE] = "while";
  gTypeNameMap[DO] = "do";
  gTypeNameMap[RETURN] = "return";
  gTypeNameMap[LPAREN] = "("; 
  gTypeNameMap[RPAREN] = ")";
  gTypeNameMap[LBRACKET] = "[";
  gTypeNameMap[RBRACKET] = "]";
  gTypeNameMap[LBRACE] = "{";
  gTypeNameMap[RBRACE] = "}";
  gTypeNameMap[PLUS] = "+";
  gTypeNameMap[MINUS] = "-";
  gTypeNameMap[MUL] = "*";
  gTypeNameMap[DIV] = "/";
  gTypeNameMap[MOD] = "%";
  gTypeNameMap[BIT_XOR] = "^";
  gTypeNameMap[GT] = ">";
  gTypeNameMap[LT] = "<";
  gTypeNameMap[GE] = ">=";
  gTypeNameMap[LE] = "<=";
  gTypeNameMap[EQ] = "==";
  gTypeNameMap[NEQ] = "!=";
  gTypeNameMap[BIT_AND] = "&";
  gTypeNameMap[BIT_OR] = "|";
  gTypeNameMap[ASSIGN] = "=";
  gTypeNameMap[NOT] = "!";
  gTypeNameMap[AND] = "&&";
  gTypeNameMap[OR] = "||";
  gTypeNameMap[PE] = "+=";
  gTypeNameMap[ME] = "-=";
  gTypeNameMap[TE] = "*=";
  gTypeNameMap[DE] = "/=";
  gTypeNameMap[RE] = "%=";
  gTypeNameMap[PP] = "++";
  gTypeNameMap[MM] = "--";
  gTypeNameMap[RS] = ">>";
  gTypeNameMap[LS] = "<<";
  gTypeNameMap[SEMICOLON] = ";";
  gTypeNameMap[COMMA] = ",";
  gTypeNameMap[QUESTION] = "?";
  gTypeNameMap[COLON] = ":";
} // end InitializegTypeNameMap()

// 函數定義，並存儲到 map 中
void DefineFunction( string name, string returnType, vector<Variable> params, vector<string> body ) {
  Function func;
  func.typeName = returnType;
  func.parameter = params;
  func.body = body;
  gfunctionMap[name] = func;
} // end DefineFunction()

void DefineVariable( string name, string returnType, vector<string> body ) {
  Variable var;
  var.name = name;
  var.typeName = returnType;
  var.body = body;
  gvariableMap[name] = var;
} // end DefineVariable

// 列出所有函數名稱
void ListAllFunctions() {
  map<string, Function>::iterator it;
  for ( it = gfunctionMap.begin() ; it != gfunctionMap.end() ; ++it ) {
    cout << it->first << "()" << endl;
  } // end for 
} // end ListAllFunctions()

void ListAllVariables() {
  map<string, Variable>::iterator it;
  for ( it = gvariableMap.begin() ; it != gvariableMap.end() ; ++it ) {
    cout << it->first << endl;
  } // end for 
} // end ListAllVariables()

// 列出指定函數的定義
void ListFunction( string name ) {
  map<string, Function>::iterator it = gfunctionMap.find( name );
  if ( it != gfunctionMap.end() ) {
    cout << "> " ;
    for ( size_t i = 0 ; i < it -> second.body.size() ; ++i ) {
      cout << it -> second.body[i] ;
      if ( it -> second.body[i].compare( ";" ) == 0 || it -> second.body[i].compare( "{" ) == 0 ||
           it -> second.body[i].compare( "}" ) == 0 ) cout << endl;
      else if ( i+1 < it -> second.body.size() && it -> second.body[i+1].compare( "(" ) == 0 ) { }
      else cout << ' ';
    } // end for
  } // end if 
    
  else {
    cout << "Function " << name << " not found." << endl;
  } // end else
} // end ListFunction()

void ListVariable( string name ) {
  bool isfind = false;

  map<string, Variable>::iterator it = gvariableMap.find( name );
  if ( it != gvariableMap.end() ) {
    if ( it -> second.body[1].compare( name ) == 0 ) isfind = true;
    cout << it -> second.typeName << " " << it -> second.name;
    
    for ( size_t i = 2 ; i < it -> second.body.size() ; ++i ) {
      if ( it -> second.body[i].compare( "," ) == 0 ||
           it -> second.body[i].compare( ";" ) == 0 ) isfind = false;
      if ( isfind && it -> second.body[i].compare( "[" ) == 0 ) {
        cout << "abc";
        cout << it -> second.body[i] ;
      } // end if
      
      else if ( isfind ) cout << " " << it -> second.body[i] ;
    } // end for 

    cout << " ;" << endl;
  } // end if 
    
  else {
    cout << "Variable " << name << " not found." << endl;
  } // end else
} // end ListVariable()

void RemoveKeyFromMap( map<string, Type>& symbolTable, string keyToRemove ) {
    // 搜尋鍵
  map<string, Type>::iterator it = symbolTable.find( keyToRemove );
  if ( it != symbolTable.end() ) {
        // 若找到，則刪除
    symbolTable.erase(it);
  } // end if
} // end RemoveKeyFromMap

string ToString( Type t ) {
  map<Type, string>::iterator it = gTypeNameMap.find( t );
  if ( it != gTypeNameMap.end() ) {
    return it->second;
  } // end if
  
  else {
    return "[Unknown Type]";
  } // end else
} // end ToString()

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
  int mlineIndex;
  int mcolumnIndex;
  int mlastmlineIndex;
public:
  Tokenizer() {
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
    
    if ( mlineIndex >= ginput.size() ) {
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
      mcolumnIndex = ginput[mlineIndex].size() ;
    } // end if
    
    else mcolumnIndex-- ;
  } // end PreviousChar()

  char GetNextChar() {
  // GetNextChar() 從一個給定的輸入（通常是一個字符串向量，每個元素代表源代碼的一行）中順序讀取下一個字符。
  // 當函數達到當前行的末尾時，它會自動轉到下一行的開始。
  // 如果函數達到輸入文本的末尾，它會返回一個空字符('\0')表示沒有更多的字符可以讀取。
  
    mlastmlineIndex = mlineIndex;  

    if ( mlineIndex < ginput.size() ) {
      if ( mcolumnIndex < ginput[mlineIndex].size() ) {
        char nextChar = ginput[mlineIndex][mcolumnIndex++];
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

Tokenizer gtokenizer;

class Parser {
public:
  Parser() {
    mnextToken = gtokenizer.GetNextToken();
  } // end Parser()
  
  void Parse() {
    User_input();
  } // end Parse() 

private:
  Token mnextToken;

  void User_input() {
    Token parsedResult ;
    parsedResult = mnextToken;
    int startLine = mnextToken.line;
    while ( mnextToken.type != QUIT && parsedResult.type != QUIT ) {
      cout << "> " ;
      parsedResult = mnextToken;
      if ( mnextToken.type == VOID || Type_specifier() ) {
        Definition( parsedResult );
      } // end if

      else if ( StartExpression() || mnextToken.type == SEMICOLON || mnextToken.type == IF ||
                mnextToken.type == WHILE || mnextToken.type == DO || mnextToken.type == RETURN ||
                mnextToken.type == ELSE  || mnextToken.type == LBRACE ) {
        bool iStatement = Statement( parsedResult );
        if ( parsedResult.type == QUIT ) { }
        else if ( iStatement ) cout << "Statement executed ...\n"; 
      } // end else if

      else Match( ERROR, parsedResult );
      if ( parsedResult.type == ERROR ) {
        if ( parsedResult.error == LEXICALERROR )
          printf( "Line %d : unrecognized token with first char : '%s'\n", 
                  parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SYNTACTICALERROR )
          printf( "Line %d : unexpected token : '%s'\n", 
                  parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SEMANTICERROR )
          printf( "Line %d : undefined identifier : '%s'\n", 
                  parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() ); 
        else cout << "Error" << endl ;
        
        while ( parsedResult.line >= mnextToken.line ) mnextToken = gtokenizer.GetNextToken();
      } // end if
      
      startLine = parsedResult.line + 1;
    } // end while
    
    gsymbolTable.clear();
  } // end User_input()
  
  bool Definition( Token &parsedResult ) {
    Type type = mnextToken.type;
    vector<Variable> variable;
    Variable temp;
    if ( mnextToken.type == VOID ) {
      Match( VOID, parsedResult );
      gIdToeknName.push_back( mnextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        parsedResult.type = VOID;
        parsedResult.tokenName = mnextToken.tokenName;
        parsedResult.content.clear(); 
        if ( Function_definition_without_ID( parsedResult ) ) {
          for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
            if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end()  ) 
              printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
            else printf( "New definition of %s() entered ...\n", gIdToeknName[i].c_str() );

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
      parsedResult.type = mnextToken.type;
      parsedResult.content.clear();
      gTempVariable.clear();

      Match( mnextToken.type, parsedResult );
      gIdToeknName.push_back( mnextToken.tokenName );
      parsedResult.tokenName = mnextToken.tokenName;
      
      if ( Match( IDENTIFIER, parsedResult ) ) {

        if ( Function_definition_or_declarators( parsedResult, function ) ) {
          for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
            if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) {
              if ( !function ) {
                
                DefineVariable( gIdToeknName[i], ToString( parsedResult.type ), parsedResult.content ); 
                printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
              } // end if
              else {
                DefineFunction( parsedResult.tokenName, ToString( parsedResult.type ), 
                                gTempVariable, parsedResult.content );
                printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
              } // end else
            } // end if

            else {
              if ( !function ) {
                DefineVariable( gIdToeknName[i], ToString( parsedResult.type ), parsedResult.content ); 
                printf( "New definition of %s entered ...\n", gIdToeknName[i].c_str() );
              } // end if
              else {
                DefineFunction( parsedResult.tokenName, ToString( parsedResult.type ), 
                                gTempVariable, parsedResult.content );
                printf( "New definition of %s() entered ...\n", gIdToeknName[i].c_str() );
              } // end else
            } // end else

            gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
          return true;
        } // end if
        
      }  // end if
    } // end else
    
    return false;
  } // end Definition()
  
  bool Type_specifier() {
    return mnextToken.type == INT || mnextToken.type == FLOAT ||
    mnextToken.type == CHAR || mnextToken.type == BOOL || mnextToken.type == STRING ;
  } // end Type_specifier()
 
  bool Function_definition_or_declarators( Token &parsedResult, bool &function ) {
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( mnextToken.type == LPAREN ) {
      function = true;
      return Function_definition_without_ID( parsedResult  );
    } // end if
    
    else {
    // 如果下一個符號不是左括號，則處理為變量宣告
      return Rest_of_declarators( parsedResult );
    } // end else
  } // end Function_definition_or_declarators()

  bool Rest_of_declarators( Token &parsedResult ) {
    // 檢查是否存在 '[' Constant ']' 結構
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );   // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) 
        Match( RBRACKET, parsedResult );
      
      else return false;
    } // end if

    // 處理 { ',' Identifier [ '[' Constant ']' ] } 結構
    while ( mnextToken.type == COMMA ) {
      Match( COMMA, parsedResult );

      if ( mnextToken.type == IDENTIFIER ) {
        gIdToeknName.push_back( mnextToken.tokenName );
        Match( IDENTIFIER, parsedResult ); // 匹配標識符
        
        // 檢查是否存在 '[' Constant ']' 結構
        if ( mnextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult );

          if ( Match( CONSTANT, parsedResult ) ) {
            Match( RBRACKET, parsedResult ); 
          } // end if

          else return false ;
        } // end if
      } // end if 
      
      else {
        if ( mnextToken.type != ERROR ) {
          parsedResult = mnextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SYNTACTICALERROR;
        } // end if
        
        else parsedResult = mnextToken;
        
        return false;
      } // end else
    } // end while

    return Match( SEMICOLON, parsedResult );   // 結束分號
  } // end Rest_of_declarators()

  bool Function_definition_without_ID( Token &parsedResult ) {
    if ( !Match( LPAREN, parsedResult ) ) return false; 

    if ( mnextToken.type == VOID ) {
      Match( VOID, parsedResult ); // 匹配 VOID
    } // end if

    else if ( Type_specifier() ) 
      if ( !Formal_parameter_list( parsedResult ) ) return false; // 解析 Formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return Compound_Statement( parsedResult ); // 解析 Compound_Statement
  } // end Function_definition_without_ID()
 
  bool Formal_parameter_list( Token &parsedResult ) {
    Variable variable;
    Type type = mnextToken.type;
    
    Match( mnextToken.type, parsedResult ) ;
    
    if ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult ); // 匹配 '&'
    } // end if
    
    if ( mnextToken.type == IDENTIFIER ) {
      gsymbolTable[mnextToken.tokenName] = type;
      variable.typeName = ToString( type );
      variable.name = mnextToken.tokenName;
      gTempVariable.push_back( variable );
      Match( IDENTIFIER, parsedResult ); 
    } // end if  
    
    else return false;
    
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult ); // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) {
        if ( !Match( RBRACKET, parsedResult ) ) return false;
      } // end if 
      
      else return false;
    } // end if

    bool isFormal_parameter_list = true;
    while ( mnextToken.type == COMMA && isFormal_parameter_list ) {
      Match( COMMA, parsedResult ); // 匹配 ','
      if ( Type_specifier() ) {
        isFormal_parameter_list = Formal_parameter_list( parsedResult );
      } // end if

      else Match( ERROR, parsedResult );
    } // end while
    
    return isFormal_parameter_list;
  } // end Formal_parameter_list()

  bool Compound_Statement( Token &parsedResult ) {
    bool isCompound_Statement = false;
    if ( !Match( LBRACE, parsedResult ) ) return false;
    isCompound_Statement = StartStatement();
    if ( isCompound_Statement ) isCompound_Statement = Statement( parsedResult );
    else isCompound_Statement = Declaration( parsedResult );


    while ( isCompound_Statement ) {
      isCompound_Statement = StartStatement();
      if ( isCompound_Statement ) isCompound_Statement = Statement( parsedResult );
      else isCompound_Statement = Declaration( parsedResult );
    } // end while

    
    if ( !Match( RBRACE, parsedResult ) ) return false;

    return true;
  } // end Compound_Statement()

  bool Declaration( Token &parsedResult ) {
    if ( !Type_specifier() ) return false;
    Type type = mnextToken.type;
    Match( mnextToken.type, parsedResult );
    
    string tokenName = mnextToken.tokenName;
      
    if ( Match( IDENTIFIER, parsedResult ) ) {
      if ( Rest_of_declarators( parsedResult ) ) {
        gLocalsymbolTable[tokenName] = type;
        return true;
      } // end if
    }  // end if
    
    return false;
  } // end Declaration()

  bool Statement( Token &parsedResult ) {
    if ( mnextToken.type == SEMICOLON ) {
      Match( SEMICOLON, parsedResult );
      return true;
    } // end if 
    
    else if ( StartExpression() ) {
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if
    
    else if ( mnextToken.type == RETURN ) {
      Match( RETURN, parsedResult );
      if ( mnextToken.type != SEMICOLON ) {
        if ( !Expression( parsedResult ) ) return false;
      } // end if

      if ( !Match( SEMICOLON, parsedResult ) ) {
        return false; // 或者進行其他錯誤處理
      } // end if
      
      return true;
    } // end else if
    
    else if ( mnextToken.type == LBRACE ) {
      gLocalsymbolTable.clear();
      if ( !Compound_Statement( parsedResult ) ) return false;
      return true;
    } // end if 
    
    else if ( mnextToken.type == IF ) {
      Match( IF, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Statement( parsedResult ) ) return false;
      
      if ( mnextToken.type == ELSE ) {
        Match( ELSE, parsedResult );
        if ( !Statement( parsedResult ) ) return false;
      } // end if 
      
      return true;
    } // end else if 
    
    else if ( mnextToken.type == WHILE ) {

      Match( WHILE, parsedResult );
      
      if ( !Match( LPAREN, parsedResult ) ) return false;
      
      if ( !Expression( parsedResult ) ) return false;
      
      if ( !Match( RPAREN, parsedResult ) ) return false;
      
      if ( !Statement( parsedResult ) ) return false; 
      
      return true;
    } // end else if
    
    else if ( mnextToken.type == DO ) {
      Match( DO, parsedResult );
      if ( !Statement( parsedResult ) ) return false; 
      if ( !Match( WHILE, parsedResult ) ) return false;
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if

    else Match( ERROR, parsedResult );
    return false;
  } // end Statement()

  bool StartStatement() {
    if ( StartExpression() || mnextToken.type == SEMICOLON || mnextToken.type == IF ||
         mnextToken.type == WHILE || mnextToken.type == DO || mnextToken.type == RETURN ||
         mnextToken.type == ELSE || mnextToken.type == LBRACE ) return true;

    else return false;
  } // end StartStatement()

  bool StartExpression() {
    if ( mnextToken.type == IDENTIFIER || mnextToken.type == CONSTANT ||
         mnextToken.type == PP || mnextToken.type == MM || mnextToken.type == LPAREN ||
         mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT )
      return true;
    else return false;
  } // end StartExpression()

  bool Expression( Token &parsedResult ) {
    bool expression = true; 
    if ( Basic_Expression( parsedResult ) ) {

      while ( mnextToken.type == COMMA && expression ) {
        Match( COMMA, parsedResult ); 
        expression = Basic_Expression( parsedResult );
      } // end while
      
      return expression;
    } // end if
    
    return false;
  } // end Expression()

  bool Basic_Expression( Token &parsedResult ) {

    if ( mnextToken.type == IDENTIFIER ) { // undefine
      
      if ( mnextToken.tokenName.compare( "cin" ) == 0 || mnextToken.tokenName.compare( "cout" ) == 0 ) {
      } // end if
      
      else if ( mnextToken.tokenName.compare( "ListAllFunctions" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;
        ListAllFunctions();
        return true;
      } // end if
      
      else if ( mnextToken.tokenName.compare( "ListFunction" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        
        if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;
        
        ListFunction( parsedResult.tokenName ); 
        return true;
      } // end else if
      
      else if ( mnextToken.tokenName.compare( "ListAllVariables" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;
        ListAllVariables();
        return true;
      } // end if
      
      else if ( mnextToken.tokenName.compare( "ListVariable" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        
        if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;
        
        ListVariable( parsedResult.tokenName ); 
        return true;
      } // end else if      

      else if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() && 
                gLocalsymbolTable.find( mnextToken.tokenName ) == gLocalsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;
        
        if ( mnextToken.tokenName.compare( "Done" ) == 0 ) {
          parsedResult.tokenName = mnextToken.tokenName;
          Match( IDENTIFIER, parsedResult );
          if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;

          return true;
        } // end if
        
        return false;
      }  // end else if 
      
      if ( !Match( IDENTIFIER, parsedResult ) ) return false;
      return Rest_of_identifier_started_basic_exp( parsedResult );
    } // end if 
    
    else if ( mnextToken.type == PP || mnextToken.type == MM ) {
      if ( !Match( mnextToken.type, parsedResult ) ) return false;
      if ( mnextToken.type != IDENTIFIER ) return false;
      if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() && 
           gLocalsymbolTable.find( mnextToken.tokenName ) == gLocalsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;
        return false;
      }  // end if 
      
      Match( IDENTIFIER, parsedResult );
      return  Rest_of_PPMM_Identifier_started_basic_exp( parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
      Sign( parsedResult );
      while ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
        Sign( parsedResult ); 
      } // end while

      return Signed_Unary_exp( parsedResult ) && Romce_and_romloe( parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == CONSTANT ) {
      parsedResult.tokenName = mnextToken.tokenName;
      return Match( CONSTANT, parsedResult ) && Romce_and_romloe( parsedResult );
    } // end else if
    
    else if ( mnextToken.type == LPAREN ) {
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return Romce_and_romloe( parsedResult );
    } // end else if
    
    return false;
  } // end Basic_Expression()

  bool Rest_of_identifier_started_basic_exp( Token &parsedResult ) {
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RBRACKET, parsedResult ) ) return false;
    } // end if

    if ( AsSignment_operator( parsedResult ) ) {
      if ( !Basic_Expression( parsedResult ) ) return false;
    } // end if 
    
    else if ( mnextToken.type == PP || mnextToken.type == MM ) {
      Match( mnextToken.type, parsedResult );
      if ( !Romce_and_romloe( parsedResult ) ) return false;
    } // end else if
    
    else Romce_and_romloe( parsedResult );
    
    if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult );
      
      if ( mnextToken.type == RPAREN ) {
        Match( RPAREN, parsedResult );
        if ( mnextToken.type == SEMICOLON && parsedResult.tokenName.compare( "Done" ) == 0 ) {
          parsedResult.type = QUIT;
          return true;
        } // end if
        
        return Romce_and_romloe( parsedResult );
      } // end if
       
      else if ( !Actual_parameter_list( parsedResult ) ) return false;
      
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return Romce_and_romloe( parsedResult );
    } // end if
    
    return true;
  } // end Rest_of_identifier_started_basic_exp()

  bool Rest_of_PPMM_Identifier_started_basic_exp( Token &parsedResult ) {
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );
      if ( !Expression( parsedResult ) ) return false; 
      if ( !Match( RBRACKET, parsedResult ) ) return false; 
    } // end if

    return Romce_and_romloe( parsedResult );  // 解析后?表?式
  } // end Rest_of_PPMM_Identifier_started_basic_exp()

  bool Actual_parameter_list( Token &parsedResult ) {
    if ( !Basic_Expression( parsedResult ) ) return false;
    
    while ( mnextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // 消耗 ','
      if ( !Basic_Expression( parsedResult ) ) return false;  
    } // end while
    
    return true;
  } // end Actual_parameter_list()

  bool AsSignment_operator( Token &parsedResult ) {
    if ( mnextToken.type == ASSIGN || mnextToken.type == TE || mnextToken.type == DE || 
         mnextToken.type == RE || mnextToken.type == PE || mnextToken.type == ME ) {
      return Match( mnextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end AsSignment_operator()

  bool Romce_and_romloe( Token &parsedResult ) {
    
    if ( !Rest_of_maybe_logical_OR_exp( parsedResult ) ) {
      return false; 
    } // end if 
    
    if ( mnextToken.type == QUESTION ) {
      Match( QUESTION, parsedResult );  // 消耗 '?'
      if ( !Basic_Expression( parsedResult ) ) return false;  // 解析?件?真?的表?式
      if ( !Match( COLON, parsedResult ) ) return false;  // 消耗 ':'
      if ( !Basic_Expression( parsedResult ) ) return false;  // 解析?件?假?的表?式
    } // end if


    return true;
  } // end Romce_and_romloe()

  bool Rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?理??与表?式

    while ( mnextToken.type == OR ) {
      Match( OR, parsedResult );  // 消耗 '||'
      if ( !Maybe_logical_AND_exp( parsedResult ) ) return false;  // 解析??与表?式
    } // end while
    
    return true;
  } // end Rest_of_maybe_logical_OR_exp()

  bool Maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位或表?式

    while ( mnextToken.type == AND ) {
      Match( AND, parsedResult );  // 消耗 '&&'
      if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理位或表?式的剩余部分

    while ( mnextToken.type == AND ) {
      Match( AND, parsedResult );  // 消耗 '&&'
      if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end Rest_of_maybe_logical_AND_exp()

  bool Maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位异或表?式

    while ( mnextToken.type == BIT_OR ) {
      Match( BIT_OR, parsedResult );  // 消耗 '|'
      if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理后?的位异或表?式
    } // end while

    return true;
  } // end Maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_ex_OR_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_OR )  {
      Match( BIT_OR, parsedResult );
      if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_bit_OR_exp()

  bool Maybe_bit_ex_OR_exp( Token &parsedResult ) {
    if ( !Maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !Maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end Maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !Maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_bit_ex_OR_exp()
  
  bool Maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !Maybe_equality_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult );
      if ( !Maybe_equality_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_bit_AND_exp()
  
  bool Rest_of_maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_equality_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult );
      if ( !Maybe_equality_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_bit_AND_exp()
  
  bool Maybe_equality_exp( Token &parsedResult ) {
    if ( !Maybe_relational_exp( parsedResult ) ) return false;

    while ( mnextToken.type == EQ || mnextToken.type == NEQ ) {
      Match( mnextToken.type, parsedResult );  // 消耗 EQ 或 NEQ
      if ( !Maybe_relational_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end Maybe_equality_exp()
  
  bool Rest_of_maybe_equality_exp( Token &parsedResult ) {

    if ( !Rest_of_maybe_relational_exp( parsedResult ) ) return false;

    while ( mnextToken.type == EQ || mnextToken.type == NEQ ) {
      Match( mnextToken.type, parsedResult );  // 消耗 EQ 或 NEQ
      if ( !Maybe_relational_exp( parsedResult ) ) return false;
    } // end while
    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_equality_exp()

  bool Maybe_relational_exp( Token &parsedResult ) {
    if ( !Maybe_shift_exp( parsedResult ) ) return false;

    while ( mnextToken.type == GT || mnextToken.type == LT || 
            mnextToken.type == LE || mnextToken.type == GE ) {
      Match( mnextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !Maybe_shift_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_shift_exp( parsedResult ) ) return false;
    while ( mnextToken.type == GT || mnextToken.type == LT || 
            mnextToken.type == LE || mnextToken.type == GE ) {
      Match( mnextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !Maybe_shift_exp( parsedResult ) ) return false;
    } // end while
    
    return true;
  } // end Rest_of_maybe_relational_exp()

  bool Maybe_shift_exp( Token &parsedResult ) {
    if ( !Maybe_additive_exp( parsedResult ) ) return false;

    while ( mnextToken.type == LS || mnextToken.type == RS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 << 或 >>
      if ( !Maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_additive_exp( parsedResult ) ) return false;

    while ( mnextToken.type == LS || mnextToken.type == RS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 LS 或 RS
      if ( !Maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp( Token &parsedResult ) {
    if ( !Maybe_mult_exp( parsedResult ) ) return false;

    while ( mnextToken.type == PLUS || mnextToken.type == MINUS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 '+' 或 '-'
      if ( !Maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_mult_exp( parsedResult ) ) return false;
    
    while ( mnextToken.type == PLUS || mnextToken.type == MINUS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 '+' 或 '-'
      if ( !Maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Rest_of_maybe_additive_exp()

  bool Maybe_mult_exp( Token &parsedResult ) {
    // cout << mnextToken.tokenName << endl;
    if ( !Unary_exp( parsedResult ) ) return false;
    return Rest_of_maybe_mult_exp( parsedResult );  
  } // end Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp( Token &parsedResult ) {
    while ( mnextToken.type == MUL || mnextToken.type == DIV || mnextToken.type == MOD ) {
      Match( mnextToken.type, parsedResult );  
      if ( !Unary_exp( parsedResult ) ) return false;
    } // end while
    
    return true;  
  } // end Rest_of_maybe_mult_exp()

  bool Unary_exp( Token &parsedResult ) {
    if ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
      while ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
        Sign( parsedResult );
      } // end while
        
      return Signed_Unary_exp( parsedResult );
    } // end if
     
    else if ( mnextToken.type == IDENTIFIER || mnextToken.type == CONSTANT || mnextToken.type == LPAREN ) {
      return Unsigned_unary_exp( parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == PP || mnextToken.type == MM ) {
      Match( mnextToken.type, parsedResult ); // 消耗 PP 或 MM
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() &&
             gLocalsymbolTable.find( mnextToken.tokenName ) == gLocalsymbolTable.end() ) {
          parsedResult = mnextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SEMANTICERROR;

          return false;
        }  // end if 
        
        if ( mnextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult ); // 消耗 '['
          if ( !Expression( parsedResult ) ) return false; 
          if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
        } // end if

        return true;
      } // end if

      return false;
    } // end else if
    
    return false;
  } // end Unary_exp()

  bool Signed_Unary_exp( Token &parsedResult ) {
    if ( mnextToken.type == IDENTIFIER ) {
      if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() &&
           gLocalsymbolTable.find( mnextToken.tokenName ) == gLocalsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;

        return false;
      }  // end if 
        
      Match( IDENTIFIER, parsedResult );
      if ( mnextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        
        if ( mnextToken.type != RPAREN ) {
          if ( !Actual_parameter_list( parsedResult ) ) return false;
        } // end if
         
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( mnextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !Expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
      } // end else if
    
      return true;
    } // end if 
    
    else if ( mnextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      if ( !Expression( parsedResult ) ) return false;
      
      return Match( RPAREN, parsedResult ); // 消耗 ')'
    } // end else if
    
    return false;
  }  // end Signed_Unary_exp()

  bool Unsigned_unary_exp( Token &parsedResult ) {
    if ( mnextToken.type == IDENTIFIER ) {
      if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() &&
           gLocalsymbolTable.find( mnextToken.tokenName ) == gLocalsymbolTable.end() ) {

        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;

        return false;
      }  // end if 

      Match( IDENTIFIER, parsedResult );
      
      if ( mnextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        if ( mnextToken.type != RPAREN ) {
          if ( !Actual_parameter_list( parsedResult ) ) return false;
        } // end if
 
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( mnextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !Expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
        if ( mnextToken.type == PP || mnextToken.type == MM ) {
          Match( mnextToken.type, parsedResult );
        } // end if
      } // end else if
    
      return true;
    } // end if 
    
    else if ( mnextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      
      if ( !Expression( parsedResult ) ) return false;
      
      return Match( RPAREN, parsedResult ); // 消耗 ')'
    } // end else if
    
    return false;
  } // end Unsigned_unary_exp()

  bool Sign( Token &parsedResult ) {
    if ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
      return Match( mnextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end Sign()

  bool Match( Type expected, Token &parsedResult ) {
    if ( mnextToken.type == expected ) {
      parsedResult.content.push_back( mnextToken.tokenName );
      parsedResult.line = mnextToken.line;
      mnextToken = gtokenizer.GetNextToken();
      return true;
    } // end if 
    
    else {
      if ( parsedResult.type == ERROR ) {    } // end if

      else if ( mnextToken.type != ERROR ) {
        
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SYNTACTICALERROR;
      } // end else if
        
      else parsedResult = mnextToken;

      mnextToken = gtokenizer.GetNextToken();
      return false;
    } // end else
  } // end Match()
 
};

int main() {
  InitializegTypeNameMap();
  ginput = read_lines_from_file("input.txt");
  cout << "Our-C running ..." << endl ;
  
  Parser parse ;
  parse.Parse() ;
  cout << "Our-C exited ..." << endl ; 
  
} // end main()
