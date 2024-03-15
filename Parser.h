#ifndef PARSER_H
#define PARSER_H

#include <cstdlib> // �� <stdlib.h> �p�G�z�ϥΪ��O��C����
#include <vector>
#include <string>
#include <map>
#include "Tokenizer.h"
#include <stdexcept>

using namespace std;
map<string, Token> symbolTable;

class Parser {
public:
    Parser( vector<Token> inputTokens ): tokens(inputTokens), currentTokenIndex(0) {}
    
    void parse() {
        Token parsedResult ;
        command( parsedResult ) ;
        
        while ( parsedResult.type != QUIT ) {
        
            if ( parsedResult.type == ERROR ) {
                if ( parsedResult.error.type == lexicalError ) 
                    cout << "unrecognized token with first char : '" << parsedResult.error.errorValue << "'" << endl;
	  
	      else if ( parsedResult.error.type == syntacticalError ) 
	          cout << "unexpected token : '" << parsedResult.error.errorValue << "'" << endl;

	      else if ( parsedResult.error.type == semanticError ) 
	          cout << "undefined identifier : '" << parsedResult.error.errorValue << "'" << endl;

	      else cout << "unknowed Error" << endl ;
            } // end if
        
            else cout << "correct:" << parsedResult.tokenName << endl;
            
            Error err;
            parsedResult = createToken( "", NONE, err ) ;
            command( parsedResult ) ;
        } // end while
    } // end parse

private:
    vector<Token> tokens;
    int currentTokenIndex;
    
    Token evaluateOperation( Token a, Token b, Type op ) {
        Token answer ;
        float floatA = atof( a.tokenName.c_str() );
        float floatB = atof( b.tokenName.c_str() );
        float floatAnswer ;
         
        if ( op == MULTIPLY ) {
            floatAnswer = floatA * floatB;
        } // end if
                    
        else if ( op == DIVIDE ) {
        	  floatAnswer = floatA / floatB;
        } // end else if
        
        else if ( op == PLUS ) {
        	  floatAnswer = floatA + floatB;
        } // end else if
        
        else if ( op == MINUS ) {
        	  floatAnswer = floatA - floatB;
        } // end else if
        
        if ( a.type == INT && b.type == INT ) {
            int intAnswer = (int)floatAnswer; 
            answer.tokenName = anyToString( intAnswer ) ;
            answer.type = INT;
            return answer;
        } // end if
    
        else {
            answer.tokenName = anyToString( floatAnswer ) ;
            answer.type = FLOAT; // Assuming FLOAT as the type for floating point numbers
            return answer;
        } // end else
    } // end evaluateOperation()
    
    Token compareOperation(const Token &left, const Token &right, Type op) {
        Token result;
        result.type = BOOL;  // ���] Token ���@�� BOOL �����Ω��ܥ��L��

        // �N�r�����ഫ���ƭȶi����
        double leftVal = atof(left.tokenName.c_str());
        double rightVal = atof(right.tokenName.c_str());
        bool comparisonResult;

        // �ھھާ@�Ŷi�����A�����b case ����^���G�A�קK�ϥ� break
        switch (op) {
            case EQUAL:
                comparisonResult = leftVal == rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case NOTEQUAL:
                comparisonResult = leftVal != rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case GREATER:
                comparisonResult = leftVal > rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case LESS:
                comparisonResult = leftVal < rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case GREATEREQUAL:
                comparisonResult = leftVal >= rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case LESSEQUAL:
                comparisonResult = leftVal <= rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
            default:
                throw runtime_error("Unsupported comparison operator.");
        }
    } //

