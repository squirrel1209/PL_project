#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
//#include <stdexcept>

using namespace std;

enum class Type {
    RPAREN,    // ')'
    LPAREN,    // '('
    IDENT,
    NUM,
    SIGN,
    OTHER,
    QUIT
};

struct TokenWithType {
    string token;
    Type type;
};

class Parser {
private:
    vector<TokenWithType> tokens;
    int currentTokenIndex;

public:
    Parser( vector<TokenWithType> inputTokens ): tokens(inputTokens), currentTokenIndex(0) {}
	
    void parse() {  // 呼叫此function作為開始 
        command();
    } // end parse

    void command() {
        if ( currentTokenType() == Type::QUIT ) {
        	match( TokenType::QUIT );
        } // end if
        
        else if (  ) {
        	
        } // end else if
    } // end command()

    // <Statement> ::= IDENT ':=' <ArithExp>
    void statement() {
        if ( currentTokenType() == TokenType::IDENT ) {
            match( TokenType::IDENT );
            
            if ( currentTokenType() == TokenType::ASSIGN ) {
                match( TokenType::ASSIGN );
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

    // <ArithExp> ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
    void arithExp() {
        term();
        while ( currentTokenType() == TokenType::PLUS || currentTokenType() == TokenType::MINUS ) {
            match( currentTokenType() );
            term();
        } // end while
    } // end arithExp()

    // <Term> ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
    void term() {
        factor();
        while ( currentTokenType() == TokenType::MULTIPLY || currentTokenType() == TokenType::DIVIDE ) {
            match( currentTokenType() );
            factor();
        } // end while
    } // end term

    // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'	
    bool factor() {
        if ( currentTokenType() == Type::IDENT ) {
        	 match(TokenType::IDENT);
        	 return true ;
        } // end if
        
        else if ( currentTokenType() == TokenType::SIGN ) {
            match(TokenType::SIGN);
            
            if ( currentTokenType() == TokenType::NUM ) {
                match(TokenType::NUM);
                return true ;
            } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in factor.");
	      return false ;	
	  } // end else
        } // end else if
        
        else if ( currentTokenType() == TokenType::NUM ) {
            match(TokenType::NUM);
            return true ;
        } // end else if
        
        else if ( currentTokenType() == TokenType::LPAREN ) {
            match(TokenType::LPAREN);
            
            if ( arithExp() ) {
                match(TokenType::ARITHEXP);
                
                if ( currentTokenType() == TokenType::RPAREN ) {
                    match(TokenType::RPAREN);
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
            return Type::END;
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
