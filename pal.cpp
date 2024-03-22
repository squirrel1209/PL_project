# include <iostream>
# include <stdio.h>
# include <cstdlib>
# include <vector>
# include <cctype> 
# include <string>
# include <map>
# include <cmath>

using namespace std;
typedef char String200[200];
string gtestnum ;

// ��ƥΩ�q���Ū�����e�ê�^�@�Ӧs�x�檺�V�q
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

map<string, Token> gsymbolTable;

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


class Parser {
public:
  Parser( vector<Token> inputmtokens ) {
    mtokens = inputmtokens;
    mcurrentTokenIndex = 0;
  } // end Parser()

  // ���R�óB�z��J�� token�C  
  void Parse() {
    Token parsedResult ;
    Command( parsedResult ) ;

    while ( parsedResult.type != QUIT ) {

      if ( parsedResult.type == ERROR ) {
        if ( parsedResult.error.type == LEXICALERROR )
          cout << "> Unrecognized token with first char : '" << parsedResult.error.errorValue << "'" << endl;

        else if ( parsedResult.error.type == SYNTACTICALERROR )
          cout << "> Unexpected token : '" << parsedResult.error.errorValue << "'" << endl;

        else if ( parsedResult.error.type == SEMANTICERROR )
          cout << "> Undefined identifier : '" << parsedResult.error.errorValue << "'" << endl;

        else cout << "> Error" << endl ;

        int temp = parsedResult.line;
        while ( GetCurrentToken().line > temp ) mcurrentTokenIndex-- ;
        while ( GetCurrentToken().line <= temp ) {
          Match();
          if ( GetCurrentToken().type == QUIT ) return ;
        } // end while
      } // end if

      else if ( parsedResult.type == FLOAT ) {
        float answer = atof( parsedResult.tokenName.c_str() ) ;
        float roundedAnswer = round( answer * 1000 ) / 1000;
        printf( "> %.3f\n", roundedAnswer ) ;
      } // end else if

      else cout << "> " << parsedResult.tokenName << endl;

      Error err;
      parsedResult = CreateToken( "", ERROR, err ) ;
      Command( parsedResult ) ;
    } // end while
  } // end Parse() 

private:
  vector<Token> mtokens;
  int mcurrentTokenIndex;

  bool IsNearlyDivisible( float a, float b, float epsilon ) {
    float result = a / b;
    float fractionalPart = result - floor( result ); // ����p�Ƴ���
    return fabs( fractionalPart ) < epsilon || fabs( fractionalPart - 1 ) < epsilon;
  } // end IsNearlyDivisible()

  Token EvaluateOperation( Token a, Token b, Type op ) {
    Token answer ;
    double floatA = atof( a.tokenName.c_str() );
    double floatB = atof( b.tokenName.c_str() );
    double floatAnswer ;

    if ( b.tokenName.compare( "0" ) == 0 && op == DIVIDE ) {
      answer.type = ERROR;
      answer.error.type = OTHERERROR;
      return answer;
    } // end if

    if ( op == MULTIPLY ) {
      floatAnswer = floatA * floatB;
    } // end if

    else if ( op == DIVIDE ) {
      floatAnswer = floatA / floatB;
    } // end else if

    else if ( op == PLUS ) {
      floatAnswer = floatA + floatB;
    } // end else if

    else if ( op == MINUS ) {
      floatAnswer = floatA - floatB;
    } // end else if

    if ( op != DIVIDE && a.type == INT && b.type == INT ) {
      int intAnswer = ( int ) floatAnswer;
      answer.tokenName = AnyToString( intAnswer ) ;
      answer.type = INT;
      return answer;
    } // end if

    else if ( op == DIVIDE ) {
      if ( IsNearlyDivisible( floatA, floatB, 0.0001 ) ) {  // �㰣 
        int intAnswer = ( int ) floatAnswer;
        answer.tokenName = AnyToString( intAnswer ) ;
        answer.type = INT;
        return answer;
      } // end if

      else {
        answer.tokenName = AnyToString( floatAnswer ) ;
        answer.type = FLOAT; // Assuming FLOAT as the type for floating point numbers
        return answer;
      } // end else
    } // end else if

    else {
      answer.tokenName = AnyToString( floatAnswer ) ;
      answer.type = FLOAT; // Assuming FLOAT as the type for floating point numbers
      return answer;
    } // end else
  } // end EvaluateOperation()
    
