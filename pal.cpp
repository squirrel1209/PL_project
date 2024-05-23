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
  while ( cin.get( ch ) ) {               // �v��Ū����󤺮e
    line += ch;
    
    if ( ch == '\n' ) {
      if ( isFirstLine ) {
        // �p�G�O�Ĥ@��A�h���mline�ç��лx���A�A���N��K�[��minput_lines
        gtestnum = line ;
        line.clear();
        isFirstLine = false;
      } // end if
      
      else {
        minput_lines.push_back( line );
        line.clear();  // �M��line�H�Ω�U�@�檺Ū��
      } // end else
    } // end if 
  } // end while

  // �ˬd�̫�@��O�_�]����󵲧��ӨS������ŵ���
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

// �ϥ� map �Ӧs�x��ƦW�ٻP��h��w�q
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

// ��Ʃw�q�A�æs�x�� map ��
  void DefineFunction( string name, string returnType, vector<Variable> params, vector<string> body) {
    Function func;
    func.typeName = returnType;
    func.parameter = params;
    func.body = body;
    functionMap[name] = func;
  }

// �C�X�Ҧ���ƦW��
void ListAllFunctions() {
    cout << "Listing all functions:" << endl;
    map<string, Function>::const_iterator it;
    for ( it = functionMap.begin(); it != functionMap.end(); ++it ) {
        cout << it->first << "()" << endl;
    }
}

