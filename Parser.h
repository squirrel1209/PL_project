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
    // �ˬd�U�@�ӲŸ��O�_�O���A���A�p�G�O�A�h�B�z����Ʃw�q
    if ( lookahead.type == LPAREN ) {
      function_definition_without_ID();
    } // end if
    else {
    // �p�G�U�@�ӲŸ����O���A���A�h�B�z���ܶq�ŧi
      rest_of_declarators();
    } // end else
  } 

  void rest_of_declarators() {
    // �ˬd�O�_�s�b '[' Constant ']' ���c
    if ( lookahead.type == LBRACKET ) {
      match(LBRACKET);   // �ǰt '['
      if ( lookahead.type == CONSTANT ) {
        match(CONSTANT); // �ǰt�`�q
      } else {
        // ���~�B�z�G�w���O�`�q
        std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
      match(RBRACKET);   // �ǰt ']'
    }

    // �B�z { ',' Identifier [ '[' Constant ']' ] } ���c
    while (lookahead.type == COMMA) {
      match(COMMA);     // �ǰt ','
      if (lookahead.type == IDENTIFIER) {
        match(IDENTIFIER); // �ǰt���Ѳ�
        // �ˬd�O�_�s�b '[' Constant ']' ���c
        if (lookahead.type == LBRACKET) {
          match(LBRACKET);   // �ǰt '['
          if (lookahead.type == CONSTANT) {
            match(CONSTANT); // �ǰt�`�q
          } else {
            // ���~�B�z�G�w���O�`�q
            std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
            exit(EXIT_FAILURE);
          }
          match(RBRACKET);   // �ǰt ']'
        }
      } else {
        // ���~�B�z�G�w���O���Ѳ�
        std::cerr << "Syntax error: Expected IDENTIFIER but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    match(SEMICOLON);   // ��������
  }

  void function_definition_without_ID() {
    match(LPAREN); // �ǰt���A? '('

    // ?�d�O�_�� VOID �� type_specifier followed by formal_parameter_list
    if (lookahead.type == VOID) {
      match(VOID); // �ǰt VOID
    } else if (type_specifier()) {
      formal_parameter_list(); // �ѪR formal_parameter_list
    }

    match(RPAREN); // �ǰt�k�A? ')'
    compound_statement(); // �ѪR compound_statement
  }
 
  void formal_parameter_list() {
    // ?�z�Ĥ@???
    type_specifier();
    if (lookahead.type == AMPERSAND) {
      match(AMPERSAND); // �ǰt '&'
    }
    if (lookahead.type == IDENTIFIER) {
      match(IDENTIFIER); // �ǰt??��
    } else {
      // ???�z
      std::cerr << "Syntax error: Expected IDENTIFIER but found " << lookahead.tokenName << std::endl;
      exit(EXIT_FAILURE);
    }
    if (lookahead.type == LBRACKET) {
      match(LBRACKET); // �ǰt '['
      if (lookahead.type == CONSTANT) {
        match(CONSTANT); // �ǰt�`�q
        match(RBRACKET); // �ǰt ']'
      } else {
        // ???�z
        std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    // ?�z�Z???
    while (lookahead.type == COMMA) {
      match(COMMA); // �ǰt ','
      type_specifier();
      if (lookahead.type == AMPERSAND) {
        match(AMPERSAND); // �ǰt '&'
      }
      if (lookahead.type == IDENTIFIER) {
        match(IDENTIFIER); // �ǰt??��
      } else {
        // ???�z
        std::cerr << "Syntax error: Expected IDENTIFIER but found " << lookahead.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
      if (lookahead.type == LBRACKET) {
        match(LBRACKET); // �ǰt '['
        if (lookahead.type == CONSTANT) {
          match(CONSTANT); // �ǰt�`�q
          match(RBRACKET); // �ǰt ']'
        } else {
          // ???�z
          std::cerr << "Syntax error: Expected CONSTANT but found " << lookahead.tokenName << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  void compound_statement() {
    match(LBRACE); // �ǰt '{'

    while (lookahead.type != RBRACE) {
      if (type_specifier()) {
        declaration(); // �ѪR?��
      } else if (isStatementStart(lookahead.type)) {
        statement(); // �ѪR?�y
      } else {
        // ???�z�G�J���O?���]���O?�y��?�l
        std::cerr << "Syntax error: Unexpected token " << lookahead.tokenName << " in compound statement" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    match(RBRACE); // �ǰt '}'
  }

  bool isStatementStart(Type type) {
    // ���u??����k??�P?�O�_??�y��?�l
    // ?�ݭn���u�A����k��statement���w????
    // �Ҧp�A?��??�a��?�Y�ǯS�w��token?��?��??�y��?�l
    return type == IF || type == WHILE || type == RETURN || type == IDENTIFIER; // ���u�ݭn�K�[��h
  }

  void declaration() {
    // �ѪR?��?����
    if (type_specifier()) {
      match(IDENTIFIER); // �ǰt??��
      rest_of_declarators(); // �ѪR�Z??��
    } else {
      // ???�z�G?���O?��?����
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
    // ���]��L�ѪR��Ƥw�g�w�q...
};

#endif // PARSER_H

