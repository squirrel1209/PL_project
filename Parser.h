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

struct Variable {
  string typeName;
  string name;
};

struct Function {
  string typeName;
  vector<Variable> parameter;
  vector<string> body;
};

// 使用 map 來存儲函數名稱與其多行定義
map<string, Function> functionMap;

// 函數定義，並存儲到 map 中
  void DefineFunction( string name, string returnType, vector<Variable> params, vector<string> body) {
    Function func;
    func.typeName = returnType;
    func.parameter = params;
    func.body = body;
    functionMap[name] = func;
  }

// 列出所有函數名稱
void ListAllFunctions() {
    cout << "Listing all functions:" << endl;
    map<string, Function>::const_iterator it;
    for (it = functionMap.begin(); it != functionMap.end(); ++it) {
        cout << it->first << "()" << endl;
    }
}

// 列出指定函數的定義
void ListFunction( string name) {
    map<string, Function>::const_iterator it = functionMap.find(name);
    if (it != functionMap.end()) {
        cout << "> " << it->second.typeName << " " << name << "(";
        for (size_t i = 0; i < it->second.parameter.size(); ++i) {
            cout << it->second.parameter[i].typeName << " " << it->second.parameter[i].name;
            if (i < it->second.parameter.size() - 1) cout << ", ";
        }
        cout << ")" << endl;
        for (size_t i = 0; i < it->second.body.size(); ++i) {
            cout << it->second.body[i] << endl;
        }
    } else {
        cout << "Function " << name << " not found." << endl;
    }
}

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
    int startLine;
    
    while ( nextToken.type != QUIT && parsedResult.type != QUIT ) {
      startLine = nextToken.line;
      parsedResult = nextToken;
      
      if ( nextToken.type == VOID || type_specifier() ) {
        definition( parsedResult );
      } // end if
      
      else if ( StartExpression() || nextToken.type == SEMICOLON ) {
        bool istatement = statement( parsedResult );
        if ( parsedResult.type == QUIT ) {}
        else if ( istatement ) cout << "Statement executed ...\n"; 
      } // end else if

      if ( parsedResult.type == ERROR ) {
        if ( parsedResult.error == LEXICALERROR )
          printf( "Line %d : unrecognized token with first char : '%s'\n", parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SYNTACTICALERROR )
          printf( "Line %d : Unexpected token : '%s'\n", parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );
	 
        else if ( parsedResult.error == SEMANTICERROR )
          printf( "Line %d : Undefined identifier : '%s'\n", parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() ); 
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
        parsedResult.type = VOID;
        parsedResult.tokenName = nextToken.tokenName;
        if ( function_definition_without_ID( parsedResult ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) 
        	    printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
        	  else printf( "New definition of %s() entered...\n", gIdToeknName[i].c_str() );
        	  
        	  gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
        } // end if
      } // end if
    } // end if
    
    else {
      bool function = false;
      Match( nextToken.type, parsedResult );
      gIdToeknName.push_back( nextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( function_definition_or_declarators( parsedResult, function ) ) {
        	for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
        		
        	  if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) {
        	    if ( !function )
        	      printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
        	    else printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
            } // end if
            
        	  else {
	    if ( !function )
	      printf( "New definition of %s entered...\n", gIdToeknName[i].c_str() );
              else
                printf( "New definition of %s() entered...\n", gIdToeknName[i].c_str() );
            } // end else
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
 
  bool function_definition_or_declarators( Token &parsedResult, bool &function ) {
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( nextToken.type == LPAREN ) {
      function = true;
      return function_definition_without_ID( parsedResult  );
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

  bool function_definition_without_ID( Token &parsedResult ) {
    if ( !Match( LPAREN, parsedResult ) ) return false; 

    if ( nextToken.type == VOID ) {
      Match( VOID, parsedResult ); // 匹配 VOID
      
    } // end if

    else if ( type_specifier() ) 
      if ( !formal_parameter_list( parsedResult ) ) return false;// 解析 formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return compound_statement( parsedResult ); // 解析 compound_statement
  } // end function_definition_without_ID()
 
  bool formal_parameter_list( Token &parsedResult ) {
    Type type = nextToken.type;

    Match( nextToken.type, parsedResult ) ;
    
    if ( nextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult ); // 匹配 '&'
    } // end if
    
    if ( nextToken.type == IDENTIFIER ) {
      gsymbolTable[nextToken.tokenName] = type;
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

    bool isformal_parameter_list = true;
    while ( nextToken.type == COMMA && isformal_parameter_list ) {
      Match( COMMA, parsedResult ); // 匹配 ','
      if ( type_specifier() ) {
        isformal_parameter_list = formal_parameter_list( parsedResult );
      } // end if

      else Match( ERROR, parsedResult );
    } // end while
    
    return isformal_parameter_list;
  } // end formal_parameter_list()

  bool compound_statement( Token &parsedResult ) {
    bool iscompound_statement = false;
    if ( !Match( LBRACE, parsedResult ) ) return false;
      iscompound_statement = statement( parsedResult );
    
    if ( iscompound_statement == false )
      iscompound_statement = declaration( parsedResult );
    
    while ( iscompound_statement ) {
      iscompound_statement = statement( parsedResult );
    
      if ( iscompound_statement == false )
        iscompound_statement = declaration( parsedResult );
    } // end while

    if ( !Match( RBRACE, parsedResult ) ) return false;
    return true;
  } // end  compound_statement()

  bool declaration( Token &parsedResult ) {
    if ( !type_specifier() ) return false;
    Type type = nextToken.type;
    Match( nextToken.type, parsedResult );
    
    string tokenName = nextToken.tokenName;
      
    if ( Match( IDENTIFIER, parsedResult ) ) {
      if ( rest_of_declarators( parsedResult ) ) {
        gsymbolTable[tokenName] = type;
        return true;
      } // end if
    }  // end if
    
    return false;
  } // end declaration()

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
    
    else if ( nextToken.type == RETURN ) {
      Match( RETURN, parsedResult );
      if ( nextToken.type != SEMICOLON ) {
        if ( !expression( parsedResult ) ) return false;
      } // end if

      if ( !Match( SEMICOLON, parsedResult ) ) {
        return false; // 或者進行其他錯誤處理
      } // end if
      
      return true;
    } // end else if
    
    else if ( nextToken.type == LBRACE ) {
      if ( !compound_statement( parsedResult ) ) return false;
      return true;
    } // end if 
    
    else if ( nextToken.type == IF ) {
      Match( IF, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !statement( parsedResult ) ) return false;
      
      if ( nextToken.type == ELSE ) {
        Match( IF, parsedResult );
        if ( !statement( parsedResult ) ) return false;
      } // end if 
      
      return true;
    } // end else if 
    
    else if (nextToken.type == WHILE) {
      Match( WHILE, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !statement( parsedResult ) ) return false; 
    } // end else if
    
    else if ( nextToken.type == DO ) {
      Match( DO, parsedResult );
      if ( !statement( parsedResult ) ) return false; 
      if ( !Match( WHILE, parsedResult ) ) return false;
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if

    return false;
  } // end statement()

  bool StartExpression() {
    if ( nextToken.type == IDENTIFIER || nextToken.type == CONSTANT ||
         nextToken.type == PP || nextToken.type == MM || nextToken.type == LPAREN ||
         nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT )
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
  } // end expression

  bool basic_expression( Token &parsedResult ) {

    if ( nextToken.type == IDENTIFIER ) { // undefine
      
      if ( nextToken.tokenName.compare( "cin" ) == 0 || nextToken.tokenName.compare( "cout" ) == 0 ) {
      } // end if
      
      else if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
        parsedResult = nextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;
        
        if ( nextToken.tokenName.compare( "Done" ) == 0 ) {
        	Match( IDENTIFIER, parsedResult );
        	if ( !rest_of_Identifier_started_basic_exp( parsedResult ) ) return false;
        	
        	return true;
        } // end if
        
        return false;
      }  // end if 
      
      return Match( IDENTIFIER, parsedResult )
      && rest_of_Identifier_started_basic_exp( parsedResult );
    } // end if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
      if ( !Match( nextToken.type, parsedResult ) ) return false;
      if ( nextToken.type != IDENTIFIER ) return false;
      if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
          parsedResult = nextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SEMANTICERROR;
          return false;
      }  // end if 
      
      Match( IDENTIFIER, parsedResult );
      return  rest_of_PPMM_Identifier_started_basic_exp( parsedResult );
    } // end else if 
    
    else if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      sign( parsedResult );
      while ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        sign( parsedResult ); 
      } // end while

      return signed_unary_exp( parsedResult ) && romce_and_romloe( parsedResult );
    } // end else if 
    
    else if ( nextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult ) && romce_and_romloe( parsedResult );
    } // end else if
    
    else if ( nextToken.type == LPAREN ) {

      return Match( LPAREN, parsedResult ) &&
             expression( parsedResult ) &&
             Match( RPAREN, parsedResult ) &&
             romce_and_romloe( parsedResult );
    } // end else if
    return false;
  } // end basic_expression()

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
    
    else romce_and_romloe( parsedResult );
    
    if ( nextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult );
      
      if ( nextToken.type == RPAREN ) {
        Match( RPAREN, parsedResult );
        if ( nextToken.type == SEMICOLON ) {
        	parsedResult.type = QUIT;
        	return true;
        } // end if
        
        return romce_and_romloe( parsedResult );
      } // end if
       
      else if ( !actual_parameter_list( parsedResult ) ) return false;

      if ( !Match( RPAREN, parsedResult ) ) return false;
    } // end if
    
    return true;
  } // end rest_of_Identifier_started_basic_exp()

  bool rest_of_PPMM_Identifier_started_basic_exp( Token &parsedResult ) {
    if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        if ( !expression(parsedResult) ) return false;  // 解析表?式
        if ( !Match( RBRACKET, parsedResult ) ) return false;  // 消耗 ']'
    } // end if

    return romce_and_romloe( parsedResult );  // 解析后?表?式
  } // end rest_of_PPMM_Identifier_started_basic_exp()

  bool actual_parameter_list( Token &parsedResult ) {
    if ( !basic_expression( parsedResult ) ) return false;
    
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // 消耗 ','
      if ( !basic_expression( parsedResult ) ) return false;  
    } // end while
    
    return true;
  } // end actual_parameter_list()

  bool assignment_operator( Token &parsedResult ) {
    if ( nextToken.type == ASSIGN || nextToken.type == TE || nextToken.type == DE || 
        nextToken.type == RE || nextToken.type == PE || nextToken.type == ME) {
      return Match( nextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end assignment_operator()

  bool romce_and_romloe( Token &parsedResult ) {
    
    if ( !rest_of_maybe_logical_OR_exp( parsedResult ) ) {
      return false;  // ?理??或表?式
    }
    
    if ( nextToken.type == QUESTION ) {
        Match( QUESTION, parsedResult );  // 消耗 '?'
        if ( !basic_expression( parsedResult ) ) return false;  // 解析?件?真?的表?式
        if ( !Match( COLON, parsedResult ) ) return false;  // 消耗 ':'
        if ( !basic_expression( parsedResult ) ) return false;  // 解析?件?假?的表?式
    } // end if


    return true;
  } // end romce_and_romloe()

  bool rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?理??与表?式

    while ( nextToken.type == OR ) {
      Match( OR, parsedResult );  // 消耗 '||'
      if ( !maybe_logical_AND_exp( parsedResult ) ) return false;  // 解析??与表?式
    } // end while
    
    return true;
  } // end rest_of_maybe_logical_OR_exp()

  bool maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位或表?式

    while (nextToken.type == AND) {
        Match( AND, parsedResult );  // 消耗 '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end maybe_logical_AND_exp()

  bool rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理位或表?式的剩余部分

    while ( nextToken.type == AND ) {
        Match( AND, parsedResult );  // 消耗 '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end rest_of_maybe_logical_AND_exp()

  bool maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位异或表?式

    while ( nextToken.type == BIT_OR ) {
      Match( BIT_OR, parsedResult );  // 消耗 '|'
      if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理后?的位异或表?式
    } // end while

    return true;
  } // end maybe_bit_OR_exp()

  bool rest_of_maybe_bit_OR_exp(Token &parsedResult) {
    if ( !rest_of_maybe_bit_ex_OR_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_OR)  {
      Match (BIT_OR, parsedResult );
      if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_bit_OR_exp()

  bool maybe_bit_ex_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end maybe_bit_ex_OR_exp()

  bool rest_of_maybe_bit_ex_OR_exp(Token &parsedResult) {
    if ( !rest_of_maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_bit_ex_OR_exp()
  
  bool maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !maybe_equality_exp( parsedResult ) ) return false;

    while (nextToken.type == BIT_AND) {
      Match(BIT_AND, parsedResult);
      if (!maybe_equality_exp(parsedResult)) return false;
    } // end while

    return true;
  } // end maybe_bit_AND_exp()
  
  bool rest_of_maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_equality_exp( parsedResult ) ) return false;

    while ( nextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult );
      if ( !maybe_equality_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_bit_AND_exp()
  
  bool maybe_equality_exp(Token &parsedResult) {
    if ( !maybe_relational_exp( parsedResult ) ) return false;

    while ( nextToken.type == EQ || nextToken.type == NEQ ) {
      Match( nextToken.type, parsedResult );  // 消耗 EQ 或 NEQ
      if ( !maybe_relational_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end maybe_equality_exp()
  
  bool rest_of_maybe_equality_exp(Token &parsedResult) {
  	
    if (!rest_of_maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // 消耗 EQ 或 NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }
    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_equality_exp()

  bool maybe_relational_exp(Token &parsedResult) {
    if ( !maybe_shift_exp( parsedResult ) ) return false;

    while ( nextToken.type == GT || nextToken.type == LT || 
           nextToken.type == LE || nextToken.type == GE) {
      Match( nextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !maybe_shift_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end maybe_relational_exp()

  bool rest_of_maybe_relational_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_shift_exp( parsedResult ) ) return false;

    while (nextToken.type == GT || nextToken.type == LT || 
           nextToken.type == LE || nextToken.type == GE) {
      Match( nextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !maybe_shift_exp( parsedResult ) ) return false;
    } // end while
    // cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_relational_exp()

  bool maybe_shift_exp( Token &parsedResult ) {
    if ( !maybe_additive_exp( parsedResult ) ) return false;

    while ( nextToken.type == LS || nextToken.type == RS ) {
      Match( nextToken.type, parsedResult );  // 消耗 << 或 >>
      if ( !maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // maybe_shift_exp()

  bool rest_of_maybe_shift_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_additive_exp( parsedResult ) ) return false;

    while ( nextToken.type == LS || nextToken.type == RS ) {
        Match( nextToken.type, parsedResult );  // 消耗 LS 或 RS
        if ( !maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    //cout << nextToken.tokenName;
    return true;
  } // end rest_of_maybe_shift_exp()

  bool maybe_additive_exp( Token &parsedResult ) {
    if ( !maybe_mult_exp( parsedResult ) ) return false;

    while ( nextToken.type == PLUS || nextToken.type == MINUS ) {
      Match( nextToken.type, parsedResult );  // 消耗 '+' 或 '-'
      if ( !maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end maybe_additive_exp()

  bool rest_of_maybe_additive_exp(Token &parsedResult) {
    if (!rest_of_maybe_mult_exp(parsedResult)) return false;
    
    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // 消耗 '+' 或 '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
  } // end rest_of_maybe_additive_exp()

  bool maybe_mult_exp( Token &parsedResult ) {
    // cout << nextToken.tokenName << endl;
    if ( !unary_exp( parsedResult ) ) return false;
    return rest_of_maybe_mult_exp( parsedResult );  
  } // end maybe_mult_exp()

  bool rest_of_maybe_mult_exp(Token &parsedResult) {
    while (nextToken.type == MUL || nextToken.type == DIV || nextToken.type == MOD) {
        Match(nextToken.type, parsedResult);  
        if (!unary_exp(parsedResult)) return false;
    }
    return true;  
  } // end rest_of_maybe_mult_exp()

  bool unary_exp( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      while ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        sign( parsedResult );
      } // end while
        
      return signed_unary_exp(parsedResult);
    } // end of
     
    else if ( nextToken.type == IDENTIFIER || nextToken.type == CONSTANT || nextToken.type == LPAREN ) {
      return unsigned_unary_exp(parsedResult);
    } // end else if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
      Match( nextToken.type, parsedResult ); // 消耗 PP 或 MM
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
          if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
              parsedResult = nextToken;
              parsedResult.type = ERROR;
              parsedResult.error = SEMANTICERROR;
              return false;
          }  // end if 
        
        if ( nextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult ); // 消耗 '['
          if (!expression(parsedResult)) return false; 
          if (!Match(RBRACKET, parsedResult)) return false; // 消耗 ']'
        } // end if
        return true;
      } // end if
        return false;
    } // end else if
    
    return false;
  } // end unary_exp()

  bool signed_unary_exp( Token &parsedResult ) {
    if ( nextToken.type == IDENTIFIER ) {
        if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
            parsedResult = nextToken;
            parsedResult.type = ERROR;
            parsedResult.error = SEMANTICERROR;
            return false;
        }  // end if 
        
      Match( IDENTIFIER, parsedResult );
      if ( nextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        
        if ( nextToken.type != RPAREN ) {
          if ( !actual_parameter_list( parsedResult ) ) return false;
        } // end if
         
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
      } // end else if
    
      return true;
    } // end if 
    
    else if ( nextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if (nextToken.type == LPAREN) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      if ( !expression( parsedResult ) ) return false;
      return Match(RPAREN, parsedResult); // 消耗 ')'
    } // end else if
    
    return false;
  }

  bool unsigned_unary_exp( Token &parsedResult ) {
    if ( nextToken.type == IDENTIFIER ) {
        if ( gsymbolTable.find( nextToken.tokenName ) == gsymbolTable.end() ) {
            parsedResult = nextToken;
            parsedResult.type = ERROR;
            parsedResult.error = SEMANTICERROR;
            return false;
        }  // end if 

      Match( IDENTIFIER, parsedResult );
      
      if ( nextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        if ( nextToken.type != RPAREN ) {
          if ( !actual_parameter_list( parsedResult ) ) return false;
        } // end if
 
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
        if ( nextToken.type == PP || nextToken.type == MM ) {
          Match( nextToken.type, parsedResult );
        } // end if
      } // end else if
    
      return true;
    } // end if 
    
    else if ( nextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if (nextToken.type == LPAREN) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      
      if ( !expression( parsedResult ) ) return false;
      
      return Match(RPAREN, parsedResult); // 消耗 ')'
    } // end else if
    
    return false;
  } // end unsigned_unary_exp

  bool sign( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      return Match( nextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end sign()

  bool Match( Type expected, Token &parsedResult ) {
    if ( nextToken.type == expected ) {
      parsedResult.content.push_back( nextToken.tokenName );
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

