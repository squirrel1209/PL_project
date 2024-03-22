
#ifndef ParseR_H
#define ParseR_H

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include "Tokenizer.h"
#include <cmath>

using namespace std;
map<string, Token> gsymbolTable;

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

#endif // ParseR_H

