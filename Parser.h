
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

  // 分析並處理輸入的 token。  
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
    float fractionalPart = result - floor( result ); // 獲取小數部分
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
      if ( IsNearlyDivisible( floatA, floatB, 0.0001 ) ) {  // 整除 
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
    result.type = BOOL;  // 假設 Token 有一個 BOOL 類型用於表示布林值

    // 將字面值轉換為數值進行比較
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

  // 解析當前命令並更新 parsedResult 參數以反映命令的結果。
  void Command( Token &parsedResult ) {
  // - **QUIT 和 ERROR 處理：** 如果當前 token 為 QUIT 或 ERROR，則直接更新 `parsedResult`。
  // - **標識符和賦值：** 處理標識符和其後的賦
    Token currentToken = GetCurrentToken();
    if ( currentToken.type == QUIT || currentToken.type == ERROR ) {
      parsedResult = currentToken;
      Match();
    } // end if

    else if ( currentToken.type == IDENT ) {
      Token ident = currentToken;
      Match();  // 移動到下一個 token，即 ASSIGN

      if ( ident.tokenName.compare( "quit" ) == 0 ) {
        parsedResult = ident;
        parsedResult.type = QUIT;
        return;
      } // end if

      if ( gsymbolTable.find( ident.tokenName ) != gsymbolTable.end() ) {  // 有define
        Token deFineident = gsymbolTable[ident.tokenName];
        
        if ( CurrentTokenType() == SEMICOLON ) {    // IDENT ;
          parsedResult = deFineident;
          Match();
          return;
        } // end if
        
        else if ( CurrentTokenType() == ASSIGN ) {  // IDENT ':=' <ArithExp>
          Match();  // 移動到賦值表達式 

          if ( ArithExp( parsedResult ) ) {
            if ( CurrentTokenType() == SEMICOLON ) {
              // 將算術表達式的結果賦值給標識符
              gsymbolTable[ident.tokenName] = parsedResult ;
              Match();  // 消耗掉分號
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
      
      else {          // 沒有define
        if ( CurrentTokenType() == ASSIGN ) {  // IDENT ':=' <ArithExp>
          Match();  // 移動到賦值表達式 

          if ( ArithExp( parsedResult ) ) {
            if ( CurrentTokenType() == SEMICOLON ) {
              // 將算術表達式的結果賦值給標識符
              gsymbolTable[ident.tokenName] = parsedResult ;
              Match();  // 消耗掉分號
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
        
        else {    // 文法錯誤 
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

  // 檢查並解析非標識符開頭的算術或布爾表達式。
  bool NOT_IDStartArithExpOrBexp( Token &parsedResult ) {
  // - **算術表達式解析：** 首先解析算術表達式，如果表達式不以標識符開頭，則進行這一步。
  // - **布爾運算解析：** 在算術表達式後，檢查並解析可能的布爾運算表達式。
  // - **比較運算處理：** 如果有布爾運算符，則進行相應的比較運算，並更新 `parsedResult`。

    if ( NOT_ID_StartArithExp( parsedResult ) ) {
      // 解析成功後，檢查是否跟隨了布爾運算符

      if ( BooleanOperator() ) {
        Token operatorToken = GetCurrentToken();
        Match(); // 消耗布爾運算符
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
  // - **初步解析：** 首先解析算術表達式的第一個 Term，如果解析失敗則函數返回 false。
  // - **運算處理：** 若遇到加號或減號，將進行相應的算術運算。
  // - **迭代解析：** 循環處理後續的 Term，直到表達式結束。
  // - **結果更新：** 根據運算更新 parsedResult 的值。

    // 解析第一個 Term
    if ( !NOT_ID_StartTerm( parsedResult ) ) {
      return false;
    } // end if

    // 當當前 token 是加號或減號時，進行運算
    while ( CurrentTokenType() == SIGN ) {
      // 保存運算符
      Type op;
      if ( GetCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
      else op = PLUS ;
      Match();  // 移動到下一個 token

      // 解析下一個 Term
      Token b;

      if ( !Term( b ) ) {
        parsedResult = b; 
        return false;
      } // end if

      // 根據運算符進行相應的運算，並更新 a 為運算結果
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while

    // 返回 true 表示成功解析 ArithExp
    return true;
  } // end NOT_ID_StartArithExp()

  bool NOT_ID_StartTerm( Token &parsedResult ) {
  // - **初步解析：** 解析開頭的 Factor，如果這一步失敗，整個 Term 的解析也將失敗。
  // - **乘除運算處理：** 繼續解析遇到的乘法或除法運算符，並對相應的操作數進行運算。
  // - **迭代解析：** 循環處理 Term 中的所有乘法和除法運算。
  // - **結果更新：** 根據運算更新 `parsedResult` 的值。

    // 首先解析一個 Factor
    if ( !NOT_ID_StartFactor( parsedResult ) ) {
      return false;
    } // end if

    // 當當前 token 是乘法或除法運算符時，進行運算
    while ( CurrentTokenType() == MULTIPLY || CurrentTokenType() == DIVIDE ) {
      // 保存運算符
      Type op = CurrentTokenType();
      Match();  // 移動到下一個 token

      // 解析下一個 Factor
      Token b;
      if ( !Factor( b ) ) {
        parsedResult = b ;
        return false;
      } // end if

      // 根據運算符進行相應的運算，並更新 a 為運算結果
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while

    // 返回 true 表示成功解析 Term
    return true;
  } // end NOT_ID_StartTerm()

  // 解析不以標識符開頭的因子(Factor)。 
  bool NOT_ID_StartFactor( Token &parsedResult ) {
  // - **直接數值：** 能夠解析直接給出的整數或浮點數。
  // - **符號處理：** 能夠識別並處理數值前的正負符號。
  // - **括號表達式：** 能夠處理括號內的算術表達式。
  // - **錯誤處理：** 當遇到語法錯誤時，會設置相應的錯誤信息並返回 false。    
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
      currentToken = GetCurrentToken(); // 更新 Token 類型

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

      Match();  // 移動到下一個 token，即 Term 或 Factor 的開始

      if ( op == PLUS || op == MINUS ) {
        if ( !Term( parsedResult ) ) {
          return false;
        } // end if

        parsedResult = EvaluateOperation( ident, parsedResult, op ); // 執行運算
      } // end if

      else if ( op == MULTIPLY || op == DIVIDE ) {
        if ( !Factor( parsedResult ) ) {
          return false;
        } // end if
        
        parsedResult = EvaluateOperation( ident, parsedResult, op ); // 執行運算
      } // end else if

      ident = parsedResult ;
    } // end while

    if ( BooleanOperator() ) {
      Type op = CurrentTokenType();
      Match();  // 移動到 Boolean 表達式的開始

      Token nextOperand;
      if ( !ArithExp( nextOperand ) ) {
        parsedResult = nextOperand;
        return false;
      } // end if

      parsedResult = CompareOperation( parsedResult, nextOperand, op ); // 執行布林運算
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
        Type op = CurrentTokenType();  // 獲取比較運算符
        Match();

        Token rightExpr;
        if ( ArithExp( rightExpr ) ) {
          // 比較兩個算術表達式的結果
          // 此處應該有比較邏輯的實現，這裡僅示範結構
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

    // 解析第一個 Term
    if ( !Term( parsedResult ) ) {
      return false;
    } // end if

    // 當當前 token 是加號或減號時，進行運算
    while ( CurrentTokenType() == SIGN ) {
      // 保存運算符
      Type op;
      if ( GetCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
      else op = PLUS ;
      Match();  // 移動到下一個 token

      // 解析下一個 Term
      Token b;

      if ( !Term( b ) ) {
        parsedResult = b;
        return false;
      } // end if

      // 根據運算符進行相應的運算，並更新 a 為運算結果
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while 

    // 返回 true 表示成功解析 ArithExp
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

    // 首先解析一個 Factor
    if ( !Factor( parsedResult ) ) {
      return false;
    } // end if

    // 當當前 token 是乘法或除法運算符時，進行運算
    while ( CurrentTokenType() == MULTIPLY || CurrentTokenType() == DIVIDE ) {
      // 保存運算符
      Type op = CurrentTokenType();
      Match();  // 移動到下一個 token

      // 解析下一個 Factor
      Token b;
      if ( !Factor( b ) ) {
        parsedResult = b ;
        return false;
      } // end if

      // 根據運算符進行相應的運算，並更新 a 為運算結果
      parsedResult = EvaluateOperation( parsedResult, b, op );
    } // end while

    // 返回 true 表示成功解析 Term
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
      currentToken = GetCurrentToken(); // 更新 Token 類型

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

  // 如果當前token符合文法，則往後移一位 
  void Match() {
    if ( mcurrentTokenIndex < mtokens.size() ) {
      mcurrentTokenIndex++;
    } // end if

    else return;
  } // end Match()

  // 獲取當前token的類型 
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

