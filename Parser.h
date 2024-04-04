// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "Tokenizer.h"

class Parser {
public:
  explicit Parser( Tokenizer tokenizer ) : tokenizer( tokenizer ), lookahead( tokenizer.GetNextToken() ) {}

  void parse() {
    user_input();
  } // end parse() 

private:
    Tokenizer tokenizer;
    Token lookahead;

  void user_input() {
    while ( lookahead.type != QUIT ) {
      if ( lookahead.type == LPAREN ) {
        if ( lookahead.type == VOID || type_specifier() ) {
          definition();
        } // end if
        /*
        else if ( startExpression() ) {
          statement();
        } // end else if
        */
        else {  // error
        } // end else
      } // end if 
    } // end while
  } // end user_input()
  
  
  bool definition() {
    if ( lookahead.type == VOID ) {
      match( VOID );
      if ( lookahead.type == IDENTIFIER ) {
        match( IDENTIFIER ); 
        function_definition_without_ID();
      } // end if
    } // end if
    
    else {
      match( lookahead.type );
      if ( lookahead.type == IDENTIFIER ) {
        function_definition_or_declarators();
      } // end if
    } // end else
  } // end definition()
  
  bool type_specifier() {
    return lookahead.type == INT || lookahead.type == FLOAT ||
    lookahead.type == CHAR || lookahead.type == BOOL || lookahead.type == STRING ;
  } // end type_specifier()
 
  void function_definition_or_declarators() {
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( lookahead.type == LPAREN ) {
      function_definition_without_ID();
    } // end if
    else {
    // 如果下一個符號不是左括號，則處理為變量宣告
      rest_of_declarators();
    } // end else
  } 

  void rest_of_declarators() {
    // 檢查是否存在 '[' Constant ']' 結構
    if ( lookahead.type == LBRACKET ) {
      match(LBRACKET);   // 匹配 '['
      if ( lookahead.type == CONSTANT ) {
        match(CONSTANT); // 匹配常量
      } else {
        // 錯誤處理：預期是常量
        std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
      match(RBRACKET);   // 匹配 ']'
    }

    // 處理 { ',' Identifier [ '[' Constant ']' ] } 結構
    while (lookahead.type == COMMA) {
      match(COMMA);     // 匹配 ','
      if (lookahead.type == IDENTIFIER) {
        match(IDENTIFIER); // 匹配標識符
        // 檢查是否存在 '[' Constant ']' 結構
        if (lookahead.type == LBRACKET) {
          match(LBRACKET);   // 匹配 '['
          if (lookahead.type == CONSTANT) {
            match(CONSTANT); // 匹配常量
          } else {
            // 錯誤處理：預期是常量
            std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
            exit(EXIT_FAILURE);
          }
          match(RBRACKET);   // 匹配 ']'
        }
      } else {
        // 錯誤處理：預期是標識符
        std::cerr << "Syntax error: Expected IDENTIFIER but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    match(SEMICOLON);   // 結束分號
  }

  void function_definition_without_ID() {
    match(LPAREN); // 匹配左括? '('

    // ?查是否有 VOID 或 type_specifier followed by formal_parameter_list
    if (lookahead.type == VOID) {
      match(VOID); // 匹配 VOID
    } else if (type_specifier()) {
      formal_parameter_list(); // 解析 formal_parameter_list
    }

    match(RPAREN); // 匹配右括? ')'
    compound_statement(); // 解析 compound_statement
  }
 
  void formal_parameter_list() {
    // ?理第一???
    type_specifier();
    if (lookahead.type == AMPERSAND) {
      match(AMPERSAND); // 匹配 '&'
    }
    if (lookahead.type == IDENTIFIER) {
      match(IDENTIFIER); // 匹配??符
    } else {
      // ???理
      std::cerr << "Syntax error: Expected IDENTIFIER but found " << lookahead.tokenName << std::endl;
      exit(EXIT_FAILURE);
    }
    if (lookahead.type == LBRACKET) {
      match(LBRACKET); // 匹配 '['
      if (lookahead.type == CONSTANT) {
        match(CONSTANT); // 匹配常量
        match(RBRACKET); // 匹配 ']'
      } else {
        // ???理
        std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    // ?理后???
    while (lookahead.type == COMMA) {
      match(COMMA); // 匹配 ','
      type_specifier();
      if (lookahead.type == AMPERSAND) {
        match(AMPERSAND); // 匹配 '&'
      }
      if (lookahead.type == IDENTIFIER) {
        match(IDENTIFIER); // 匹配??符
      } else {
        // ???理
        std::cerr << "Syntax error: Expected IDENTIFIER but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
      if (lookahead.type == LBRACKET) {
        match(LBRACKET); // 匹配 '['
        if (lookahead.type == CONSTANT) {
          match(CONSTANT); // 匹配常量
          match(RBRACKET); // 匹配 ']'
        } else {
          // ???理
          std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  void compound_statement() {
    match(LBRACE); // 匹配 '{'

    while (lookahead.type != RBRACE) {
      if (type_specifier()) {
        declaration(); // 解析?明
      } else if (isStatementStart(lookahead.type)) {
        statement(); // 解析?句
      } else {
        // ???理：既不是?明也不是?句的?始
        std::cerr << "Syntax error: Unexpected token " << lookahead.tokenName << " in compound statement" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    match(RBRACE); // 匹配 '}'
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
      match(IDENTIFIER); // 匹配??符
      rest_of_declarators(); // 解析后??明
    } else {
      // ???理：?期是?型?明符
      std::cerr << "Syntax error: Expected type specifier but found " << lookahead.tokenName << std::endl;
      exit(EXIT_FAILURE);
    }
  }



  bool startStatement() {
    
  } // end startStatement()
  void statement();
  /*
  bool startExpression() {
    lookahead.type == SEMICOLON || expression() || lookahead.type == RETURN || 
                  lookahead.type == LBRACE || lookahead.type == IF || lookahead.type == WHILE ||
	        lookahead.type == DO
  } // end isExpression()
  */
    void expression();
    void basic_expression();
  void match( Type expected ) {
    if ( lookahead.type == expected ) {
      lookahead = tokenizer.GetNextToken();
    } // end if 
    else {
      cerr << "Syntax error: Unexpected token " << lookahead.tokenName << endl; 
      exit( EXIT_FAILURE );
    } // end else
  }
    // 假設其他解析函數已經定義...
};

#endif // PARSER_H

