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

vector<Variable> gTempVariable;

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
    for ( it = functionMap.begin(); it != functionMap.end(); ++it ) {
        cout << it->first << "()" << endl;
    }
}

// 列出指定函數的定義
void ListFunction( string name ) {
    map<string, Function>::const_iterator it = functionMap.find(name);
    if (it != functionMap.end()) {
    	cout << "> " ;
        for (size_t i = 0; i < it->second.body.size(); ++i) {
            cout << it->second.body[i] ;
            if ( it->second.body[i].compare( ";" ) == 0 || it->second.body[i].compare( "{" ) == 0 || it->second.body[i].compare( "}" ) == 0 ) cout << endl;
            else if ( i+1 < it->second.body.size() && it->second.body[i+1].compare( "(" ) == 0 ) {}
	  else cout << ' ';
        }
    } else {
        cout << "Function " << name << " not found." << endl;
    }
}

class Parser {
public:
  Parser() {
    mnextToken = gtokenizer.GetNextToken();
  } // end Parser()
  
  void Parse() {
    User_input();
  } // end Parse() 

private:
  Token mnextToken;

  void User_input() {
    Token parsedResult ;
    int startLine;

    while ( mnextToken.type != QUIT && parsedResult.type != QUIT ) {

      startLine = mnextToken.line;
      parsedResult = mnextToken;
      
      if ( mnextToken.type == VOID || Type_specifier() ) {
        Definition( parsedResult );
      } // end if

      else if ( StartExpression() || mnextToken.type == SEMICOLON || mnextToken.type == IF ||
                mnextToken.type == WHILE || mnextToken.type == DO || mnextToken.type == RETURN ||
                mnextToken.type == ELSE ) {
        bool iStatement = Statement( parsedResult );
        if ( parsedResult.type == QUIT ) { }
        else if ( iStatement ) cout << "Statement executed ...\n"; 
      } // end else if

      if ( parsedResult.type == ERROR ) {
        if ( parsedResult.error == LEXICALERROR )
          printf( "Line %d : unrecognized token with first char : '%s'\n", 
                  parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SYNTACTICALERROR )
          printf( "Line %d : Unexpected token : '%s'\n", 
                  parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() );

        else if ( parsedResult.error == SEMANTICERROR )
          printf( "Line %d : Undefined identifier : '%s'\n", 
                  parsedResult.line - startLine + 1, parsedResult.tokenName.c_str() ); 
        else cout << "> Error" << endl ;
        
        while ( parsedResult.line >= mnextToken.line ) mnextToken = gtokenizer.GetNextToken();
      } // end if
      
    } // end while
    
    gsymbolTable.clear();
  } // end User_input()
  
