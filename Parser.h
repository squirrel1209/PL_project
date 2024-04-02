// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

class Parser {
public:
  explicit Parser( Lexer lexer ) : lexer( lexer ), lookahead( lexer.getNextToken() ) {}

  void parse() {
    user_input();
  } // end parse() 

private:
    Lexer lexer;
    Token lookahead;

  void user_input() {
    while ( lookahead.type != QUIT ) {
      if ( lookahead.type == LPAREN ) {
        if ( lookahead.type == VOID || type_specifier() ) {
        	bool isDefinition = true ;
          while ( isDefinition ) {
            isDefinition = definition();
	} // end while
        } // end if
        
        else if ( startExpression() ) {
          statement();
        } // end else if
        
        else {  // error
        } // end else
      } // end if 
    } // end while
  } // end user_input()
  
  
  bool definition() {
    if ( lookahead.type == VOID ) {
      match( VOID );
      if ( lookahead.type == IDENTIFIER ) {
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
    if ( lookahead.type == INT || lookahead.type == FLOAT ||
      lookahead.type == CHAR || lookahead.type == BOOL || lookahead.type == STRING ) {
      return true;
    } // end if
    
    else {
      return false;
    } // end else
  } // end type_specifier()
 
  void function_definition_without_ID();
  void compound_statement();
  bool startStatement() {
    
  } // end startStatement()
  void statement();
  bool startExpression() {
    lookahead.type == SEMICOLON || expression() || lookahead.type == RETURN || 
                  lookahead.type == LBRACE || lookahead.type == IF || lookahead.type == WHILE ||
	        lookahead.type == DO
  } // end isExpression()
    void expression();
    void basic_expression();
  void match( TokenType expected ) {
    if ( lookahead.type == expected ) {
      lookahead = lexer.getNextToken();
    } // end if 
    else {
      cerr << "Syntax error: Unexpected token " << lookahead.value << endl; 
      exit( EXIT_FAILURE );
    } // end else
  }
    // 假設其他解析函數已經定義...
};

#endif // PARSER_H

