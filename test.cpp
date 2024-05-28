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
  while ( cin.get( ch ) ) {               // �v��Ū����󤺮e
    line += ch;
    
    if ( ch == '\n' ) {
      if ( isFirstLine ) {
        // �p�G�O�Ĥ@��A�h���mline�ç��лx���A�A���N��K�[��ginput_lines
        gtestnum = line ;
        line.clear();
        isFirstLine = false;
      } // end if
      
      else {
        ginput_lines.push_back( line );
        line.clear();  // �M��line�H�Ω�U�@�檺Ū��
      } // end else
    } // end if 
  } // end while

  // �ˬd�̫�@��O�_�]����󵲧��ӨS������ŵ���
  if ( !line.empty() ) {
    ginput_lines.push_back( line );
  } // end if
  
  return ginput_lines;
} // end Read_lines()

vector<string> read_lines_from_file( string filename ) {

  vector<string> input_lines ;
  ifstream file(filename.c_str()) ;                            // ���}���
    
  if ( file.is_open() ) {
    string line;         
    while ( getline( file, line ) ) {               // �v��Ū����󤺮e
      input_lines.push_back( line );
    } // end while
         
    file.close();                                  // �������
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
  LEXICALERROR,      // ����k���R���J��@�ӥ��L�k�ѧO�����󦳮ĲŸ����r�Ůɳ��i�����~�C
  SYNTACTICALERROR,  // ��y�k���R���b���R�Ÿ��ǦC�ɹJ��@�Ӥ��ŦX��e�y�k�W�h���Ÿ��ɳ��i�����~�C
  SEMANTICERROR,     // ��y�q���R���b�ˬd�{�����y�q�ɵo�{�ϥΤF���w�q�����ѲŮɳ��i�����~�C
  OTHERERROR         // ��ܨ�L���~
};

struct Token {
  string tokenName;  // �аO�W��
  Type type;         // �аO����
  int line;          // �аO�Ҧb�����
  ErrorType error;       // ���p�����~�H��
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

// �ϥ� map �Ӧs�x��ƦW�ٻP��h��w�q
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

// ��Ʃw�q�A�æs�x�� map ��
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

// �C�X�Ҧ���ƦW��
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

// �C�X���w��ƪ��w�q
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
    // �j�M��
  map<string, Type>::iterator it = symbolTable.find( keyToRemove );
  if ( it != symbolTable.end() ) {
        // �Y���A�h�R��
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
  } // end GetKeywordType() 

  Token GetIdentOrKeyword( char nextChar ) {
    string id = "";
    
    while ( ( isalnum( nextChar ) || nextChar == '_' ) && nextChar != '\0' ) {
      id += AnyToString( nextChar );
      nextChar = GetNextChar();
    } // end while
    
    // �N�U�@�ӫD�ѧO�Ū��r�ũ�^
    PreviousChar();
    
    // �ˬd�O�_������r
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

  bool IsDelimiter( char ch ) {
    // �ˬd�O�_����r�Ť��j�ũΥi�ন���h�r�žާ@�Ū��Ĥ@�Ӧr��
    return ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
         ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^' ||
         ch == '>' || ch == '<' || ch == '=' || ch == '!' || ch == '&' || ch == '|' ||
         ch == ';' || ch == ',' || ch == '?' || ch == ':';
  } // end IsDelimiter() 

  Token GetDelimiter( char ch ) {
    Type tokenType = ERROR;  // �w�]��ERROR�A�b�U��������y�y���|�Q�л\
    
    string tokenValue = string( 1, ch );  // ��l�]�mtoken�Ȭ���e�r��
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
    // ��^�Ыت�Token
    return CreatToken( tokenValue, mlineIndex, tokenType, OTHERERROR );
  } // end GetDelimiter()

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
      mcolumnIndex = ginput[mlineIndex].size() ;
    } // end if
    
    else mcolumnIndex-- ;
  } // end PreviousChar()

  char GetNextChar() {
  // GetNextChar() �q�@�ӵ��w����J�]�q�`�O�@�Ӧr�Ŧ�V�q�A�C�Ӥ����N���N�X���@��^������Ū���U�@�Ӧr�šC
  // ���ƹF���e�檺�����ɡA���|�۰����U�@�檺�}�l�C
  // �p�G��ƹF���J�奻�������A���|��^�@�ӪŦr��('\0')��ܨS����h���r�ťi�HŪ���C
  
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
    // �ˬd�U�@�ӲŸ��O�_�O���A���A�p�G�O�A�h�B�z����Ʃw�q
    if ( mnextToken.type == LPAREN ) {
      function = true;
      return Function_definition_without_ID( parsedResult  );
    } // end if
    
    else {
    // �p�G�U�@�ӲŸ����O���A���A�h�B�z���ܶq�ŧi
      return Rest_of_declarators( parsedResult );
    } // end else
  } // end Function_definition_or_declarators()

  bool Rest_of_declarators( Token &parsedResult ) {
    // �ˬd�O�_�s�b '[' Constant ']' ���c
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );   // �ǰt '['
      
      if ( Match( CONSTANT, parsedResult ) ) 
        Match( RBRACKET, parsedResult );
      
      else return false;
    } // end if

    // �B�z { ',' Identifier [ '[' Constant ']' ] } ���c
    while ( mnextToken.type == COMMA ) {
      Match( COMMA, parsedResult );

      if ( mnextToken.type == IDENTIFIER ) {
        gIdToeknName.push_back( mnextToken.tokenName );
        Match( IDENTIFIER, parsedResult ); // �ǰt���Ѳ�
        
        // �ˬd�O�_�s�b '[' Constant ']' ���c
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

    return Match( SEMICOLON, parsedResult );   // ��������
  } // end Rest_of_declarators()

  bool Function_definition_without_ID( Token &parsedResult ) {
    if ( !Match( LPAREN, parsedResult ) ) return false; 

    if ( mnextToken.type == VOID ) {
      Match( VOID, parsedResult ); // �ǰt VOID
    } // end if

    else if ( Type_specifier() ) 
      if ( !Formal_parameter_list( parsedResult ) ) return false; // �ѪR Formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return Compound_Statement( parsedResult ); // �ѪR Compound_Statement
  } // end Function_definition_without_ID()
 
  bool Formal_parameter_list( Token &parsedResult ) {
    Variable variable;
    Type type = mnextToken.type;
    
    Match( mnextToken.type, parsedResult ) ;
    
    if ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult ); // �ǰt '&'
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
      Match( LBRACKET, parsedResult ); // �ǰt '['
      
      if ( Match( CONSTANT, parsedResult ) ) {
        if ( !Match( RBRACKET, parsedResult ) ) return false;
      } // end if 
      
      else return false;
    } // end if

    bool isFormal_parameter_list = true;
    while ( mnextToken.type == COMMA && isFormal_parameter_list ) {
      Match( COMMA, parsedResult ); // �ǰt ','
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
        return false; // �Ϊ̶i���L���~�B�z
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

    return Romce_and_romloe( parsedResult );  // �ѪR�Z?��?��
  } // end Rest_of_PPMM_Identifier_started_basic_exp()

  bool Actual_parameter_list( Token &parsedResult ) {
    if ( !Basic_Expression( parsedResult ) ) return false;
    
    while ( mnextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // ���� ','
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
      Match( QUESTION, parsedResult );  // ���� '?'
      if ( !Basic_Expression( parsedResult ) ) return false;  // �ѪR?��?�u?����?��
      if ( !Match( COLON, parsedResult ) ) return false;  // ���� ':'
      if ( !Basic_Expression( parsedResult ) ) return false;  // �ѪR?��?��?����?��
    } // end if


    return true;
  } // end Romce_and_romloe()

  bool Rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?�z??�O��?��

    while ( mnextToken.type == OR ) {
      Match( OR, parsedResult );  // ���� '||'
      if ( !Maybe_logical_AND_exp( parsedResult ) ) return false;  // �ѪR??�O��?��
    } // end while
    
    return true;
  } // end Rest_of_maybe_logical_OR_exp()

  bool Maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Ĥ@?�i�઺��Ϊ�?��

    while ( mnextToken.type == AND ) {
      Match( AND, parsedResult );  // ���� '&&'
      if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z��Ϊ�?�����ѧE����

    while ( mnextToken.type == AND ) {
      Match( AND, parsedResult );  // ���� '&&'
      if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end Rest_of_maybe_logical_AND_exp()

  bool Maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?�z�Ĥ@?�i�઺���ݩΪ�?��

    while ( mnextToken.type == BIT_OR ) {
      Match( BIT_OR, parsedResult );  // ���� '|'
      if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?�z�Z?�����ݩΪ�?��
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
      Match( mnextToken.type, parsedResult );  // ���� EQ �� NEQ
      if ( !Maybe_relational_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end Maybe_equality_exp()
  
  bool Rest_of_maybe_equality_exp( Token &parsedResult ) {

    if ( !Rest_of_maybe_relational_exp( parsedResult ) ) return false;

    while ( mnextToken.type == EQ || mnextToken.type == NEQ ) {
      Match( mnextToken.type, parsedResult );  // ���� EQ �� NEQ
      if ( !Maybe_relational_exp( parsedResult ) ) return false;
    } // end while
    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_equality_exp()

  bool Maybe_relational_exp( Token &parsedResult ) {
    if ( !Maybe_shift_exp( parsedResult ) ) return false;

    while ( mnextToken.type == GT || mnextToken.type == LT || 
            mnextToken.type == LE || mnextToken.type == GE ) {
      Match( mnextToken.type, parsedResult );  // ���� <, >, <=, �� >=
      if ( !Maybe_shift_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_shift_exp( parsedResult ) ) return false;
    while ( mnextToken.type == GT || mnextToken.type == LT || 
            mnextToken.type == LE || mnextToken.type == GE ) {
      Match( mnextToken.type, parsedResult );  // ���� <, >, <=, �� >=
      if ( !Maybe_shift_exp( parsedResult ) ) return false;
    } // end while
    
    return true;
  } // end Rest_of_maybe_relational_exp()

  bool Maybe_shift_exp( Token &parsedResult ) {
    if ( !Maybe_additive_exp( parsedResult ) ) return false;

    while ( mnextToken.type == LS || mnextToken.type == RS ) {
      Match( mnextToken.type, parsedResult );  // ���� << �� >>
      if ( !Maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_additive_exp( parsedResult ) ) return false;

    while ( mnextToken.type == LS || mnextToken.type == RS ) {
      Match( mnextToken.type, parsedResult );  // ���� LS �� RS
      if ( !Maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp( Token &parsedResult ) {
    if ( !Maybe_mult_exp( parsedResult ) ) return false;

    while ( mnextToken.type == PLUS || mnextToken.type == MINUS ) {
      Match( mnextToken.type, parsedResult );  // ���� '+' �� '-'
      if ( !Maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_mult_exp( parsedResult ) ) return false;
    
    while ( mnextToken.type == PLUS || mnextToken.type == MINUS ) {
      Match( mnextToken.type, parsedResult );  // ���� '+' �� '-'
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
      Match( mnextToken.type, parsedResult ); // ���� PP �� MM
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() &&
             gLocalsymbolTable.find( mnextToken.tokenName ) == gLocalsymbolTable.end() ) {
          parsedResult = mnextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SEMANTICERROR;

          return false;
        }  // end if 
        
        if ( mnextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult ); // ���� '['
          if ( !Expression( parsedResult ) ) return false; 
          if ( !Match( RBRACKET, parsedResult ) ) return false; // ���� ']'
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
         
        if ( !Match( RPAREN, parsedResult ) ) return false; // ���� ')'
      } // end if
      
      else if ( mnextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !Expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // ���� ']'
      } // end else if
    
      return true;
    } // end if 
    
    else if ( mnextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult ); // ���� '('
      if ( !Expression( parsedResult ) ) return false;
      
      return Match( RPAREN, parsedResult ); // ���� ')'
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
 
        if ( !Match( RPAREN, parsedResult ) ) return false; // ���� ')'
      } // end if
      
      else if ( mnextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !Expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // ���� ']'
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
      Match( LPAREN, parsedResult ); // ���� '('
      
      if ( !Expression( parsedResult ) ) return false;
      
      return Match( RPAREN, parsedResult ); // ���� ')'
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
