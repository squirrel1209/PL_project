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

     
    while ( nextToken.type != QUIT ) {

      if ( nextToken.type == VOID || type_specifier() ) {
        definition( parsedResult );
      } // end if
      /*
      else if ( startExpression() ) {
        statement( parsedResult );
      } // end else if
      */

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
      
      if ( Match( IDENTIFIER, parsedResult ) ) 
        return function_definition_without_ID( parsedResult );
    } // end if
    
    else {
      Match( nextToken.type, parsedResult );
      gIdToeknName.push_back( nextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( function_definition_or_declarators( parsedResult ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) 
        	    printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
        	  else printf( "New definition of %s entered\n", gIdToeknName[i].c_str() );
        	  
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
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( nextToken.type == LPAREN ) {
      //return function_definition_without_ID();
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

    return Match( SEMICOLON, parsedResult );   // 結束分號
  } // end rest_of_declarators()

  void function_definition_without_ID( Token &parsedResult ) {
    if ( Match( LPAREN, parsedResult ) ) {
      if ( nextToken.type == VOID ) {
        Match( VOID, parsedResult ); // 匹配 VOID
      } // end if
      
      else if ( type_specifier() ) {
        formal_parameter_list( parsedResult ); // 解析 formal_parameter_list
      } // end else if
      
      else return;
      if ( !Match( RPAREN, parsedResult ) ) return;
    } // end if

    else return;
    compound_statement( parsedResult ); // 解析 compound_statement
  } // end function_definition_without_ID()
 
  bool formal_parameter_list( Token &parsedResult ) {
    Match( nextToken.type, parsedResult ) ;
    
    if ( nextToken.type == AMPERSAND ) {
      Match( AMPERSAND, parsedResult ); // 匹配 '&'
    } // end if
    
    if ( nextToken.type == IDENTIFIER ) {
      Match( IDENTIFIER, parsedResult ); 
    } // end if 
    else return false;
    
    if ( nextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult ); // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) {
        if ( !Match( RBRACKET, parsedResult ) ) return false;
      } // end if 
      
      else return false;
    } // end if

    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult ); // 匹配 ','
      if ( type_specifier() ) {
        formal_parameter_list( parsedResult );
      } // end if

      else Match( ERROR, parsedResult );
    } // end while
  } // end formal_parameter_list()
  /*
  void compound_statement() {
    bool iscompound_statement = true;
    if ( Match(LBRACE) ) {
      if ( ! ( type_specifier() )  ) iscompound_statement = false;
      while ( iscompound_statement ) {
        if ( type_specifier() ) {
        	
        } // end if
        
      } // end while 
    } // end if

    Match(RBRACE); // 匹配 '}'
  } // end  compound_statement()
  */
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