  Token CompareOperation( Token left, Token right, Type op ) {
    Token result;
    result.type = BOOL;  // ���] Token ���@�� BOOL �����Ω��ܥ��L��

    // �N�r�����ഫ���ƭȶi����
    double leftVal = atof( left.tokenName.c_str() );
    double rightVal = atof( right.tokenName.c_str() );
    bool comparisonResult;


    if ( op == EQUAL ) {
      comparisonResult = fabs( leftVal - rightVal ) < 0.0001;
      result.tokenName = comparisonResult ? "true" : "false";
      return result;
    } // end if

    else if ( op == NOTEQUAL ) {
      comparisonResult = leftVal != rightVal;
      result.tokenName = comparisonResult ? "true" : "false";
      return result;
    } // end else if 

    else if ( op == GREATER ) {
      comparisonResult = leftVal > rightVal;
      result.tokenName = comparisonResult ? "true" : "false";
      return result;
    } // end else if 

    else if ( op == LESS ) {
      comparisonResult = leftVal < rightVal;
      result.tokenName = comparisonResult ? "true" : "false";
      return result;
    } // end else if 

    else if ( op == GREATEREQUAL ) {
      comparisonResult = leftVal >= rightVal;
      result.tokenName = comparisonResult ? "true" : "false";
      return result;
    } // end else if

    else if ( op == LESSEQUAL ) {
      comparisonResult = leftVal <= rightVal;
      result.tokenName = comparisonResult ? "true" : "false";
      return result;
    } // end else if

    else return result;
  } // end CompareOperation()

  // �ѪR��e�R�O�ç�s parsedResult �ѼƥH�ϬM�R�O�����G�C
  void Command( Token &parsedResult ) {
  // - **QUIT �M ERROR �B�z�G** �p�G��e token �� QUIT �� ERROR�A�h������s `parsedResult`�C
  // - **���ѲũM��ȡG** �B�z���ѲũM��᪺��
    Token currentToken = GetCurrentToken();
    if ( currentToken.type == QUIT || currentToken.type == ERROR ) {
      parsedResult = currentToken;
      Match();
    } // end if

    else if ( currentToken.type == IDENT ) {
      Token ident = currentToken;
      Match();  // ���ʨ�U�@�� token�A�Y ASSIGN

      if ( ident.tokenName.compare( "quit" ) == 0 ) {
        parsedResult = ident;
        parsedResult.type = QUIT;
        return;
      } // end if

      if ( gsymbolTable.find( ident.tokenName ) != gsymbolTable.end() ) {  // ��define
        Token deFineident = gsymbolTable[ident.tokenName];
        
        if ( CurrentTokenType() == SEMICOLON ) {    // IDENT ;
          parsedResult = deFineident;
          Match();
          return;
        } // end if
        
        else if ( CurrentTokenType() == ASSIGN ) {  // IDENT ':=' <ArithExp>
          Match();  // ���ʨ��Ȫ�F�� 

          if ( ArithExp( parsedResult ) ) {
            if ( CurrentTokenType() == SEMICOLON ) {
              // �N��N��F�������G��ȵ����Ѳ�
              gsymbolTable[ident.tokenName] = parsedResult ;
              Match();  // ���ӱ�����
              return ;
            } // end if

            else {
              if ( GetCurrentToken().type == ERROR ) {
                parsedResult = GetCurrentToken();
                Match();
                return;
              } // end if

              else {
                parsedResult.type = ERROR;
                parsedResult.error.type = SYNTACTICALERROR;
                parsedResult.error.errorValue = GetCurrentToken().tokenName;
                parsedResult.line = GetCurrentToken().line;
                Match();
                return ;
              } // end else
            } // end else
          }  // end if
        } // end else if
        
        else if ( IDlessArithExpOrBexp( deFineident, parsedResult ) ) {
          if ( CurrentTokenType() == SEMICOLON ) {
            Match();
            return;
          } // end if

          else {
            if ( CurrentTokenType() == ERROR ) {
              parsedResult = GetCurrentToken();
              Match();
              return;
            } // end if

            else {
              parsedResult.type = ERROR;
              parsedResult.error.type = SYNTACTICALERROR;
              parsedResult.error.errorValue = GetCurrentToken().tokenName;
              parsedResult.line = GetCurrentToken().line;
              Match();
              return ;
            } // end else
          }  // end else
        } // end else if
      } // end if
      
      else {          // �S��define
        if ( CurrentTokenType() == ASSIGN ) {  // IDENT ':=' <ArithExp>
          Match();  // ���ʨ��Ȫ�F�� 

          if ( ArithExp( parsedResult ) ) {
            if ( CurrentTokenType() == SEMICOLON ) {
              // �N��N��F�������G��ȵ����Ѳ�
              gsymbolTable[ident.tokenName] = parsedResult ;
              Match();  // ���ӱ�����
              return ;
            } // end if

            else {
              if ( GetCurrentToken().type == ERROR ) {
                parsedResult = GetCurrentToken();
                Match();
                return;
              } // end if

              else {
                parsedResult.type = ERROR;
                parsedResult.error.type = SYNTACTICALERROR;
                parsedResult.error.errorValue = GetCurrentToken().tokenName;
                parsedResult.line = GetCurrentToken().line;
                Match();
                return ;
              } // end else
            } // end else
          }  // end if
        } // end else if
        
        else if ( CurrentTokenType() == SEMICOLON || CurrentTokenType() == SIGN ||
                  CurrentTokenType() == MULTIPLY || CurrentTokenType() == MULTIPLY ||
                  BooleanOperator() ) {

          parsedResult = ident;
          parsedResult.type = ERROR;
          parsedResult.error.type = SEMANTICERROR;
          parsedResult.error.errorValue = ident.tokenName;
          parsedResult.line = ident.line;
          Match();
          return;
        } // end else if
        
        else {    // ��k���~ 
          if ( CurrentTokenType() == ERROR ) {
            parsedResult = GetCurrentToken();
            Match();
            return ;
          } // end if

          else {
            parsedResult.type = ERROR;
            parsedResult.error.type = SYNTACTICALERROR;
            parsedResult.error.errorValue = GetCurrentToken().tokenName;
            parsedResult.line = currentToken.line;
            Match();
            return ;
          } // end else
          
        } // end else
        
      } // end else
      
    } // end else if

    else if ( NOT_IDStartArithExpOrBexp( parsedResult ) ) {
      if ( CurrentTokenType() == SEMICOLON ) {
        Match();
        return;
      } // end if

      else {
        if ( CurrentTokenType() == ERROR ) {
          parsedResult = GetCurrentToken();
          Match();
          return;
        } // end if

        else if ( parsedResult.error.type == OTHERERROR ) {
          Match();
          return;
        } // end else if

        else {
          parsedResult.type = ERROR;
          parsedResult.error.type = SYNTACTICALERROR;
          parsedResult.error.errorValue = GetCurrentToken().tokenName;
          parsedResult.line = GetCurrentToken().line;
          Match();
          return ;
        } // end else
      } // end else
    } // end else if 
    
  } // end Command()