    void command( Token &parsedResult ) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == QUIT || currentToken.type == ERROR ) {
            parsedResult = currentToken;
            match();
        } // end if

        else if ( currentToken.type == IDENT ) {
            Token ident = currentToken;
            match();  // ���ʨ�U�@�� token�A�Y ASSIGN
            
            if (currentTokenType() == ASSIGN) {
                match();  // ���ʨ��Ȫ�F�� 
                
	      if ( arithExp( parsedResult ) ) {
                    // �N��N��F�������G��ȵ����Ѳ�
                    symbolTable[ident.tokenName] = parsedResult ;
	          
	          if ( currentTokenType() == SEMICOLON ) {
	              match();  // ���ӱ�����
	              return ;
		} // end if
		
		else {
		    parsedResult.type = ERROR;
                        parsedResult.error.type = syntacticalError;
                        parsedResult.error.errorValue = getCurrentToken().tokenName;
	              throw runtime_error("Error: Missing ';' after assignment command.");
                        return ;	
		} // end else
	      }  // end if
	      
	      else {
                    throw runtime_error("Error: Invalid arithmetic expression in assignment.");
                    return; 	
	      } // end else
	  } // end if
	  
	  else if ( IDlessArithExpOrBexp( ident, parsedResult) ) {
	      
                if ( currentTokenType() == SEMICOLON ) {
	          match();
	          return;
	      } // end if
		
	      else {
	          parsedResult.type = ERROR;
                    parsedResult.error.type = syntacticalError;
                    parsedResult.error.errorValue = getCurrentToken().tokenName;
	          throw runtime_error("Error: Missing ';' after IDlessArithExpOrBexp.");
                    return;	
                } // end else
	  } // end else if
	  
	  else {
	      parsedResult.type = ERROR;
                parsedResult.error.type = syntacticalError;
                parsedResult.error.errorValue = parsedResult.tokenName;
                throw runtime_error("Error: Unexpected token in statement.");
                return ;
	  } // end else
        } // end else if

        else if ( NOT_IDStartArithExpOrBexp(parsedResult) ) {
            if ( currentTokenType() == SEMICOLON ) {
	      match();
	      return;
	  } // end if
		
	  else {
	      parsedResult.type = ERROR;
                parsedResult.error.type = syntacticalError;
                parsedResult.error.errorValue = getCurrentToken().tokenName;
	      throw runtime_error("Error: Missing ';' after NOT_IDStartArithExpOrBexp.");
                return;	
            } // end else
        } // end else if 

        else {
        	  parsedResult.type = ERROR;
            parsedResult.error.type = syntacticalError;
            parsedResult.error.errorValue = getCurrentToken().tokenName;
            throw runtime_error("Error: Unexpected token in command.");
            return;
        } // end else
        
    } // end command()

    bool NOT_IDStartArithExpOrBexp(Token &parsedResult) {
        if (NOT_ID_StartArithExp(parsedResult)) {
            // �ѪR���\��A�ˬd�O�_���H�F�����B���
            
            if (BooleanOperator()) {
                Token operatorToken = getCurrentToken();
                match(); // ���ӥ����B���

                Token rightOperand;
                if (arithExp(rightOperand)) {
                    // �i�楬���B��
                    // �o�̰��] compareOperation ����ھ� operatorToken �M rightOperand �i�楬���B��
                    // �ñN���G�s�x�b parsedResult ��
                    parsedResult = compareOperation(parsedResult, rightOperand, operatorToken.type);
                    return true;
                } // end if
	       
	      else {
                    throw runtime_error("Error: Invalid right-hand side expression in NOT_IDStartArithExpOrBexp.");
                    return false;
                } // end else
            } // end if
            
            else {
                // �p�G�S�����H�����B��šA�h��^ true
                return true;
            } // end else
        }  // end if   
        
        else {
            throw runtime_error("Error: Unexpected token in NOT_IDStartArithExpOrBexp.");
            return false;
        } // end else
    } // end NOT_IDStartArithExpOrBexp

    bool NOT_ID_StartArithExp(Token &parsedResult) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
        
        // �ѪR�Ĥ@�� term
        if (!NOT_ID_StartTerm(parsedResult)) {
            throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
            return false;
        } // end if

        // �O�s��e�ѪR�쪺 token�A�i��Ω���򪺥[�k�δ�k�B��
        Token a = parsedResult;
        // ���e token �O�[���δ�ɡA�i��B��
        while (currentTokenType() == SIGN ) {
            // �O�s�B���
            Type op;
            if ( getCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
            else op = PLUS ;
            match();  // ���ʨ�U�@�� token

            // �ѪR�U�@�� term
            Token b;
            
            if (!term(b)) {
                throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
                return false;
            } // end if
            // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
            a = evaluateOperation(a, b, op);

            // ��s parsedResult ���ثe�����G
            parsedResult = a;
        } // end while 

        // ��^ true ��ܦ��\�ѪR arithExp
        return true;
    } // end NOT_IDStartArithExpOrBexp()
    
    bool NOT_ID_StartTerm(Token &parsedResult) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
    	
        // �����ѪR�@�� factor
        if (!NOT_ID_StartFactor(parsedResult)) {
            throw runtime_error("Error: Unexpected token in term, expecting a factor.");
            return false;
        } // end if

        // �O�s��e�ѪR�쪺 token�A�i��Ω���򪺭��k�ΰ��k�B��
        Token a = parsedResult;

        // ���e token �O���k�ΰ��k�B��ŮɡA�i��B��
        while (currentTokenType() == MULTIPLY || currentTokenType() == DIVIDE) {
            // �O�s�B���
            Type op = currentTokenType();
            match();  // ���ʨ�U�@�� token

            // �ѪR�U�@�� factor
            Token b;
            if (!factor(b)) {
                parsedResult = b ;
                throw runtime_error("Error: Unexpected token in term, expecting a factor.");
                return false;
            } // end if

            // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
            a = evaluateOperation(a, b, op);

            // ��s rule ���ثe�����G
            parsedResult = a;
        } // end while

        // ��^ true ��ܦ��\�ѪR term
        return true;
    } // end NOT_ID_StartTerm()
    
    bool NOT_ID_StartFactor(Token &parsedResult) {        
        Token currentToken = getCurrentToken();
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
        	  parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if

        else if ( currentToken.type == SIGN ) {
        	  if ( currentToken.tokenName.compare( "-" ) == 0 )
        	      parsedResult = currentToken ;

            match();
            currentToken = getCurrentToken(); // ��s Token ����
            
            if ( currentToken.type == INT || currentToken.type == FLOAT ) {
                parsedResult.tokenName += currentToken.tokenName;
                parsedResult.type = currentToken.type ;
                match();
                return true ;
            } // end if
            
            else {
               parsedResult.type = ERROR;
               parsedResult.error.type = syntacticalError;
               parsedResult.error.errorValue = currentToken.tokenName;
               match();
	     return false ;	
	  } // end else
        } // end else if
        
        else if ( currentToken.type == INT || currentToken.type == FLOAT ) {
        	  parsedResult = currentToken ;
            match();
            return true ;
        } // end else if
        
        else if ( currentToken.type == LPAREN ) {
            match();
            
            if ( arithExp(parsedResult) ) { 
	      currentToken = getCurrentToken();
                
                if ( currentToken.type == RPAREN ) {
                    match();
                    return true ;
	      } // end if
	      
	      else {
	          parsedResult.type = ERROR;
                    parsedResult.error.type = syntacticalError;
                    parsedResult.error.errorValue = currentToken.tokenName;
                    match();
	      	return false ;
	      } // end else
            } // end if 
            
            else {
                throw runtime_error("Error: arithExp�AUnexpected token in factor. ");
                match();
                return false ;
	  } // end else 
        } // end else if 
        
        else {
	  parsedResult.type = ERROR;
            parsedResult.error.type = syntacticalError;
            parsedResult.error.errorValue = currentToken.tokenName;
            match();
            return false ;
        } // end else
    } // end NOT_ID_StartFactor()
    
    // <IDlessArithExpOrBexp>  ::= {   '+' <Term>   | '-' <Term> | '*' <Factor> | '/' <Factor> }
    //                             [ <BooleanOperator> <ArithExp> ]
    bool IDlessArithExpOrBexp( Token ident, Token &parsedResult ) {
        if ( symbolTable.find( ident.tokenName ) != symbolTable.end() ) {   // IDENT���w�q 
            ident = symbolTable[ident.tokenName];
        } // end if
        
        else {                                                              // IDENT�L�w�q
            parsedResult.type = ERROR;
            parsedResult.error.type = semanticError;
            parsedResult.error.errorValue = ident.tokenName;
            return false;
        } // end else
        
        while ( currentTokenType() == SIGN || currentTokenType() == MULTIPLY || currentTokenType() == DIVIDE ) {
            Type op;
            if ( getCurrentToken().tokenName.compare( "+" ) == 0 ) op = PLUS;
            else if ( getCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
            else op = currentTokenType();
            
            match();  // ���ʨ�U�@�� token�A�Y Term �� Factor ���}�l
            

            if (op == PLUS || op == MINUS) {
                if (!term(parsedResult)) {
                    throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp after PLUS or MINUS.");
                    return false;
                } // end if
                
                parsedResult = evaluateOperation(ident, parsedResult, op); // ����B��
            } // end if
            
            else if (op == MULTIPLY || op == DIVIDE) {
                if (!factor(parsedResult)) {
                    throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp after MULTIPLY or DIVIDE.");
                    return false;
                } // end if
                
                parsedResult = evaluateOperation(ident, parsedResult, op); // ����B��
            } // end else if
            
            ident = parsedResult ;
        } // end while
        
        if (BooleanOperator()) {
        	  Type op = currentTokenType();
            match();  // ���ʨ� Boolean ��F�����}�l
            
            Token nextOperand;
            if (!arithExp(nextOperand)) {
                throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp Boolean expression.");
                return false;
            } // end if
            
            parsedResult = compareOperation(parsedResult, nextOperand, op); // ���楬�L�B��
        } // end if
    
        return true;
    } // end IDlessArithExpOrBexp

    bool BooleanOperator() {
        if ( currentTokenType() == EQUAL || currentTokenType() == NOTEQUAL || currentTokenType() == GREATER ||
             currentTokenType() == LESS || currentTokenType() == GREATEREQUAL || currentTokenType() == LESSEQUAL ) 
	   return true ;
        else return false ;	
    } // end BooleanOperator()

    // <BooleanExp> ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
    bool booleanExp( Token &parsedResult ) {
        if ( arithExp(parsedResult) ) {
        	  //cout << parsedResult.tokenName;
        	  cout << currentTokenType();
            if ( BooleanOperator() ) {
                Type op = currentTokenType();  // �������B���
                match();
                
                Token rightExpr;
                if (arithExp(rightExpr)) {
                    // �����Ӻ�N��F�������G
                    // ���B���Ӧ�����޿誺��{�A�o�̶ȥܽd���c
                    parsedResult = compareOperation(parsedResult, rightExpr, op);
                    return true;
	      } // end if
	      
                else {
                	throw runtime_error("Error: Invalid right-hand side expression in booleanExp.");
                	return false ;
	      } // end else
            } // end if
            
            else {
                parsedResult.type = ERROR;
                parsedResult.error.type = syntacticalError;
                parsedResult.error.errorValue = getCurrentToken().tokenName;
                throw runtime_error("Error: Expected comparison operator in booleanExp.");
                return false ;	
	  } // end else
        } // end if
        
        else {
            throw runtime_error("Error: Invalid left-hand side expression in booleanExp.");
            return false ;
        } // end else
    } // end booleanExp()

    // <ArithExp> ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
    bool arithExp(Token &parsedResult) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
        
        // �ѪR�Ĥ@�� term
        if (!term(parsedResult)) {
            throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
            return false;
        } // end if

        // �O�s��e�ѪR�쪺 token�A�i��Ω���򪺥[�k�δ�k�B��
        Token a = parsedResult;
        // ���e token �O�[���δ�ɡA�i��B��
        while (currentTokenType() == SIGN ) {
            // �O�s�B���
            Type op;
            if ( getCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
            else op = PLUS ;
            match();  // ���ʨ�U�@�� token

            // �ѪR�U�@�� term
            Token b;
            
            if (!term(b)) {
                throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
                return false;
            } // end if
            // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
            a = evaluateOperation(a, b, op);

            // ��s parsedResult ���ثe�����G
            parsedResult = a;
        } // end while 

        // ��^ true ��ܦ��\�ѪR arithExp
        return true;
    } // end arithExp()

    // <Term> ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
    bool term(Token &parsedResult) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
    	
        // �����ѪR�@�� factor
        if (!factor(parsedResult)) {
            throw runtime_error("Error: Unexpected token in term, expecting a factor.");
            return false;
        } // end if

        // �O�s��e�ѪR�쪺 token�A�i��Ω���򪺭��k�ΰ��k�B��
        Token a = parsedResult;

        // ���e token �O���k�ΰ��k�B��ŮɡA�i��B��
        while (currentTokenType() == MULTIPLY || currentTokenType() == DIVIDE) {
            // �O�s�B���
            Type op = currentTokenType();
            match();  // ���ʨ�U�@�� token

            // �ѪR�U�@�� factor
            Token b;
            if (!factor(b)) {
                parsedResult = b ;
                throw runtime_error("Error: Unexpected token in term, expecting a factor.");
                return false;
            } // end if

            // �ھڹB��Ŷi��������B��A�ç�s a ���B�⵲�G
            a = evaluateOperation(a, b, op);

            // ��s rule ���ثe�����G
            parsedResult = a;
        } // end while

        // ��^ true ��ܦ��\�ѪR term
        return true;
    } // end term()
    
    // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'  	
    bool factor( Token &parsedResult ) {
        Token currentToken = getCurrentToken();
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
        	  parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if

        if ( currentToken.type == IDENT ) {
        	 if ( symbolTable.find(currentToken.tokenName) != symbolTable.end() ) {
        	     parsedResult = symbolTable[currentToken.tokenName];
        	     match();  // factor
        	     return true ;
	 } // end if

           else {
               //undefined identifier
               parsedResult.type = ERROR;
               parsedResult.error.type = semanticError;
               parsedResult.error.errorValue = currentToken.tokenName;
               match();
               return false;
	 } // end else
        } // end if

        else if ( currentToken.type == SIGN ) {
        	  if ( currentToken.tokenName.compare( "-" ) == 0 )
        	      parsedResult = currentToken ;

            match();
            currentToken = getCurrentToken(); // ��s Token ����
            
            if ( currentToken.type == INT || currentToken.type == FLOAT ) {
                parsedResult.tokenName += currentToken.tokenName;
                parsedResult.type = currentToken.type ;
                match();
                return true ;
            } // end if
            
            else {
               parsedResult.type = ERROR;
               parsedResult.error.type = syntacticalError;
               parsedResult.error.errorValue = currentToken.tokenName;
               match();
	     return false ;	
	  } // end else
        } // end else if
        
        else if ( currentToken.type == INT || currentToken.type == FLOAT ) {
        	  parsedResult = currentToken ;
            match();
            return true ;
        } // end else if
        
        else if ( currentToken.type == LPAREN ) {
            match();
            
            if ( arithExp(parsedResult) ) { 
	      currentToken = getCurrentToken();
                
                if ( currentToken.type == RPAREN ) {
                    match();
                    return true ;
	      } // end if
	      
	      else {
	          parsedResult.type = ERROR;
                    parsedResult.error.type = syntacticalError;
                    parsedResult.error.errorValue = currentToken.tokenName;
                    match();
	      	return false ;
	      } // end else
            } // end if 
            
            else {
                throw runtime_error("Error: arithExp�AUnexpected token in factor. ");
                match();
                return false ;
	  } // end else 
        } // end else if 
        
        else {
	  parsedResult.type = ERROR;
            parsedResult.error.type = syntacticalError;
            parsedResult.error.errorValue = currentToken.tokenName;
            match();
            return false ;
        } // end else
    } // end factor()
    
    // �p�G��etoken�ŦX��k�A�h���Ჾ�@�� 
    void match() {
        if ( currentTokenIndex < tokens.size() ) {
            currentTokenIndex++;
        } // end if

        else return  ;
    } // end match()
    
    // �����etoken������ 
    Type currentTokenType() {
        if ( currentTokenIndex < tokens.size() ) {
            return tokens[currentTokenIndex].type;
        } // end if
         
        else {
            return QUIT;
        } // end else
    } // end currentTokenType
    
    Token getCurrentToken() {
        Token quit;
        quit.type = QUIT;

        if ( currentTokenIndex < tokens.size() ) {
            return tokens[currentTokenIndex];
        } // end if
         
        else {
        	return quit;
        }
        
    } // end currentTokenValue()
    
};

#endif // PARSER_H