// �C�X���w��ƪ��w�q
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
    // �ˬd�U�@�ӲŸ��O�_�O���A���A�p�G�O�A�h�B�z����Ʃw�q
    if ( nextToken.type == LPAREN ) {
      function = true;
      return function_definition_without_ID( parsedResult  );
    } // end if
    
    else {
    // �p�G�U�@�ӲŸ����O���A���A�h�B�z���ܶq�ŧi
      return rest_of_declarators( parsedResult );
    } // end else
  } // end function_definition_or_declarators()

  bool rest_of_declarators( Token &parsedResult ) {
    // �ˬd�O�_�s�b '[' Constant ']' ���c
    if ( nextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );   // �ǰt '['
      
      if ( Match( CONSTANT, parsedResult ) ) 
        Match( RBRACKET, parsedResult );
      
      else return false;
    } // end if

    // �B�z { ',' Identifier [ '[' Constant ']' ] } ���c
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );
      
      if ( nextToken.type == IDENTIFIER ) {
        gIdToeknName.push_back( nextToken.tokenName );
        Match( IDENTIFIER, parsedResult ); // �ǰt���Ѳ�
        
        // �ˬd�O�_�s�b '[' Constant ']' ���c
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

    return Match( SEMICOLON, parsedResult );   // ��������
  } // end rest_of_declarators()

  bool function_definition_without_ID( Token &parsedResult ) {
    if ( !Match( LPAREN, parsedResult ) ) return false; 

    if ( nextToken.type == VOID ) {
      Match( VOID, parsedResult ); // �ǰt VOID
    } // end if

    else if ( type_specifier() ) 
      if ( !formal_parameter_list( parsedResult ) ) return false;// �ѪR formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return compound_statement( parsedResult ); // �ѪR compound_statement
  } // end function_definition_without_ID()
 
  bool formal_parameter_list( Token &parsedResult ) {
    Variable variable;
    Type type = nextToken.type;
    
    Match( nextToken.type, parsedResult ) ;
    
    if ( nextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult ); // �ǰt '&'
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
      Match( LBRACKET, parsedResult ); // �ǰt '['
      
      if ( Match( CONSTANT, parsedResult ) ) {
        if ( !Match( RBRACKET, parsedResult ) ) return false;
      } // end if 
      
      else return false;
    } // end if

    bool isformal_parameter_list = true;
    while ( nextToken.type == COMMA && isformal_parameter_list ) {
      Match( COMMA, parsedResult ); // �ǰt ','
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
        return false; // �Ϊ̶i���L���~�B�z
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

    return romce_and_romloe( parsedResult );  // �ѪR�Z?��?��
  } // end rest_of_PPMM_Identifier_started_basic_exp()

  bool actual_parameter_list( Token &parsedResult ) {
    if ( !basic_expression( parsedResult ) ) return false;
    
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // ���� ','
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
        Match( QUESTION, parsedResult );  // ���� '?'
        if ( !basic_expression( parsedResult ) ) return false;  // �ѪR?��?�u?����?��
        if ( !Match( COLON, parsedResult ) ) return false;  // ���� ':'
        if ( !basic_expression( parsedResult ) ) return false;  // �ѪR?��?��?����?��
    } // end if


    return true;
  } // end romce_and_romloe()

  bool rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?�z??�O��?��

    while ( nextToken.type == OR ) {
      Match( OR, parsedResult );  // ���� '||'
      if ( !maybe_logical_AND_exp( parsedResult ) ) return false;  // �ѪR??�O��?��
    } // end while
    
    return true;
  } // end rest_of_maybe_logical_OR_exp()

  bool maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Ĥ@?�i�઺��Ϊ�?��

    while (nextToken.type == AND) {
        Match( AND, parsedResult );  // ���� '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end maybe_logical_AND_exp()

  bool rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z��Ϊ�?�����ѧE����

    while ( nextToken.type == AND ) {
        Match( AND, parsedResult );  // ���� '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end rest_of_maybe_logical_AND_exp()

  bool maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?�z�Ĥ@?�i�઺���ݩΪ�?��

    while ( nextToken.type == BIT_OR ) {
      Match( BIT_OR, parsedResult );  // ���� '|'
      if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?�z�Z?�����ݩΪ�?��
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
      Match( nextToken.type, parsedResult );  // ���� EQ �� NEQ
      if ( !maybe_relational_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end maybe_equality_exp()
  
  bool rest_of_maybe_equality_exp(Token &parsedResult) {
  	
    if (!rest_of_maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // ���� EQ �� NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }
    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_equality_exp()

  bool maybe_relational_exp(Token &parsedResult) {
    if ( !maybe_shift_exp( parsedResult ) ) return false;

    while ( nextToken.type == GT || nextToken.type == LT || 
           nextToken.type == LE || nextToken.type == GE) {
      Match( nextToken.type, parsedResult );  // ���� <, >, <=, �� >=
      if ( !maybe_shift_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end maybe_relational_exp()

  bool rest_of_maybe_relational_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_shift_exp( parsedResult ) ) return false;
    while (nextToken.type == GT || nextToken.type == LT || 
           nextToken.type == LE || nextToken.type == GE) {
      Match( nextToken.type, parsedResult );  // ���� <, >, <=, �� >=
      if ( !maybe_shift_exp( parsedResult ) ) return false;
    } // end while
    return true;
  } // end rest_of_maybe_relational_exp()

  bool maybe_shift_exp( Token &parsedResult ) {
    if ( !maybe_additive_exp( parsedResult ) ) return false;

    while ( nextToken.type == LS || nextToken.type == RS ) {
      Match( nextToken.type, parsedResult );  // ���� << �� >>
      if ( !maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // maybe_shift_exp()

  bool rest_of_maybe_shift_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_additive_exp( parsedResult ) ) return false;

    while ( nextToken.type == LS || nextToken.type == RS ) {
        Match( nextToken.type, parsedResult );  // ���� LS �� RS
        if ( !maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_shift_exp()

  bool maybe_additive_exp( Token &parsedResult ) {
    if ( !maybe_mult_exp( parsedResult ) ) return false;

    while ( nextToken.type == PLUS || nextToken.type == MINUS ) {
      Match( nextToken.type, parsedResult );  // ���� '+' �� '-'
      if ( !maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end maybe_additive_exp()

  bool rest_of_maybe_additive_exp(Token &parsedResult) {
    if (!rest_of_maybe_mult_exp(parsedResult)) return false;
    
    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // ���� '+' �� '-'
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
      Match( nextToken.type, parsedResult ); // ���� PP �� MM
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
          if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
              parsedResult = nextToken;
              parsedResult.type = ERROR;
              parsedResult.error = SEMANTICERROR;
              return false;
          }  // end if 
        
        if ( nextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult ); // ���� '['
          if (!expression(parsedResult)) return false; 
          if (!Match(RBRACKET, parsedResult)) return false; // ���� ']'
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
         
        if ( !Match( RPAREN, parsedResult ) ) return false; // ���� ')'
      } // end if
      
      else if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // ���� ']'
      } // end else if
    
      return true;
    } // end if 
    
    else if ( nextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if (nextToken.type == LPAREN) {
      Match( LPAREN, parsedResult ); // ���� '('
      if ( !expression( parsedResult ) ) return false;
      return Match(RPAREN, parsedResult); // ���� ')'
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
 
        if ( !Match( RPAREN, parsedResult ) ) return false; // ���� ')'
      } // end if
      
      else if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // ���� ']'
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
      Match( LPAREN, parsedResult ); // ���� '('
      
      if ( !expression( parsedResult ) ) return false;
      
      return Match(RPAREN, parsedResult); // ���� ')'
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