  // �ˬd�øѪR�D���ѲŶ}�Y����N�Υ�����F���C
  bool NOT_IDStartArithExpOrBexp( Token &parsedResult ) {
  // - **��N��F���ѪR�G** �����ѪR��N��F���A�p�G��F�����H���ѲŶ}�Y�A�h�i��o�@�B�C
  // - **�����B��ѪR�G** �b��N��F����A�ˬd�øѪR�i�઺�����B���F���C
  // - **����B��B�z�G** �p�G�������B��šA�h�i�����������B��A�ç�s `parsedResult`�C

    if ( NOT_ID_StartArithExp( parsedResult ) ) {
      // �ѪR���\��A�ˬd�O�_���H�F�����B���

      if ( BooleanOperator() ) {
        Token operatorToken = GetCurrentToken();
        Match(); // ���ӥ����B���
        Token rightOperand;
        if ( ArithExp( rightOperand ) ) {
          parsedResult = CompareOperation( parsedResult, rightOperand, operatorToken.type );
          return true;
        } // end if

        else {
          parsedResult = rightOperand;
          return false;
        } // end else
      } // end if

      else return true;
    }  // end if

    else return false;
  } // end NOT_IDStartArithExpOrBexp()

  bool NOT_ID_StartArithExp( Token &parsedResult ) {
  // - **��B�ѪR�G** �����ѪR��N��F�����Ĥ@�� Term�A�p�G�ѪR���ѫh��ƪ�^ false�C
  // - **�B��B�z�G** �Y�J��[���δ�A�N�i���������N�B��C
  // - **���N�ѪR�G** �`���B�z���� Term�A�����F�������C
  // - **���G��s�G** �ھڹB���s parsedResult ���ȡC

    // �ѪR�Ĥ@�� Term
    if ( !NOT_ID_StartTerm( parsedResult ) ) {
      return false;
    } // end if

    // ���e token �O�[���δ�ɡA�i��B��
    while ( CurrentTokenType() == SIGN ) {
      // �O�s�B���
      Type op;
      if ( GetCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
      else op = PLUS ;
      Match();  // ���ʨ�U�@�� token

      // �ѪR�U�@�� Term
      Token b;

      if ( !Term( b ) ) {
        parsedResult = b; 
        return false;
      } // end if

      // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while

    // ��^ true ��ܦ��\�ѪR ArithExp
    return true;
  } // end NOT_ID_StartArithExp()

  bool NOT_ID_StartTerm( Token &parsedResult ) {
  // - **��B�ѪR�G** �ѪR�}�Y�� Factor�A�p�G�o�@�B���ѡA��� Term ���ѪR�]�N���ѡC
  // - **�����B��B�z�G** �~��ѪR�J�쪺���k�ΰ��k�B��šA�ù�������ާ@�ƶi��B��C
  // - **���N�ѪR�G** �`���B�z Term �����Ҧ����k�M���k�B��C
  // - **���G��s�G** �ھڹB���s `parsedResult` ���ȡC

    // �����ѪR�@�� Factor
    if ( !NOT_ID_StartFactor( parsedResult ) ) {
      return false;
    } // end if

    // ���e token �O���k�ΰ��k�B��ŮɡA�i��B��
    while ( CurrentTokenType() == MULTIPLY || CurrentTokenType() == DIVIDE ) {
      // �O�s�B���
      Type op = CurrentTokenType();
      Match();  // ���ʨ�U�@�� token

      // �ѪR�U�@�� Factor
      Token b;
      if ( !Factor( b ) ) {
        parsedResult = b ;
        return false;
      } // end if

      // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while

    // ��^ true ��ܦ��\�ѪR Term
    return true;
  } // end NOT_ID_StartTerm()

  // �ѪR���H���ѲŶ}�Y���]�l(Factor)�C 
  bool NOT_ID_StartFactor( Token &parsedResult ) {
  // - **�����ƭȡG** ����ѪR�������X����ƩίB�I�ơC
  // - **�Ÿ��B�z�G** ����ѧO�óB�z�ƭȫe�����t�Ÿ��C
  // - **�A����F���G** ����B�z�A��������N��F���C
  // - **���~�B�z�G** ��J��y�k���~�ɡA�|�]�m���������~�H���ê�^ false�C    
    Token currentToken = GetCurrentToken();

    if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
      parsedResult = currentToken;
      Match();  // Match ERROR || QUIT
      return false ;
    } // end if

    else if ( currentToken.type == SIGN ) {
      if ( currentToken.tokenName.compare( "-" ) == 0 )
        parsedResult = currentToken ;

      Match();
      currentToken = GetCurrentToken(); // ��s Token ����

      if ( currentToken.type == INT || currentToken.type == FLOAT ) {
        parsedResult.tokenName += currentToken.tokenName;
        parsedResult.type = currentToken.type ;
        Match();
        return true ;
      } // end if

      else {
        if ( CurrentTokenType() == ERROR ) {
          parsedResult = GetCurrentToken();
          Match();
          return false;
        } // end if

        else {
          parsedResult.type = ERROR;
          parsedResult.error.type = SYNTACTICALERROR;
          parsedResult.error.errorValue = GetCurrentToken().tokenName;
          parsedResult.line = GetCurrentToken().line;
          Match();
          return false;
        } // end else
      } // end else
    } // end else if

    else if ( currentToken.type == INT || currentToken.type == FLOAT ) {
      parsedResult = currentToken ;
      Match();
      return true ;
    } // end else if

    else if ( currentToken.type == LPAREN ) {
      Match();

      if ( ArithExp( parsedResult ) ) {
        currentToken = GetCurrentToken();
        if ( currentToken.type == RPAREN ) {
          Match();
          return true ;
        } // end if

        else {
          if ( CurrentTokenType() == ERROR ) {
            parsedResult = GetCurrentToken();
            Match();
            return false;
          } // end if

          else {
            parsedResult.type = ERROR;
            parsedResult.error.type = SYNTACTICALERROR;
            parsedResult.error.errorValue = GetCurrentToken().tokenName;
            parsedResult.line = GetCurrentToken().line;
            Match();
            return false;
          } // end else
        } // end else
      } // end if 

      else {
        Match();
        return false ;
      } // end else 
    } // end else if 

    else {
      if ( CurrentTokenType() == ERROR ) {
        parsedResult = GetCurrentToken();
        Match();
        return false;
      } // end if

      else {
        parsedResult.type = ERROR;
        parsedResult.error.type = SYNTACTICALERROR;
        parsedResult.error.errorValue = GetCurrentToken().tokenName;
        parsedResult.line = GetCurrentToken().line;
        Match();
        return false;
      } // end else
    } // end else
  } // end NOT_ID_StartFactor()

    // <IDlessArithExpOrBexp>  ::= {   '+' <Term>   | '-' <Term> | '*' <Factor> | '/' <Factor> }
    //                             [ <BooleanOperator> <ArithExp> ]
  bool IDlessArithExpOrBexp( Token ident, Token &parsedResult ) {
    while ( CurrentTokenType() == SIGN || CurrentTokenType() == MULTIPLY || CurrentTokenType() == DIVIDE ) {
      Type op;
      if ( GetCurrentToken().tokenName.compare( "+" ) == 0 ) op = PLUS;
      else if ( GetCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
      else op = CurrentTokenType();

      Match();  // ���ʨ�U�@�� token�A�Y Term �� Factor ���}�l

      if ( op == PLUS || op == MINUS ) {
        if ( !Term( parsedResult ) ) {
          return false;
        } // end if

        parsedResult = EvaluateOperation( ident, parsedResult, op ); // ����B��
      } // end if

      else if ( op == MULTIPLY || op == DIVIDE ) {
        if ( !Factor( parsedResult ) ) {
          return false;
        } // end if
        
        parsedResult = EvaluateOperation( ident, parsedResult, op ); // ����B��
      } // end else if

      ident = parsedResult ;
    } // end while

    if ( BooleanOperator() ) {
      Type op = CurrentTokenType();
      Match();  // ���ʨ� Boolean ��F�����}�l

      Token nextOperand;
      if ( !ArithExp( nextOperand ) ) {
        parsedResult = nextOperand;
        return false;
      } // end if

      parsedResult = CompareOperation( parsedResult, nextOperand, op ); // ���楬�L�B��
    } // end if

    return true;
  } // end IDlessArithExpOrBexp()

  bool BooleanOperator() {
    if ( CurrentTokenType() == EQUAL || CurrentTokenType() == NOTEQUAL || CurrentTokenType() == GREATER ||
         CurrentTokenType() == LESS || CurrentTokenType() == GREATEREQUAL || 
         CurrentTokenType() == LESSEQUAL )
      return true ;
    else return false ;
  } // end BooleanOperator()

  // <BooleanExp> ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
  bool BooleanExp( Token &parsedResult ) {
    if ( ArithExp( parsedResult ) ) {
      if ( BooleanOperator() ) {
        Type op = CurrentTokenType();  // �������B���
        Match();

        Token rightExpr;
        if ( ArithExp( rightExpr ) ) {
          // �����Ӻ�N��F�������G
          // ���B���Ӧ�����޿誺��{�A�o�̶ȥܽd���c
          parsedResult = CompareOperation( parsedResult, rightExpr, op );
          return true;
        } // end if

        else {
          parsedResult = rightExpr ;
          return false ;
        } // end else
      } // end if

      else {
        if ( CurrentTokenType() == ERROR ) {
          parsedResult = GetCurrentToken();
          Match();
          return false;
        } // end if

        else {
          parsedResult.type = ERROR;
          parsedResult.error.type = SYNTACTICALERROR;
          parsedResult.error.errorValue = GetCurrentToken().tokenName;
          parsedResult.line = GetCurrentToken().line;
          Match();
          return false;
        } // end else
      } // end else
    } // end if

    else {
      return false ;
    } // end else
  } // end BooleanExp()

  // <ArithExp> ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
  bool ArithExp( Token &parsedResult ) {
    Token currentToken = GetCurrentToken();

    if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
      parsedResult = currentToken;
      Match();  // Match ERROR || QUIT
      return false ;
    } // end if

    // �ѪR�Ĥ@�� Term
    if ( !Term( parsedResult ) ) {
      return false;
    } // end if

    // ���e token �O�[���δ�ɡA�i��B��
    while ( CurrentTokenType() == SIGN ) {
      // �O�s�B���
      Type op;
      if ( GetCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
      else op = PLUS ;
      Match();  // ���ʨ�U�@�� token

      // �ѪR�U�@�� Term
      Token b;

      if ( !Term( b ) ) {
        parsedResult = b;
        return false;
      } // end if

      // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while 

    // ��^ true ��ܦ��\�ѪR ArithExp
    return true;
  } // end ArithExp()

  // <Term> ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
  bool Term( Token &parsedResult ) {
    Token currentToken = GetCurrentToken();

    if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
      parsedResult = currentToken;
      Match();  // Match ERROR || QUIT
      return false ;
    } // end if

    // �����ѪR�@�� Factor
    if ( !Factor( parsedResult ) ) {
      return false;
    } // end if

    // ���e token �O���k�ΰ��k�B��ŮɡA�i��B��
    while ( CurrentTokenType() == MULTIPLY || CurrentTokenType() == DIVIDE ) {
      // �O�s�B���
      Type op = CurrentTokenType();
      Match();  // ���ʨ�U�@�� token

      // �ѪR�U�@�� Factor
      Token b;
      if ( !Factor( b ) ) {
        parsedResult = b ;
        return false;
      } // end if

      // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while

    // ��^ true ��ܦ��\�ѪR Term
    return true;
  } // end Term()

  // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'
  bool Factor( Token &parsedResult ) {
    Token currentToken = GetCurrentToken();
    if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
      parsedResult = currentToken;
      Match();  // Match ERROR || QUIT
      return false ;
    } // end if

    if ( currentToken.type == IDENT ) {
      if ( gsymbolTable.find( currentToken.tokenName ) != gsymbolTable.end() ) {
        parsedResult = gsymbolTable[currentToken.tokenName];
        Match();  // Factor
        return true ;
      } // end if

      else {
        // undefined identifier
        if ( CurrentTokenType() == ERROR ) {
          parsedResult = GetCurrentToken();
          Match();
          return false;
        } // end if

        else {
          parsedResult.type = ERROR;
          parsedResult.error.type = SEMANTICERROR;
          parsedResult.error.errorValue = currentToken.tokenName;
          parsedResult.line = currentToken.line;
          Match();
          return false;
        } // end else
      } // end else
    } // end if

    else if ( currentToken.type == SIGN ) {
      if ( currentToken.tokenName.compare( "-" ) == 0 )
        parsedResult = currentToken ;

      Match();
      currentToken = GetCurrentToken(); // ��s Token ����

      if ( currentToken.type == INT || currentToken.type == FLOAT ) {
        parsedResult.tokenName += currentToken.tokenName;
        parsedResult.type = currentToken.type ;
        Match();
        return true ;
      } // end if

      else {
        if ( currentToken.type == ERROR ) {
          parsedResult = currentToken;
          Match();
          return false;
        } // end if

        else {
          parsedResult.type = ERROR;
          parsedResult.error.type = SYNTACTICALERROR;
          parsedResult.error.errorValue = currentToken.tokenName;
          parsedResult.line = currentToken.line;
          Match();
          return false ;
        } // end else
      } // end else
    } // end else if

    else if ( currentToken.type == INT || currentToken.type == FLOAT ) {
      parsedResult = currentToken ;
      Match();
      return true ;
    } // end else if

    else if ( currentToken.type == LPAREN ) {
      Match();

      if ( ArithExp( parsedResult ) ) {
        currentToken = GetCurrentToken();

        if ( currentToken.type == RPAREN ) {
          Match();
          return true ;
        } // end if

        else {
          if ( CurrentTokenType() == ERROR ) {
            parsedResult = GetCurrentToken();
            Match();
            return false;
          } // end if

          else {
            parsedResult.type = ERROR;
            parsedResult.error.type = SYNTACTICALERROR;
            parsedResult.error.errorValue = currentToken.tokenName;
            parsedResult.line = currentToken.line;
            Match();
            return false ;
          } // end else
        } // end else
      } // end if

      else {
        Match();
        return false ;
      } // end else 
    } // end else if 

    else {
      if ( CurrentTokenType() == ERROR ) {
        parsedResult = GetCurrentToken();
        Match();
        return false;
      } // end if

      else {
        parsedResult.type = ERROR;
        parsedResult.error.type = SYNTACTICALERROR;
        parsedResult.error.errorValue = currentToken.tokenName;
        parsedResult.line = currentToken.line;
        Match();
        return false ;
      } // end else
    } // end else
  } // end Factor()

  // �p�G��etoken�ŦX��k�A�h���Ჾ�@�� 
  void Match() {
    if ( mcurrentTokenIndex < mtokens.size() ) {
      mcurrentTokenIndex++;
    } // end if

    else return;
  } // end Match()

  // �����etoken������ 
  Type CurrentTokenType() {
    if ( mcurrentTokenIndex < mtokens.size() ) {
      return mtokens[mcurrentTokenIndex].type;
    } // end if

    else {
      return QUIT;
    } // end else
  } // end CurrentTokenType()

  Token GetCurrentToken() {
    Token quit;
    quit.type = QUIT;

    if ( mcurrentTokenIndex < mtokens.size() ) {
      return mtokens[mcurrentTokenIndex];
    } // end if

    else {
      return quit;
    } // end else

  } // end GetCurrentToken()
};

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
    string tokenName = GetNextToken() ;
    // �j��B�z�C��token�A�õ������A 
    while ( tokenName != "\0" ) {
      Token token;
      token.tokenName = tokenName;
      token.line = mlastmlineIndex ;
      
      // ���]����ƪ�^token�����A
      token.type = AnalyzeToken( tokenName );
      if ( token.type == ERROR ) {
      // �B�z�ҥ~�A�|�������㪺token �Ҧp:floatfloat 1.23.23

        if ( tokenName.substr( 0, 4 ) == "quit" ) {
          token.type = IDENT;
          token.tokenName = "quit";
        } // end if
        
        else if ( IsMultiFloat( tokenName ) ) {
          string left = "";
          string right = "";
          SplitString( tokenName, left, right ) ;
          token.tokenName = left;
          token.type = FLOAT;
          tokens.push_back( token );
          token.tokenName = right;
        } // end if

        else {
          // ���~�B�z 
          token.error = CheckError( tokenName );
        } // end else
      } // end if

      tokens.push_back( token );
      tokenName = GetNextToken() ;
    }  // end while

    return tokens;
  }  // end ProcessTokens()

  // �ˬd���w�r�Ŧꪺ���~�A��^�]�t���~�����M���~�Ȫ� Error ���c��C
  Error CheckError( string str ) {
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
      for ( size_t i = 0; i < str.length() ; i++ ) {
        char c = str[i];
        if ( c == '.' && !hasDot ) {
          hasDot = true;
        } // end if

        else if ( c == '+' || c == '-' ) {
          // �p�G�w�g���Ʀr�X�{�L�A�B���t�����b�Ĥ@��A�h��^ false
          if ( hasDigit || c != str[0] ) {
            error.errorValue = AnyToString( c );
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
          error.type = SYNTACTICALERROR;
        } // end else
      } // end for

      return error;
    } // end else
  } // end CheckError()

  // �ھڵ��w�� token �r��Ӥ��R�ê�^������ Type�C
  Type AnalyzeToken( string token ) {
  // - **���ѲŧP�_�G** �p�G token �O���ѲšA��^ `IDENT`�C
  // - **�Ʀr�����P�_�G** �p�G token �O�Ʀr�A�i�@�B�P�_�O��� (`INT`) �٬O�B�I�� (`FLOAT`)�C
  // - **�B��ũM���j�ŧP�_�G** �ھ� token ������Ȩ��ѧO�ê�^�����������A�p `PLUS`�B`MINUS`�B`MULTIPLY` ���C
  // - **���~�B�z�G** �p�G token ���ǰt����w�������A��^ `ERROR`�C

    if ( IsIDENT( token ) ) return IDENT;
        
    else if ( IsNUM( token ) ) {
      if ( IsInt( token ) ) return INT;
      else return FLOAT;
    } // end else if
    
    else if ( IsSIGN( token ) ) return SIGN;
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
  } // end AnalyzeToken()

  // �N�r�Ŧ�ھڲĤG���I('.')���Φ��ⳡ���C
  void SplitString( string str, string& left, string& right ) {
  // - **���μзǡG** �ھڲĤG�ӥX�{���I ('.') �i����ΡC
  // - **�����r�Ŧ�G** �ĤG���I���e���Ҧ��r�š]�]�t�Ĥ@���I�^�C
  // - **�k���r�Ŧ�G** �ĤG���I���᪺�r�šA����J��ĤT���I�ΫD�Ʀr�r�šC
  // - **�߱����G** �p�G�b�ĤG���I����X�{�D�Ʀr�r�ũβĤT���I�A�h�߱�o���᪺�Ҧ��r�šC
    bool firstDot = false;
    bool secondDot = false;
    bool thirdThrow = false;

    for ( int i = 0 ; i < str.size() ; i++ ) {
      if ( str[i] == '.' && firstDot == false ) firstDot = true;
      else if ( secondDot == true && ( !isdigit( str[i] ) ) ) thirdThrow = true;
      else if ( firstDot == true && str[i] == '.' ) secondDot = true;

      if ( thirdThrow == false ) {
        if ( secondDot == true ) {
          right = right + str[i];
        } // end if

        else left = left + str[i];
      } // end if
    } // end for
  } // end SplitString()
    
  // �ˬd���w���r�Ŧ�O�_�����Ī��ƾǲŸ��G�[�� ("+") �δ ("-")�C 
  bool IsSIGN( string str ) {
    if ( str.compare( "+" ) == 0 || str.compare( "-" ) == 0 )
      return true ;
    else {
      return false ;
    } // end else
  } // end IsSIGN()

  // �ˬd���w���r�Ŧ�O�_�O�@�Ӧ��Ī���ơC 
  bool IsInt( string str ) {
    if ( str.empty() ) return false;

    for ( size_t i = 0 ; i < str.length() ; i++ ) {
      char c = str[i];
      if ( !isdigit( c ) ) {
        return false;
      } // end if
    } // end for

    return true;
  } // end IsInt()

  // �ˬd���w���r�Ŧ�O�_�O�@�Ӧ��Ī��Ʀr�]��ƩίB�I�ơ^�C
  bool IsNUM( string str ) {
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
  } // end IsNUM() 
 
  // �ˬd���w���r�Ŧ�O�_��ܦh�ӯB�I�ơC 
  bool IsMultiFloat( string str ) {
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
  } // end IsMultiFloat()

  // �ˬd���w���r�Ŧ�O�_�O�@�ӦX�k�����ѲšC 
  bool IsIDENT( string str ) {
  // - **�D���ˬd�G** �Ŧr�ꤣ�����X�k�����ѲšC
  // - **���r���ˬd�G** ���ѲŪ��Ĥ@�Ӧr�ť����O�r���C
  // - **����r���ˬd�G** ���ѲŪ�����r�ťi�H�O�r���B�Ʀr�Ω��u�C 
  
    if ( str.empty() ) // �Ŧr�ꤣ�O�X�k�����Ѳ�
      return false;

    if ( !isalpha( str[0] ) )  // �Ĥ@�Ӧr�������O�r��
      return false;

    for ( size_t i = 1; i < str.length() ; i++ ) {
      if ( !isalnum( str[i] ) && str[i] != '_' )  // ��l�r�������O�Ʀr�B�r���Ω��u
        return false;
    } // end for

    return true;
  } // end IsIDENT()

  // �ˬd���w���r�ŬO�_�����j�šC 
  bool IsDelimiter( char ch ) {
    if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
         ch == '=' || ch == '<' || ch == '>' || ch == ';' )
      return true ;
            
    else if ( ch == ':' ) {
      ch = GetNextChar() ;
      if ( ch == '=' ) {
        LastChar() ;
        return true ;
      } // end if

      else {
        LastChar() ;
        return false ;
      } // end else
    } // end else if
    
    else return false ;
  } // end IsDelimiter()

