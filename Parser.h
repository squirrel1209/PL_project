// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "Tokenizer.h"

map<string, Type> gsymbolTable;
vector<string> gIdToeknName;

class Parser {
public:
  explicit Parser( Tokenizer tokenizer ) : tokenizer( tokenizer ), nextToken( tokenizer.GetNextToken() ) {}

  void parse() {
    user_input();
  } // end parse() 

private:
    Tokenizer tokenizer;
    Token nextToken;

  void user_input() {
    Token parsedResult ;
    
    while ( nextToken.type != QUIT ) {
      if ( nextToken.type == VOID || type_specifier() ) {
        definition( parsedResult );
      } // end if
      /*
      else if ( startExpression() ) {
        statement( parsedResult );
      } // end else if
      */
      
      else {
        if ( nextToken.type != ERROR )
          parsedResult = tokenizer.CreatToken( nextToken.tokenName, nextToken.line, ERROR, SYNTACTICALERROR );
        
        else parsedResult = nextToken;
      } // end else
      
      nextToken = tokenizer.GetNextToken();
    } // end while
  } // end user_input()
  
  void definition( Token &parsedResult ) {
    Type type = nextToken.type;
    string tokenName;
    
    if ( nextToken.type == VOID ) {
      Match( VOID, parsedResult );
      tokenName = nextToken.tokenName;
      
      if ( Match( IDENTIFIER, parsedResult ) ) 
        function_definition_without_ID();
    } // end if
    
    else {
      Match( nextToken.type, parsedResult );
      tokenName = nextToken.tokenName;
      
      if ( Match( IDENTIFIER, parsedResult ) )
        if ( function_definition_or_declarators( parsedResult ) ) 
          gsymbolTable[tokenName] = type;
    } // end else
  } // end definition()
  
  bool type_specifier() {
    return nextToken.type == INT || nextToken.type == FLOAT ||
    nextToken.type == CHAR || nextToken.type == BOOL || nextToken.type == STRING ;
  } // end type_specifier()
 
  bool function_definition_or_declarators( Token &parsedResult ) {
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( nextToken.type == LPAREN ) {
      return function_definition_without_ID();
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
        if ( nextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult );   // 匹配 '['
          
          if ( Match( CONSTANT, parsedResult) ) 
            return Match( RBRACKET, parsedResult );
          else return false ;
        } // end if
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

  void function_definition_without_ID() {
    Match(LPAREN); 

   
    if (nextToken.type == VOID) {
      Match(VOID); // 匹配 VOID
    } else if (type_specifier()) {
      formal_parameter_list(); // 解析 formal_parameter_list
    }

    Match(RPAREN); // 匹配右括? ')'
    compound_statement(); // 解析 compound_statement
  }
 
  void formal_parameter_list() {
    // ?理第一???
    type_specifier();
    if (nextToken.type == AMPERSAND) {
      Match(AMPERSAND); // 匹配 '&'
    }
    if (nextToken.type == IDENTIFIER) {
      Match(IDENTIFIER); // 匹配??符
    } else {
      // ???理
      std::cerr << "Syntax error: Expected IDENTIFIER but found " << nextToken.tokenName << std::endl;
      exit(EXIT_FAILURE);
    }
    if (nextToken.type == LBRACKET) {
      Match(LBRACKET); // 匹配 '['
      if (nextToken.type == CONSTANT) {
        Match(CONSTANT); // 匹配常量
        Match(RBRACKET); // 匹配 ']'
      } else {
        // ???理
        std::cerr << "Syntax error: Expected CONSTANT but found " << nextToken.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    // ?理后???
    while (nextToken.type == COMMA) {
      Match(COMMA); // 匹配 ','
      type_specifier();
      if (nextToken.type == AMPERSAND) {
        Match(AMPERSAND); // 匹配 '&'
      }
      if (nextToken.type == IDENTIFIER) {
        Match(IDENTIFIER); // 匹配??符
      } else {
        // ???理
        std::cerr << "Syntax error: Expected IDENTIFIER but found " << nextToken.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
      if (nextToken.type == LBRACKET) {
        Match(LBRACKET); // 匹配 '['
        if (nextToken.type == CONSTANT) {
          Match(CONSTANT); // 匹配常量
          Match(RBRACKET); // 匹配 ']'
        } else {
          // ???理
          std::cerr << "Syntax error: Expected CONSTANT but found " << nextToken.tokenName << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  void compound_statement() {
    Match(LBRACE); // 匹配 '{'

    while (nextToken.type != RBRACE) {
      if (type_specifier()) {
        declaration(); // 解析?明
      } else if (isStatementStart(nextToken.type)) {
        statement(); // 解析?句
      } else {
        // ???理：既不是?明也不是?句的?始
        std::cerr << "Syntax error: Unexpected token " << nextToken.tokenName << " in compound statement" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    Match(RBRACE); // 匹配 '}'
  }

  bool isStatementStart(Type type) {
    // 根据??的文法??判?是否??句的?始
    // ?需要根据你的文法中statement的定????
    // 例如，?里??地假?某些特定的token?型?志??句的?始
    return type == IF || type == WHILE || type == RETURN || type == IDENTIFIER; // 根据需要添加更多
  }

  void declaration() {
    // 解析?型?明符
    if (type_specifier()) {
      Match(IDENTIFIER); // 匹配??符
      rest_of_declarators(); // 解析后??明
    } else {
      // ???理：?期是?型?明符
      std::cerr << "Syntax error: Expected type specifier but found " << nextToken.tokenName << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  bool startStatement() {
    
  } // end startStatement()
  void statement();
  /*
  bool startExpression() {
    nextToken.type == SEMICOLON || expression() || nextToken.type == RETURN || 
                  nextToken.type == LBRACE || nextToken.type == IF || nextToken.type == WHILE ||
	        nextToken.type == DO
  } // end isExpression()
  */
  void expression();
  void basic_expression();
    
  bool Match( Type expected, Token &parsedResult ) {
    if ( lookahead.type == expected ) {
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
      return false;
    } // end else
  } // end Match()
 
};

#endif // PARSER_H

