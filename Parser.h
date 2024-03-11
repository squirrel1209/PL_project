#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "Tokenizer.h"
#include <stdexcept>

using namespace std;

//template<typename T>
struct GrammerRule {
    Token  token;
    Type type;
};

class Parser {
private:
    vector<Token>& tokens;
    int currentTokenIndex;

public:
    Parser( vector<Token> inputTokens ): tokens(inputTokens), currentTokenIndex(0) {}
    
    Token evaluateOperation( Token a, Token b, Type op ) {
        Token answer ;
        float floatA = atof( a.tokenName.c_str() );
        float floatB = atof( b.tokenName.c_str() );
        
        if ( op == Type::MULTIPLY ) {
            float 
        } // end if
    } // end evaluateOperation()
    
    void parse() {  // 呼叫此function作為開始 
        GrammerRule rule ;
         
        if ( factor( rule ) ) cout << "Correct" ;
        else cout << "isNotCorrect" ;
         
        //command();
    } // end parse

    bool command() {
        if ( currentTokenType() == Type::QUIT ) {
            match( Type::QUIT );
            cout << "command" << endl ;
            return true ;
        } // end if

        else if ( currentTokenType() == Type::IDENT ) {
            match( Type::IDENT );
            
            if ( currentTokenType() == Type::ASSIGN ) {
                match( Type::ASSIGN );
                
	      if ( arithExp( 5 ) ) {
	          match( Type::ARITHEXP );
	          
	          if ( currentTokenType() == Type::SEMICOLON ) {
	              match( Type::SEMICOLON );
	              cout << "command" << endl ;
	              return true ;
		} // end if
		
		else {
	              throw runtime_error("Error: Unexpected token in command.");
                        return false ;	
		} // end else
	      }  // end if
	      
	      else {
                    throw runtime_error("Error: Unexpected token in command.");
                    return false ; 	
	      } // end else
	  } // end if
	  
	  else if ( IDlessArithExpOrBexp() ) {
                if ( currentTokenType() == Type::SEMICOLON ) {
	          match( Type::SEMICOLON );
	          cout << "command" << endl ;
	          return true ;
	      } // end if
		
	      else {
	          throw runtime_error("Error: Unexpected token in command.");
                    return false ;	
                } // end else
	  } // end else if
	  
	  else {
                throw runtime_error("Error: Unexpected token in command.");
                return false ;
	  } // end else
        } // end else if

        else if ( NOT_IDStartArithExpOrBexp() ) {
            if ( currentTokenType() == Type::SEMICOLON ) {
	      match( Type::SEMICOLON );
	      cout << "command" << endl ;
	      return true ;
	  } // end if
		
	  else {
	      throw runtime_error("Error: Unexpected token in command.");
                return false ;	
            } // end else
        } // end else if 

        else {
            throw runtime_error("Error: Unexpected token in command.");
            return false ;
        } // end else
    } // end command()

    bool NOT_IDStartArithExpOrBexp() {
        if ( NOT_ID_StartArithExp() ) {
            if ( BooleanOperator() ) {
                match( currentTokenType() );
                if ( arithExp(5) ) {
                    match( Type::ARITHEXP );
                    cout << "NOT_IDStartArithExpOrBexp" << endl ;
	          return true ; 
	      } // end if
	  
    	      else {
                    throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp.");
                    return false ;	
	      } // end else
            } // end if
            
            else return true ;
        } // end if
        
        else {
            throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp.");
            return false ;
        } // end else
    } // end NOT_IDStartArithExpOrBexp()

    bool NOT_ID_StartArithExp() {
        if ( NOT_ID_StartTerm() ) {
        	  match( Type::TERM );
            while ( currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS ) {
                match( currentTokenType() );
                
                if ( !term(5) ) {
                    throw runtime_error("Error: Unexpected token in arithExp.");
                    return false ;
	      } // end if
	      
	      else match( Type::TERM );
            } // end while
            
            cout << "arithExp" << endl ;
            return true ;
        } // end if
        
        else {
            throw runtime_error("Error: Unexpected token in arithExp.");
            return false ;
        } // end else
    } // end NOT_IDStartArithExpOrBexp()
    
    bool NOT_ID_StartTerm() {
        if ( NOT_ID_StartFactor() ) {
            match(Type::FACTOR);
            while ( currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE ) {
                match( currentTokenType() );
                
                if ( !factor(5) ) {
                    throw runtime_error("Error: Unexpected token in term.");
                    return false ;
	      } // end if
	      
	      else match(Type::FACTOR);
            } // end while
            
            cout << "term" << endl ;
            return true ;
        } // end if
        
        else {
        	  throw runtime_error("Error: Unexpected token in term.");
            return false ;
        } // end else

    } // end NOT_ID_StartTerm()
    
    bool NOT_ID_StartFactor() {        
        if ( currentTokenType() == Type::SIGN ) {
            match(Type::SIGN);
            
            if ( currentTokenType() == Type::NUM ) {
                match(Type::NUM);
                cout << "NOT_ID_StartFactor" << endl ;
                return true ;
            } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in factor.");
	      return false ;	
	  } // end else
        } // end if
        
        else if ( currentTokenType() == Type::NUM ) {
            match(Type::NUM);
            cout << "NOT_ID_StartFactor" << endl ;
            return true ;
        } // end else if
        