  // �ھڵ��w���r���ѧO�ê�^���������j�ũξާ@�šC 
  string GetDelimiter( char ch ) {
  // - **��r�ųB�z�G** ������^��r�Ť��j�ũξާ@�šC
  // - **���r�ųB�z�G** ��� `<`�B`>` �� `:` �o�˪��r�šA��Ʒ|�ˬd�U�@�Ӧr�ťH�T�w�O�_�Φ����r�žާ@�šC
  // - **�^�h����G** �p�G�T�w�r�Ť��Φ����r�žާ@�šA��Ʒ|�NŪ�����w�^�h�@��C 
  
    if ( ch == '\0' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '*' || ch == '/' ||
         ch == '='  || ch == ';' ) {
      return string( 1, ch );
    } // end if
        
    else if ( ch == '<' ) {
      ch = GetNextChar() ;
      if ( ch == '>' ) return "<>" ;
      else if ( ch == '=' ) return "<=" ;
      else {
        LastChar() ;
        return "<" ;
      } // end else
    } // end else if
        
    else if ( ch == '>' ) {
      ch = GetNextChar() ;
      if ( ch == '=' ) return ">=" ;
      else {
        LastChar() ;
        return ">" ;
      } // end else
    } // end else if
        
    else if ( ch == ':' ) {
      ch = GetNextChar() ;
      if ( ch == '=' ) return ":=" ;
      else return ":";
    } // end else if
        
    else return string( 1, ch );
  } // end GetDelimiter()
  // ---------------------------------------GetToken--------------------------------------- 
  // ����U�@�� token�A�����ťաA�B�z�����A���ѧO���j�ũM���q�r�šC  
  string GetNextToken() {
  // - **���L�ťաG** �۰ʸ��L�D���Ħr�šA�p�Ů�M����šC
  // - **�B�z�����G** �˴�������Ÿ��ɡA������᪺�r�Ū��ܦ���C
  // - **�ѧO���j�šG** ��J����j�ŮɡA��^���j�Ū� token�C
  // - **�ѧO���q�r�ŧǦC�G** ���D���j�Ū��r�ŧǦC�A�N��@���@�ӧ��㪺 token ��^�C
  
    char nextChar = GetNextNonWhiteSpaceChar();
    if ( mlineIndex == minput.size() ) {
      return "\0";
    } // end if 
        
    else if ( nextChar == '/' ) {
      nextChar = GetNextChar();
            
      if ( nextChar == '/' ) {
        while ( nextChar != '\n' ) nextChar = GetNextChar() ;
        return GetNextToken();
      } // end if
            
      else {
        LastChar() ;
        return "/" ;
      } // end else
    } // end else if 
        
    else if ( IsDelimiter( nextChar ) ) {
      return GetDelimiter( nextChar ) ;
    } // end else if
        
    else {
      string tokenValue = ReadRemainingToken();
      return nextChar + tokenValue;
    } // end else 
  } // end GetNextToken()

