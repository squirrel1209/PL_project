#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "Tokenizer.h"
#include <stdexcept>

using namespace std;


class Parser {
private:
    vector<TokenWithType> tokens;
    int currentTokenIndex;

public:
    Parser( vector<TokenWithType> inputTokens ): tokens(inputTokens), currentTokenIndex(0) {}
	
    void parse() {  // 呼叫此function作為開始 
        if ( factor() ) cout << "isfactor" ;
        else cout << "isNotFactor" ;
        //command();
    } // end parse

    void command() {
        if ( currentTokenType() == Type::QUIT ) {
        	match( Type::QUIT );
        } // end if
        
        /*else if (  ) {
        	
        } // end else if*/ 
    } // end command()

    // <Statement> ::= IDENT ':=' <ArithExp>
    /*
    void statement() {
        if ( currentTokenType() == Type::IDENT ) {
            match( Type::IDENT );
            
            if ( currentTokenType() == Type::ASSIGN ) {
                match( Type::ASSIGN );
                arithExp();
            } // end if
            
            else throw runtime_error("Error: Unexpected token in statement.");
        } // end if
        
        else throw runtime_error("Error: Unexpected token in statement.");
    } // end statement()

    // <BooleanExp> ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
    void booleanExp() {
        arithExp();
        if ( currentTokenType() == TokenType::EQUAL || currentTokenType() == TokenType::NOTEQUAL || currentTokenType() == TokenType::GREATER ||
             currentTokenType() == TokenType::LESS || currentTokenType() == TokenType::GREATEREQUAL || currentTokenType() == TokenType::LESSEQUAL ) {
            match( currentTokenType() );
        } // end if
         
        else {
            throw runtime_error("Error: Expected comparison operator.");
        } // end else
        
        arithExp();
    } // end booleanExp()
*/
    // <ArithExp> ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
    bool arithExp() {
        if ( term() ) {
            while ( currentTokenType() == TokenType::PLUS || currentTokenType() == TokenType::MINUS ) {
                match( currentTokenType() );
                
                if ( !term() ) {
                    throw runtime_error("Error: Unexpected token in arithExp.");
                    return false ;
	      } // end if
            } // end while
            
            return true ;
        } // end if
        
        else {
            throw runtime_error("Error: Unexpected token in arithExp.");
            return false ;
        } // end else
    } // end arithExp()

    // <Term> ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
    bool term() {
        if ( factor() ) {
            while ( currentTokenType() == TokenType::MULTIPLY || currentTokenType() == TokenType::DIVIDE ) {
                match( currentTokenType() );
                
                if ( !factor() ) {
                    throw runtime_error("Error: Unexpected token in term.");
                    return false ;
	      } // end if
            } // end while
            
            return true ;
        } // end if
        
        else {
        	  throw runtime_error("Error: Unexpected token in term.");
            return false ;
        } // end else

    } // end term

    // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'	
    bool factor() {
        if ( currentTokenType() == Type::IDENT ) {
        	 match(Type::IDENT);
        	 return true ;
        } // end if
        
        else if ( currentTokenType() == Type::SIGN ) {
            match(Type::SIGN);
            
            if ( currentTokenType() == Type::NUM ) {
                match(Type::NUM);
                return true ;
            } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in factor.");
	      return false ;	
	  } // end else
        } // end else if
        
        else if ( currentTokenType() == Type::NUM ) {
            match(Type::NUM);
            return true ;
        } // end else if
        /*
        else if ( currentTokenType() == Type::LPAREN ) {
            match(Type::LPAREN);
            
            if ( arithExp() ) {
                match(Type::ARITHEXP);
                
                if ( currentTokenType() == Type::RPAREN ) {
                    match(Type::RPAREN);
                    return true ;
	      } // end if
	      
	      else {
	          throw runtime_error("Error: Unexpected token in factor.");	
	      	return false ;
	      } // end else
	  } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in factor.");
                return false ;
	  } // end else 
        } // end else if 
        */
        else {
            throw runtime_error("Error: Unexpected token in factor.");
            return false ;
        } // end else
    } // end factor()
    
    // 如果當前token符合文法，則往後移一位 
    void match( Type expected ) {
        if ( currentTokenType() == expected ) {
            currentTokenIndex++;
        } // end if 
        
        else throw runtime_error("Error: Unexpected token.");
    } // end match()
    
    // 獲取當前token的類型 
    Type currentTokenType() {
        if ( currentTokenIndex < tokens.size() ) {
            return tokens[currentTokenIndex].type;
        } // end if
         
        else {
            return Type::QUIT;
        } // end else
    } // end currentTokenType
    
    string currentTokenValue() {
        if ( currentTokenIndex < tokens.size() ) {
            return tokens[currentTokenIndex].token;
        } // end if
         
        else return "";
    } // end currentTokenValue()
};

#endif // PARSER_H
