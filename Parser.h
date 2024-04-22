// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include "Tokenizer.h"

map<string, Type> gsymbolTable;
vector<string> gIdToeknName;

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
    bool test = true;
    if ( test ) {
      while ( nextToken.type != QUIT ) {
        
      } // end while
      return;
    } // end if


    while ( nextToken.type != QUIT ) {
      if ( nextToken.type == VOID || type_specifier() ) {
        definition( parsedResult );
      } // end if
      
      else if ( statement( parsedResult ) ) {
        cout << "Statement executed ...\n"; 
      } // end else if

      else 
        parsedResult = nextToken;

      if ( parsedResult.type == ERROR ) {
        if ( parsedResult.error == LEXICALERROR )
          printf( "Line %d : unrecognized token with first char : '%s'\n", parsedResult.line, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SYNTACTICALERROR )
          printf( "Line %d : Unexpected token : '%s'\n", parsedResult.line, parsedResult.tokenName.c_str() );
	 
        else if ( parsedResult.error == SEMANTICERROR )
          printf( "Line %d : Undefined identifier : '%s'\n", parsedResult.line, parsedResult.tokenName.c_str() ); 
        else cout << "> Error" << endl ;
        
        while ( parsedResult.line >= nextToken.line ) nextToken = tokenizer.GetNextToken();
      } // end if
      
    } // end while
    
    gsymbolTable.clear();
  } // end user_input()
  
  bool definition( Token &parsedResult ) {
    Type type = nextToken.type;

    if ( nextToken.type == VOID ) {
      Match( VOID, parsedResult );
      gIdToeknName.push_back( nextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( function_definition_without_ID( parsedResult ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) 
        	    printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
        	  else printf( "New definition of %s entered...\n", gIdToeknName[i].c_str() );
        	  
        	  gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
        } // end if
      } // end if
    } // end if
    
    else {
      Match( nextToken.type, parsedResult );
      gIdToeknName.push_back( nextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( function_definition_or_declarators( parsedResult ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) 
        	    printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
        	  else printf( "New definition of %s entered...\n", gIdToeknName[i].c_str() );
        	  
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
 
  bool function_definition_or_declarators( Token &parsedResult ) {
    // �ˬd�U�@�ӲŸ��O�_�O���A���A�p�G�O�A�h�B�z����Ʃw�q
    if ( nextToken.type == LPAREN ) {
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

    if ( nextToken.type == VOID ) 
      Match( VOID, parsedResult ); // �ǰt VOID
      
    else if ( type_specifier() ) 
      if ( !formal_parameter_list( parsedResult ) ) return false;// �ѪR formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return compound_statement( parsedResult ); // �ѪR compound_statement
  } // end function_definition_without_ID()
 
  bool formal_parameter_list( Token &parsedResult ) {
    Match( nextToken.type, parsedResult ) ;
    
    if ( nextToken.type == AMPERSAND ) {
      Match( AMPERSAND, parsedResult ); // �ǰt '&'
    } // end if
    
    if ( nextToken.type == IDENTIFIER ) {
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
    if ( !Match( LBRACE, parsedResult ) ) return false; 

    if ( !Match( RBRACE, parsedResult ) ) return false;
    return true;
  } // end  compound_statement()

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
/*
    else if ( nextToken.type == RETURN ) {
        if (!Match(RETURN, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (nextToken.type != ';') {
            expression();
        }
        if (!Match(';', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
    } else if (nextToken.type == '{') {
        compound_statement();
    } else if (nextToken.type == IF) {
        if (!Match(IF, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match('(', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        statement();
        if (nextToken.type == ELSE) {
            if (!Match(ELSE, parsedResult)) {
                return; // �Ϊ̶i���L���~�B�z
            }
            statement();
        }
    } else if (nextToken.type == WHILE) {
        if (!Match(WHILE, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match('(', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        statement();
    } else if (nextToken.type == DO) {
        if (!Match(DO, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        statement();
        if (!Match(WHILE, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match('(', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match(';', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
    } else {
        // �B�z���w���� token
        std::cerr << "Unexpected token in statement" << std::endl;
        return; // �פ�B�z
    }
*/
  } // end statement()

  bool StartExpression() {
    if ( nextToken.type == IDENTIFIER || nextToken.type == CONSTANT ||
         nextToken.type == PP || nextToken.type == MM 
         /*nextToken.type == SIGN*/ )
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
  } // end if

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
    
    if ( nextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult );
      if ( !actual_parameter_list( parsedResult ) ) return false;
        
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return romce_and_romloe( parsedResult );
    } // end if
    
    return false;
  } // end rest_of_Identifier_started_basic_exp()

  bool assignment_operator( Token &parsedResult ) {
    if ( nextToken.type == ASSIGN || nextToken.type == TE || nextToken.type == DE || 
        nextToken.type == RE || nextToken.type == PE || nextToken.type == ME) {
      return Match( nextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end assignment_operator()

  bool romce_and_romloe( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_OR_exp( parsedResult ) ) return false;  // ?�z??�Ϊ�?��

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

  bool rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z��Ϊ�?�����ѧE����

    while ( nextToken.type == AND ) {
        Match( AND, parsedResult );  // ���� '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end rest_of_maybe_logical_AND_exp()

  bool rest_of_maybe_bit_OR_exp(Token &parsedResult) {
    if (!rest_of_maybe_bit_ex_OR_exp(parsedResult)) return false;

    while (nextToken.type == BIT_OR) {
        Match(BIT_OR, parsedResult);
        if (!maybe_bit_ex_OR_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_bit_OR_exp()

  bool rest_of_maybe_bit_ex_OR_exp(Token &parsedResult) {
    if (!rest_of_maybe_bit_AND_exp(parsedResult)) return false;

    while (nextToken.type == BIT_XOR) {
        Match(BIT_XOR, parsedResult);
        if (!maybe_bit_AND_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_bit_ex_OR_exp()
  
  bool rest_of_maybe_bit_AND_exp(Token &parsedResult) {
    if (!rest_of_maybe_equality_exp(parsedResult)) return false;

    while (nextToken.type == BIT_AND) {
        Match(BIT_AND, parsedResult);
        if (!maybe_equality_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_bit_AND_exp()
  
  bool rest_of_maybe_equality_exp(Token &parsedResult) {
  	
    if (!rest_of_maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // ���� EQ �� NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_equality_exp()

  bool rest_of_maybe_relational_exp(Token &parsedResult) {
    if (!rest_of_maybe_shift_exp(parsedResult)) return false;

    while (nextToken.type == LESS || nextToken.type == GREATER || 
           nextToken.type == LE || nextToken.type == GE) {
        Match(nextToken.type, parsedResult);  // ���� <, >, <=, �� >=
        if (!maybe_shift_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_relational_exp()

  bool rest_of_maybe_shift_exp(Token &parsedResult) {
    if (!rest_of_maybe_additive_exp(parsedResult)) return false;

    while (nextToken.type == LS || nextToken.type == RS) {
        Match(nextToken.type, parsedResult);  // ���� LS �� RS
        if (!maybe_additive_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_shift_exp()

  bool rest_of_maybe_additive_exp(Token &parsedResult) {
    if (!rest_of_maybe_mult_exp(parsedResult)) return false;

    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // ���� '+' �� '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_additive_exp()

  bool rest_of_maybe_mult_exp(Token &parsedResult) {
    while (nextToken.type == MUL || nextToken.type == DIV || nextToken.type == MOD) {
        Match(nextToken.type, parsedResult);  // ���� '*', '/', �� '%'
        if (!unary_exp(parsedResult)) return false;
    }
    return true;  // ?�����i�H?�šA�]���q?��^ true
  }

  bool unary_exp( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      while ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        sign( parsedResult );  // ?�z�Ҧ��e�m����?
      } // end while
        
      return signed_unary_exp(parsedResult);
    } // end of
     
    else if ( nextToken.type == IDENTIFIER || nextToken.type == CONSTANT || nextToken.type == LPAREN ) {
      return unsigned_unary_exp(parsedResult);
    } // end else if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
      Match( nextToken.type, parsedResult ); // ���� PP �� MM
      if ( Match( IDENTIFIER, parsedResult ) ) {
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

  bool Match( Type expected, Token &parsedResult ) {
    if ( nextToken.type == expected ) {
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

#endif // PARSER_H