  bool Definition( Token &parsedResult ) {
    Type type = mnextToken.type;
    vector<Variable> variable;
    Variable temp;
    if ( mnextToken.type == VOID ) {
      Match( VOID, parsedResult );
      gIdToeknName.push_back( mnextToken.tokenName );
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        parsedResult.type = VOID;
        parsedResult.tokenName = mnextToken.tokenName;
        parsedResult.content.clear(); 
        if ( Function_definition_without_ID( parsedResult ) ) {
          for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
            if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) 
              printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
            else printf( "New Definition of %s() entered...\n", gIdToeknName[i].c_str() );

            temp.typeName = "void";
            temp.name = "void";
            variable.push_back( temp );
            DefineFunction( parsedResult.tokenName, "void", variable, parsedResult.content );
            gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
        } // end if
      } // end if
    } // end if
    
    else {
      bool function = false;
      parsedResult.type = mnextToken.type;
      parsedResult.content.clear();
      gTempVariable.clear();
      
      Match( mnextToken.type, parsedResult );
      gIdToeknName.push_back( mnextToken.tokenName );
      parsedResult.tokenName = mnextToken.tokenName;
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( Function_definition_or_declarators( parsedResult, function ) ) {
          for ( int i = 0 ; i < gIdToeknName.size() ; i++ ) {
            if ( gsymbolTable.find( gIdToeknName[i] ) == gsymbolTable.end() ) {
              if ( !function ) {
                printf( "Definition of %s entered ...\n", gIdToeknName[i].c_str() );
              } // end if
              else {
                DefineFunction( parsedResult.tokenName, ToString( parsedResult.type ), 
                                gTempVariable, parsedResult.content );
                printf( "Definition of %s() entered ...\n", gIdToeknName[i].c_str() );
              } // end else
            } // end if

            else {
              if ( !function )
                printf( "New Definition of %s entered...\n", gIdToeknName[i].c_str() );
              else {
                DefineFunction( parsedResult.tokenName, ToString( parsedResult.type ), 
                                gTempVariable, parsedResult.content );
                printf( "New Definition of %s() entered...\n", gIdToeknName[i].c_str() );
              } // end else
            } // end else

            gsymbolTable[gIdToeknName[i]] = type;
          } // end for
          
          gIdToeknName.clear();
          return true;
        } // end if
      }  // end if
    } // end else
    
    return false;
  } // end Definition()
  
  bool Type_specifier() {
    return mnextToken.type == INT || mnextToken.type == FLOAT ||
    mnextToken.type == CHAR || mnextToken.type == BOOL || mnextToken.type == STRING ;
  } // end Type_specifier()
 
  bool Function_definition_or_declarators( Token &parsedResult, bool &function ) {
    // 檢查下一個符號是否是左括號，如果是，則處理為函數定義
    if ( mnextToken.type == LPAREN ) {
      function = true;
      return Function_definition_without_ID( parsedResult  );
    } // end if
    
    else {
    // 如果下一個符號不是左括號，則處理為變量宣告
      return Rest_of_declarators( parsedResult );
    } // end else
  } // end Function_definition_or_declarators()

  bool Rest_of_declarators( Token &parsedResult ) {
    // 檢查是否存在 '[' Constant ']' 結構
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );   // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) 
        Match( RBRACKET, parsedResult );
      
      else return false;
    } // end if

    // 處理 { ',' Identifier [ '[' Constant ']' ] } 結構
    while ( mnextToken.type == COMMA ) {
      Match( COMMA, parsedResult );
      
      if ( mnextToken.type == IDENTIFIER ) {
        gIdToeknName.push_back( mnextToken.tokenName );
        Match( IDENTIFIER, parsedResult ); // 匹配標識符
        
        // 檢查是否存在 '[' Constant ']' 結構
        if ( Match( LBRACKET, parsedResult ) ) {

          if ( Match( CONSTANT, parsedResult ) ) 
            Match( RBRACKET, parsedResult ) ;
          else return false ;
        } // end if
        else return false;
      } // end if 
      
      else {
        if ( mnextToken.type != ERROR ) {
          parsedResult = mnextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SYNTACTICALERROR;
        } // end if
        
        else parsedResult = mnextToken;
        
        return false;
      } // end else
    } // end while

    return Match( SEMICOLON, parsedResult );   // 結束分號
  } // end Rest_of_declarators()

  bool Function_definition_without_ID( Token &parsedResult ) {
    if ( !Match( LPAREN, parsedResult ) ) return false; 

    if ( mnextToken.type == VOID ) {
      Match( VOID, parsedResult ); // 匹配 VOID
    } // end if

    else if ( Type_specifier() ) 
      if ( !Formal_parameter_list( parsedResult ) ) return false; // 解析 Formal_parameter_list

    if ( !Match( RPAREN, parsedResult ) ) return false;

    return Compound_Statement( parsedResult ); // 解析 Compound_Statement
  } // end Function_definition_without_ID()
 
  bool Formal_parameter_list( Token &parsedResult ) {
    Variable variable;
    Type type = mnextToken.type;
    
    Match( mnextToken.type, parsedResult ) ;
    
    if ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult ); // 匹配 '&'
    } // end if
    
    if ( mnextToken.type == IDENTIFIER ) {
      gsymbolTable[mnextToken.tokenName] = type;
      variable.typeName = ToString( type );
      variable.name = mnextToken.tokenName;
      gTempVariable.push_back( variable );
      Match( IDENTIFIER, parsedResult ); 
    } // end if  
    
    else return false;
    
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult ); // 匹配 '['
      
      if ( Match( CONSTANT, parsedResult ) ) {
        if ( !Match( RBRACKET, parsedResult ) ) return false;
      } // end if 
      
      else return false;
    } // end if

    bool isFormal_parameter_list = true;
    while ( mnextToken.type == COMMA && isFormal_parameter_list ) {
      Match( COMMA, parsedResult ); // 匹配 ','
      if ( Type_specifier() ) {
        isFormal_parameter_list = Formal_parameter_list( parsedResult );
      } // end if

      else Match( ERROR, parsedResult );
    } // end while
    
    return isFormal_parameter_list;
  } // end Formal_parameter_list()

  bool Compound_Statement( Token &parsedResult ) {
    bool isCompound_Statement = false;
    if ( !Match( LBRACE, parsedResult ) ) return false;
    isCompound_Statement = StartStatement();
    if ( isCompound_Statement ) isCompound_Statement = Statement( parsedResult );
    else isCompound_Statement = Declaration( parsedResult );

    
    while ( isCompound_Statement ) {
      isCompound_Statement = StartStatement();
      if ( isCompound_Statement ) isCompound_Statement = Statement( parsedResult );
      else isCompound_Statement = Declaration( parsedResult );
    } // end while

    
    if ( !Match( RBRACE, parsedResult ) ) return false;
    return true;
  } // end Compound_Statement()

  bool Declaration( Token &parsedResult ) {
    if ( !Type_specifier() ) return false;
    Type type = mnextToken.type;
    Match( mnextToken.type, parsedResult );
    
    string tokenName = mnextToken.tokenName;
      
    if ( Match( IDENTIFIER, parsedResult ) ) {
      if ( Rest_of_declarators( parsedResult ) ) {
        gsymbolTable[tokenName] = type;
        return true;
      } // end if
    }  // end if
    
    return false;
  } // end Declaration()

  bool Statement( Token &parsedResult ) {
    if ( mnextToken.type == SEMICOLON ) {
      Match( SEMICOLON, parsedResult );
      return true;
    } // end if 
    
    else if ( StartExpression() ) {
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if
    
    else if ( mnextToken.type == RETURN ) {
      Match( RETURN, parsedResult );
      if ( mnextToken.type != SEMICOLON ) {
        if ( !Expression( parsedResult ) ) return false;
      } // end if

      if ( !Match( SEMICOLON, parsedResult ) ) {
        return false; // 或者進行其他錯誤處理
      } // end if
      
      return true;
    } // end else if
    
    else if ( mnextToken.type == LBRACE ) {
      if ( !Compound_Statement( parsedResult ) ) return false;
      return true;
    } // end if 
    
    else if ( mnextToken.type == IF ) {
      Match( IF, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Statement( parsedResult ) ) return false;
      
      if ( mnextToken.type == ELSE ) {
        Match( ELSE, parsedResult );
        if ( !Statement( parsedResult ) ) return false;
      } // end if 
      
      return true;
    } // end else if 
    
    else if ( mnextToken.type == WHILE ) {
      Match( WHILE, parsedResult );
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Statement( parsedResult ) ) return false; 
      return true;
    } // end else if
    
    else if ( mnextToken.type == DO ) {
      Match( DO, parsedResult );
      if ( !Statement( parsedResult ) ) return false; 
      if ( !Match( WHILE, parsedResult ) ) return false;
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      if ( !Match( SEMICOLON, parsedResult ) ) return false;
      return true;
    } // end else if

    else Match( ERROR, parsedResult );
    return false;
  } // end Statement()

  bool StartStatement() {
    if ( StartExpression() || mnextToken.type == SEMICOLON || mnextToken.type == IF ||
         mnextToken.type == WHILE || mnextToken.type == DO || mnextToken.type == RETURN ||
         mnextToken.type == ELSE ) return true;

    else return false;
  } // end StartStatement()

  bool StartExpression() {
    if ( mnextToken.type == IDENTIFIER || mnextToken.type == CONSTANT ||
         mnextToken.type == PP || mnextToken.type == MM || mnextToken.type == LPAREN ||
         mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT )
      return true;
    else return false;
  } // end StartExpression()

  bool Expression( Token &parsedResult ) {
    bool expression = true; 
    if ( Basic_Expression( parsedResult ) ) {

      while ( mnextToken.type == COMMA && expression ) {
        Match( COMMA, parsedResult ); 
        expression = Basic_Expression( parsedResult );
      } // end while
      
      return expression;
    } // end if
    
    return false;
  } // end Expression()

  bool Basic_Expression( Token &parsedResult ) {

    if ( mnextToken.type == IDENTIFIER ) { // undefine
      
      if ( mnextToken.tokenName.compare( "cin" ) == 0 || mnextToken.tokenName.compare( "cout" ) == 0 ) {
      } // end if
      
      else if ( mnextToken.tokenName.compare( "ListAllFunctions" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;
        ListAllFunctions();
        return true;
      } // end if
      
      else if ( mnextToken.tokenName.compare( "ListFunction" ) == 0 ) {
        Match( IDENTIFIER, parsedResult );
        
        if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;
        
        ListFunction( parsedResult.tokenName ); 
        return true;
      } // end if      

      else if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;
        
        if ( mnextToken.tokenName.compare( "Done" ) == 0 ) {
          parsedResult.tokenName = mnextToken.tokenName;
          Match( IDENTIFIER, parsedResult );
          if ( !Rest_of_identifier_started_basic_exp( parsedResult ) ) return false;

          return true;
        } // end if
        
        return false;
      }  // end if 
      
      if ( !Match( IDENTIFIER, parsedResult ) ) return false;
      return Rest_of_identifier_started_basic_exp( parsedResult );
    } // end if 
    
    else if ( mnextToken.type == PP || mnextToken.type == MM ) {
      if ( !Match( mnextToken.type, parsedResult ) ) return false;
      if ( mnextToken.type != IDENTIFIER ) return false;
      if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;
        return false;
      }  // end if 
      
      Match( IDENTIFIER, parsedResult );
      return  Rest_of_PPMM_Identifier_started_basic_exp( parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
      Sign( parsedResult );
      while ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
        Sign( parsedResult ); 
      } // end while

      return Signed_Unary_exp( parsedResult ) && Romce_and_romloe( parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == CONSTANT ) {
      parsedResult.tokenName = mnextToken.tokenName;
      return Match( CONSTANT, parsedResult ) && Romce_and_romloe( parsedResult );
    } // end else if
    
    else if ( mnextToken.type == LPAREN ) {
      if ( !Match( LPAREN, parsedResult ) ) return false;
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return Romce_and_romloe( parsedResult );
    } // end else if
    
    return false;
  } // end Basic_Expression()

  bool Rest_of_identifier_started_basic_exp( Token &parsedResult ) {
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );
      if ( !Expression( parsedResult ) ) return false;
      if ( !Match( RBRACKET, parsedResult ) ) return false;
    } // end if

    if ( AsSignment_operator( parsedResult ) ) {
      if ( !Basic_Expression( parsedResult ) ) return false;
    } // end if 
    
    else if ( mnextToken.type == PP || mnextToken.type == MM ) {
      Match( mnextToken.type, parsedResult );
      if ( !Romce_and_romloe( parsedResult ) ) return false;
    } // end else if
    
    else Romce_and_romloe( parsedResult );
    
    if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult );
      
      if ( mnextToken.type == RPAREN ) {
        Match( RPAREN, parsedResult );
        if ( mnextToken.type == SEMICOLON && parsedResult.tokenName.compare( "Done" ) == 0 ) {
          parsedResult.type = QUIT;
          return true;
        } // end if
        
        return Romce_and_romloe( parsedResult );
      } // end if
       
      else if ( !Actual_parameter_list( parsedResult ) ) return false;
      
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return Romce_and_romloe( parsedResult );
    } // end if
    
    return true;
  } // end Rest_of_identifier_started_basic_exp()

  bool Rest_of_PPMM_Identifier_started_basic_exp( Token &parsedResult ) {
    if ( mnextToken.type == LBRACKET ) {
      Match( LBRACKET, parsedResult );
      if ( !Expression( parsedResult ) ) return false; 
      if ( !Match( RBRACKET, parsedResult ) ) return false; 
    } // end if

    return Romce_and_romloe( parsedResult );  // 解析后?表?式
  } // end Rest_of_PPMM_Identifier_started_basic_exp()

  bool Actual_parameter_list( Token &parsedResult ) {
    if ( !Basic_Expression( parsedResult ) ) return false;
    
    while ( mnextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // 消耗 ','
      if ( !Basic_Expression( parsedResult ) ) return false;  
    } // end while
    
    return true;
  } // end Actual_parameter_list()

  bool AsSignment_operator( Token &parsedResult ) {
    if ( mnextToken.type == ASSIGN || mnextToken.type == TE || mnextToken.type == DE || 
         mnextToken.type == RE || mnextToken.type == PE || mnextToken.type == ME ) {
      return Match( mnextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end AsSignment_operator()

  bool Romce_and_romloe( Token &parsedResult ) {
    
    if ( !Rest_of_maybe_logical_OR_exp( parsedResult ) ) {
      return false; 
    } // end if 
    
    if ( mnextToken.type == QUESTION ) {
      Match( QUESTION, parsedResult );  // 消耗 '?'
      if ( !Basic_Expression( parsedResult ) ) return false;  // 解析?件?真?的表?式
      if ( !Match( COLON, parsedResult ) ) return false;  // 消耗 ':'
      if ( !Basic_Expression( parsedResult ) ) return false;  // 解析?件?假?的表?式
    } // end if


    return true;
  } // end Romce_and_romloe()

  bool Rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?理??与表?式

    while ( mnextToken.type == OR ) {
      Match( OR, parsedResult );  // 消耗 '||'
      if ( !Maybe_logical_AND_exp( parsedResult ) ) return false;  // 解析??与表?式
    } // end while
    
    return true;
  } // end Rest_of_maybe_logical_OR_exp()

  bool Maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位或表?式

    while ( mnextToken.type == AND ) {
      Match( AND, parsedResult );  // 消耗 '&&'
      if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理位或表?式的剩余部分

    while ( mnextToken.type == AND ) {
      Match( AND, parsedResult );  // 消耗 '&&'
      if ( !Maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end Rest_of_maybe_logical_AND_exp()

  bool Maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位异或表?式

    while ( mnextToken.type == BIT_OR ) {
      Match( BIT_OR, parsedResult );  // 消耗 '|'
      if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理后?的位异或表?式
    } // end while

    return true;
  } // end Maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_ex_OR_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_OR )  {
      Match( BIT_OR, parsedResult );
      if ( !Maybe_bit_ex_OR_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_bit_OR_exp()

  bool Maybe_bit_ex_OR_exp( Token &parsedResult ) {
    if ( !Maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !Maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end Maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_bit_AND_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_XOR ) {
      Match( BIT_XOR, parsedResult );
      if ( !Maybe_bit_AND_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_bit_ex_OR_exp()
  
  bool Maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !Maybe_equality_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult );
      if ( !Maybe_equality_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_bit_AND_exp()
  
  bool Rest_of_maybe_bit_AND_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_equality_exp( parsedResult ) ) return false;

    while ( mnextToken.type == BIT_AND ) {
      Match( BIT_AND, parsedResult );
      if ( !Maybe_equality_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_bit_AND_exp()
  
  bool Maybe_equality_exp( Token &parsedResult ) {
    if ( !Maybe_relational_exp( parsedResult ) ) return false;

    while ( mnextToken.type == EQ || mnextToken.type == NEQ ) {
      Match( mnextToken.type, parsedResult );  // 消耗 EQ 或 NEQ
      if ( !Maybe_relational_exp( parsedResult ) ) return false;
    } // end while 

    return true;
  } // end Maybe_equality_exp()
  
  bool Rest_of_maybe_equality_exp( Token &parsedResult ) {

    if ( !Rest_of_maybe_relational_exp( parsedResult ) ) return false;

    while ( mnextToken.type == EQ || mnextToken.type == NEQ ) {
      Match( mnextToken.type, parsedResult );  // 消耗 EQ 或 NEQ
      if ( !Maybe_relational_exp( parsedResult ) ) return false;
    } // end while
    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_equality_exp()

  bool Maybe_relational_exp( Token &parsedResult ) {
    if ( !Maybe_shift_exp( parsedResult ) ) return false;

    while ( mnextToken.type == GT || mnextToken.type == LT || 
            mnextToken.type == LE || mnextToken.type == GE ) {
      Match( mnextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !Maybe_shift_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_shift_exp( parsedResult ) ) return false;
    while ( mnextToken.type == GT || mnextToken.type == LT || 
            mnextToken.type == LE || mnextToken.type == GE ) {
      Match( mnextToken.type, parsedResult );  // 消耗 <, >, <=, 或 >=
      if ( !Maybe_shift_exp( parsedResult ) ) return false;
    } // end while
    
    return true;
  } // end Rest_of_maybe_relational_exp()

  bool Maybe_shift_exp( Token &parsedResult ) {
    if ( !Maybe_additive_exp( parsedResult ) ) return false;

    while ( mnextToken.type == LS || mnextToken.type == RS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 << 或 >>
      if ( !Maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_additive_exp( parsedResult ) ) return false;

    while ( mnextToken.type == LS || mnextToken.type == RS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 LS 或 RS
      if ( !Maybe_additive_exp( parsedResult ) ) return false;
    } // end while

    // cout << mnextToken.tokenName;
    return true;
  } // end Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp( Token &parsedResult ) {
    if ( !Maybe_mult_exp( parsedResult ) ) return false;

    while ( mnextToken.type == PLUS || mnextToken.type == MINUS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 '+' 或 '-'
      if ( !Maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp( Token &parsedResult ) {
    if ( !Rest_of_maybe_mult_exp( parsedResult ) ) return false;
    
    while ( mnextToken.type == PLUS || mnextToken.type == MINUS ) {
      Match( mnextToken.type, parsedResult );  // 消耗 '+' 或 '-'
      if ( !Maybe_mult_exp( parsedResult ) ) return false;
    } // end while

    return true;
  } // end Rest_of_maybe_additive_exp()

  bool Maybe_mult_exp( Token &parsedResult ) {
    // cout << mnextToken.tokenName << endl;
    if ( !Unary_exp( parsedResult ) ) return false;
    return Rest_of_maybe_mult_exp( parsedResult );  
  } // end Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp( Token &parsedResult ) {
    while ( mnextToken.type == MUL || mnextToken.type == DIV || mnextToken.type == MOD ) {
      Match( mnextToken.type, parsedResult );  
      if ( !Unary_exp( parsedResult ) ) return false;
    } // end while
    
    return true;  
  } // end Rest_of_maybe_mult_exp()

  bool Unary_exp( Token &parsedResult ) {
    if ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
      while ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
        Sign( parsedResult );
      } // end while
        
      return Signed_Unary_exp( parsedResult );
    } // end if
     
    else if ( mnextToken.type == IDENTIFIER || mnextToken.type == CONSTANT || mnextToken.type == LPAREN ) {
      return Unsigned_unary_exp( parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == PP || mnextToken.type == MM ) {
      Match( mnextToken.type, parsedResult ); // 消耗 PP 或 MM
      
      if ( Match( IDENTIFIER, parsedResult ) ) {
        if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() ) {
          parsedResult = mnextToken;
          parsedResult.type = ERROR;
          parsedResult.error = SEMANTICERROR;

          return false;
        }  // end if 
        
        if ( mnextToken.type == LBRACKET ) {
          Match( LBRACKET, parsedResult ); // 消耗 '['
          if ( !Expression( parsedResult ) ) return false; 
          if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
        } // end if

        return true;
      } // end if

      return false;
    } // end else if
    
    return false;
  } // end Unary_exp()

  bool Signed_Unary_exp( Token &parsedResult ) {
    if ( mnextToken.type == IDENTIFIER ) {
      if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;

        return false;
      }  // end if 
        
      Match( IDENTIFIER, parsedResult );
      if ( mnextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        
        if ( mnextToken.type != RPAREN ) {
          if ( !Actual_parameter_list( parsedResult ) ) return false;
        } // end if
         
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( mnextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !Expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
      } // end else if
    
      return true;
    } // end if 
    
    else if ( mnextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      if ( !Expression( parsedResult ) ) return false;
      
      return Match( RPAREN, parsedResult ); // 消耗 ')'
    } // end else if
    
    return false;
  }  // end Signed_Unary_exp()

  bool Unsigned_unary_exp( Token &parsedResult ) {
    if ( mnextToken.type == IDENTIFIER ) {
      if ( gsymbolTable.find( mnextToken.tokenName ) == gsymbolTable.end() ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SEMANTICERROR;

        return false;
      }  // end if 

      Match( IDENTIFIER, parsedResult );
      
      if ( mnextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult ); 
        if ( mnextToken.type != RPAREN ) {
          if ( !Actual_parameter_list( parsedResult ) ) return false;
        } // end if
 
        if ( !Match( RPAREN, parsedResult ) ) return false; // 消耗 ')'
      } // end if
      
      else if ( mnextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        
        if ( !Expression( parsedResult ) ) return false;
        if ( !Match( RBRACKET, parsedResult ) ) return false; // 消耗 ']'
        if ( mnextToken.type == PP || mnextToken.type == MM ) {
          Match( mnextToken.type, parsedResult );
        } // end if
      } // end else if
    
      return true;
    } // end if 
    
    else if ( mnextToken.type == CONSTANT ) {
      return Match( CONSTANT, parsedResult );
    } // end else if 
    
    else if ( mnextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult ); // 消耗 '('
      
      if ( !Expression( parsedResult ) ) return false;
      
      return Match( RPAREN, parsedResult ); // 消耗 ')'
    } // end else if
    
    return false;
  } // end Unsigned_unary_exp()

  bool Sign( Token &parsedResult ) {
    if ( mnextToken.type == PLUS || mnextToken.type == MINUS || mnextToken.type == NOT ) {
      return Match( mnextToken.type, parsedResult );
    } // end if
    
    return false;
  } // end Sign()

  bool Match( Type expected, Token &parsedResult ) {
    if ( mnextToken.type == expected ) {
      parsedResult.content.push_back( mnextToken.tokenName );
      mnextToken = gtokenizer.GetNextToken();
      return true;
    } // end if 
    
    else {
      if ( mnextToken.type != ERROR ) {
        parsedResult = mnextToken;
        parsedResult.type = ERROR;
        parsedResult.error = SYNTACTICALERROR;
      } // end if
        
      else parsedResult = mnextToken;
      mnextToken = gtokenizer.GetNextToken();
      return false;
    } // end else
  } // end Match()
 
};

#endif // PARSER_H