        else if ( currentTokenType() == Type::LPAREN ) {
            match(Type::LPAREN);
            
            if ( arithExp(5) ) {
                match(Type::ARITHEXP);
                
                if ( currentTokenType() == Type::RPAREN ) {
                    match(Type::RPAREN);
                    cout << "NOT_ID_StartFactor" << endl ;
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
    } // end NOT_ID_StartFactor()
    
    bool IDlessArithExpOrBexp() {
        while ( currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS ||
                currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE ) {
                	
            if ( currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS ) {
                match( currentTokenType() );
                if ( term(5) ) match(Type::TERM);
	      else {
	          throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp.");
	          return false ;
	      } // end else
	  } // end if
            
            else if ( currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE ) {
                match( currentTokenType() );
                if ( factor(5) ) match(Type::FACTOR);
	      else {
	          throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp.");
	          return false ;
	      } // end else
	  } // end else if
        } // end while
      
        if ( BooleanOperator() ) {
            match( currentTokenType() );
            if ( arithExp(5) ) {
                match( Type::ARITHEXP );
                cout << "IDlessArithExpOrBexp" << endl ;
	      return true ; 
	  } // end if
	  
	  else {
                throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp.");
                return false ;	
	  } // end else
        } // end if
        return true ;
    } // end IDlessArithExpOrBexp

    bool BooleanOperator() {
        if ( currentTokenType() == Type::EQUAL || currentTokenType() == Type::NOTEQUAL || currentTokenType() == Type::GREATER ||
             currentTokenType() == Type::LESS || currentTokenType() == Type::GREATEREQUAL || currentTokenType() == Type::LESSEQUAL ) 
	   return true ;
        else return false ;	
    } // end BooleanOperator()

    // <Statement> ::= IDENT ':=' <ArithExp>
    bool statement() {
        if ( currentTokenType() == Type::IDENT ) {
            match( Type::IDENT );
            
            if ( currentTokenType() == Type::ASSIGN ) {
                match( Type::ASSIGN );
                if ( arithExp(5) ) {
                	match( Type::ARITHEXP );
                	cout << "statement" << endl ;
                    return true ;    
	      } // end if
	      
                else {
                    throw runtime_error("Error: Unexpected token in statement.");
                    return false ;
	      } // end else
            } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in statement.");
                return false ;
	  } // end else 
        } // end if
        
        else {
            throw runtime_error("Error: Unexpected token in statement.");
	  return false ;   
        } // end else
    } // end statement()

    // <BooleanExp> ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
    bool booleanExp() {
        if ( arithExp(5) ) {
        	  match( Type::ARITHEXP );
        	  
            if ( BooleanOperator() ) {
                match( currentTokenType() );
                
                if ( arithExp(5) ) {
                	match( Type::ARITHEXP );
                	cout << "booleanExp" << endl ;
                    return true ;
	      } // end if
	      
                else {
                	throw runtime_error("Error: Expected comparison operator.");
                	return false ;
	      } // end else
            } // end if
            
            else {
                throw runtime_error("Error: Expected comparison operator.");
                return false ;	
	  } // end else
        } // end if
        
        else {
            throw runtime_error("Error: Expected comparison operator.");
            return false ;
        } // end else
    } // end booleanExp()

    // <ArithExp> ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
    bool arithExp( int value ) {
        if ( term(5) ) {
        	  match( Type::TERM );
            while ( currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS ) {
                match( currentTokenType() );
                
                if ( !term(5) ) {
                    throw runtime_error("Error: Unexpected token in arithExp.");
                    return false ;
	      } // end if
	      
	      else match( Type::TERM );
            } // end while
            
            cout << "arithExp" << endl ;
            return true ;
        } // end if
        
        else {
            throw runtime_error("Error: Unexpected token in arithExp.");
            return false ;
        } // end else
    } // end arithExp()

    // <Term> ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
    bool term( GrammerRule & rule ) {
        if ( factor( rule ) ) {
            match(Type::FACTOR);
            while ( currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE ) {
                match( currentTokenType() );
                
                if ( !factor(5) ) {
                    throw runtime_error("Error: Unexpected token in term.");
                    return false ;
	      } // end if
	      
	      else match(Type::FACTOR);
            } // end while
            
            cout << "term" << endl ;
            return true ;
        } // end if
        
        else {
        	  throw runtime_error("Error: Unexpected token in term.");
            return false ;
        } // end else
    } // end term

    // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'  	
    bool factor( GrammerRule & rule ) {
        
        if ( currentTokenType() == Type::ERROR ) {
            return false ;
        } // end if
    	
        if ( currentTokenType() == Type::IDENT ) {
        	 match(Type::IDENT);
        	 rule.token = currentToken() ;
        	 rule.type = Type::FACTOR ;
        	 return true ;
        } // end if
        
        else if ( currentTokenType() == Type::SIGN ) {
        	  rule.token = currentToken() ;
            match(Type::SIGN);
            
            if ( currentTokenType() == Type::INT || currentTokenType() == Type::FLOAT ) {
                rule.token.tokenName = rule.token.tokenName + currentToken().tokenName ;
                rule.type = Type::FACTOR ;
                match(currentTokenType());
                return true ;
            } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in factor.");
	      return false ;	
	  } // end else
        } // end else if
        
        else if ( currentTokenType() == Type::INT || currentTokenType() == Type::FLOAT ) {
        	  rule.token = currentToken() ;
        	  rule.type = Type::FACTOR ;
            match(currentTokenType());
            return true ;
        } // end else if
        
        else if ( currentTokenType() == Type::LPAREN ) {
            match(Type::LPAREN);
            
            if ( arithExp(rule) ) {
                match(Type::ARITHEXP);
                
                if ( currentTokenType() == Type::RPAREN ) {
                	
                    match(Type::RPAREN);

                    rule.type = Type::FACTOR ;
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
            return Type::QUIT;
        } // end else
    } // end currentTokenType
    
    Token currentToken() {
        Token none;
        if ( currentTokenIndex < tokens.size() ) {
            return tokens[currentTokenIndex];
        } // end if
         
        else return none;
    } // end currentTokenValue()
    
};

#endif // PARSER_H
