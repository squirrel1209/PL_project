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
    // �ˬd�U�@�ӲŸ��O�_�O���A���A�p�G�O�A�h�B�z����Ʃw�q
    if ( nextToken.type == LPAREN ) {
      return function_definition_without_ID();
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
        if ( nextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult );   // �ǰt '['
          
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

    return Match( SEMICOLON, parsedResult );   // ��������
  } // end rest_of_declarators()

  void function_definition_without_ID() {
    Match(LPAREN); 

   
    if (nextToken.type == VOID) {
      Match(VOID); // �ǰt VOID
    } else if (type_specifier()) {
      formal_parameter_list(); // �ѪR formal_parameter_list
    }

    Match(RPAREN); // �ǰt�k�A? ')'
    compound_statement(); // �ѪR compound_statement
  }
 
  void formal_parameter_list() {
    // ?�z�Ĥ@???
    type_specifier();
    if (nextToken.type == AMPERSAND) {
      Match(AMPERSAND); // �ǰt '&'
    }
    if (nextToken.type == IDENTIFIER) {
      Match(IDENTIFIER); // �ǰt??��
    } else {
      // ???�z
      std::cerr << "Syntax error: Expected IDENTIFIER but found " << nextToken.tokenName << std::endl;
      exit(EXIT_FAILURE);
    }
    if (nextToken.type == LBRACKET) {
      Match(LBRACKET); // �ǰt '['
      if (nextToken.type == CONSTANT) {
        Match(CONSTANT); // �ǰt�`�q
        Match(RBRACKET); // �ǰt ']'
      } else {
        // ???�z
        std::cerr << "Syntax error: Expected CONSTANT but found " << nextToken.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    // ?�z�Z???
    while (nextToken.type == COMMA) {
      Match(COMMA); // �ǰt ','
      type_specifier();
      if (nextToken.type == AMPERSAND) {
        Match(AMPERSAND); // �ǰt '&'
      }
      if (nextToken.type == IDENTIFIER) {
        Match(IDENTIFIER); // �ǰt??��
      } else {
        // ???�z
        std::cerr << "Syntax error: Expected IDENTIFIER but found " << nextToken.tokenName << std::endl;
        exit(EXIT_FAILURE);
      }
      if (nextToken.type == LBRACKET) {
        Match(LBRACKET); // �ǰt '['
        if (nextToken.type == CONSTANT) {
          Match(CONSTANT); // �ǰt�`�q
          Match(RBRACKET); // �ǰt ']'
        } else {
          // ???�z
          std::cerr << "Syntax error: Expected CONSTANT but found " << nextToken.tokenName << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  void compound_statement() {
    Match(LBRACE); // �ǰt '{'

    while (nextToken.type != RBRACE) {
      if (type_specifier()) {
        declaration(); // �ѪR?��
      } else if (isStatementStart(nextToken.type)) {
        statement(); // �ѪR?�y
      } else {
        // ???�z�G�J���O?���]���O?�y��?�l
        std::cerr << "Syntax error: Unexpected token " << nextToken.tokenName << " in compound statement" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    Match(RBRACE); // �ǰt '}'
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
      Match(IDENTIFIER); // �ǰt??��
      rest_of_declarators(); // �ѪR�Z??��
    } else {
      // ???�z�G?���O?��?����
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