  // Ū����e��m���᪺�s��r�šA����J��Ů�Τ��j�šA�Φ��@��token�C
  string ReadRemainingToken() {
  // **�s��Ū���G** �s��Ū���r�Ū���J��ťթΤ��j�šC
  // **�����B�z�G** �J���ӳs�򪺱׽u (`//`) �|�N����������A�������촫��Ū��Ҧ��r�šC
  // **Token �ֿn�G** �ֿnŪ�����r�šA�Φ� token �r��C
  
    string tokenValue = "";
    char nextChar = GetNextChar();
    bool hasDot = false;
    
    while ( !isspace( nextChar ) && !IsDelimiter( nextChar ) ) {
    
      if ( nextChar == '/' ) {       // �B�z�s�򪺵��� 
        nextChar = GetNextChar() ;
        
        if ( nextChar == '/' ) 
          while ( nextChar != '\n' ) nextChar = GetNextChar() ;
        else LastChar() ;
      } // end if

      else {
        tokenValue += nextChar;
        nextChar = GetNextChar();
      } // end else
      
    } // end while
        
    if ( IsDelimiter( nextChar ) ) LastChar() ; // delimiter���⦹token�A���e�@��column
    return tokenValue;
  } // ReadRemainingToken() 

  // ����U�@�ӫD�ťզr�šC
  char GetNextNonWhiteSpaceChar() {
  // ����D�ťզr�šG ����Ƴs��Ū���r�šA������Ĥ@�ӫD�ťզr�Ŭ���C
  // �۰ʩ����ťաG ��Ʒ|�۰ʩ����Ҧ��ťզr�šA�]�A�Ů�B����šB�s��ŵ��C
  // ��^�ȡG ��^��쪺�Ĥ@�ӫD�ťզr�šC�p�G�F���J�������ӨS�����D�ťզr�šA�h��^�Ŧr��('\0')�C
  
    char nextChar = GetNextChar();
    
    while ( isspace( nextChar ) ) 
      nextChar = GetNextChar();

    return nextChar; 
  } // end GetNextNonWhiteSpaceChar()

  void LastChar() {
    if ( mcolumnIndex == 0 && mlineIndex >= 1 ) {
      mlineIndex-- ;
      mcolumnIndex = minput[mlineIndex].size() ;
    } // end if
    
    else mcolumnIndex-- ;
  } // end LastChar()

  // �w�q�@�Ө�ƨ�����U�@�Ӧr�űq��J�奻���C
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

int main() {
  vector<string> minput = Read_lines();
  Tokenizer tokenizer( minput );
  vector<Token> tokenGroup = tokenizer.ProcessTokens() ;

  cout << "Program starts..." << endl ;
  Parser parse( tokenGroup ) ;
  parse.Parse() ;
  cout << "> Program exits..." << endl ;
  
} // end main()
