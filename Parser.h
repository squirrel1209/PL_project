#ifndef PARSER_H
#define PARSER_H

#include <cstdlib> // 或 <stdlib.h> 如果您使用的是純C環境
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
            
            while ( parsedResult.type != SEMICOLON ) {
                match();
                parsedResult = getCurrentToken();
                if ( parsedResult.type == QUIT ) return ;
	  } // end while
	  match();
	  
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
        result.type = BOOL;  // 假設 Token 有一個 BOOL 類型用於表示布林值

        // 將字面值轉換為數值進行比較
        double leftVal = atof(left.tokenName.c_str());
        double rightVal = atof(right.tokenName.c_str());
        bool comparisonResult;

        // 根據操作符進行比較，直接在 case 中返回結果，避免使用 break
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
            match();  // 移動到下一個 token，即 ASSIGN
            
            if (currentTokenType() == ASSIGN) {
                match();  // 移動到賦值表達式 
                
	      if ( arithExp( parsedResult ) ) {
                    // 將算術表達式的結果賦值給標識符
                    symbolTable[ident.tokenName] = parsedResult ;
	          
	          if ( currentTokenType() == SEMICOLON ) {
	              match();  // 消耗掉分號
	              return ;
		} // end if
		
		else {
		    parsedResult.type = ERROR;
                        parsedResult.error.type = syntacticalError;
                        parsedResult.error.errorValue = getCurrentToken().tokenName;
                        return ;	
		} // end else
	      }  // end if
	      
	      else {
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
                    return;	
                } // end else
	  } // end else if
	  
	  else {
	      parsedResult.type = ERROR;
                parsedResult.error.type = syntacticalError;
                parsedResult.error.errorValue = parsedResult.tokenName;
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
                return;	
            } // end else
        } // end else if 

        
    } // end command()

    bool NOT_IDStartArithExpOrBexp(Token &parsedResult) {
        if (NOT_ID_StartArithExp(parsedResult)) {
            // 解析成功後，檢查是否跟隨了布爾運算符
            
            if (BooleanOperator()) {
                Token operatorToken = getCurrentToken();
                match(); // 消耗布爾運算符

                Token rightOperand;
                if (arithExp(rightOperand)) {
                    // 進行布爾運算
                    // 這裡假設 compareOperation 能夠根據 operatorToken 和 rightOperand 進行布爾運算
                    // 並將結果存儲在 parsedResult 中
                    parsedResult = compareOperation(parsedResult, rightOperand, operatorToken.type);
                    return true;
                } // end if
	       
	      else {
                    return false;
                } // end else
            } // end if
            
            else {
                // 如果沒有跟隨布爾運算符，則返回 true
                return true;
            } // end else
        }  // end if   
        
        else {
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
        
        // 解析第一個 term
        if (!NOT_ID_StartTerm(parsedResult)) {
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的加法或減法運算
        Token a = parsedResult;
        // 當當前 token 是加號或減號時，進行運算
        while (currentTokenType() == SIGN ) {
            // 保存運算符
            Type op;
            if ( getCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
            else op = PLUS ;
            match();  // 移動到下一個 token

            // 解析下一個 term
            Token b;
            
            if (!term(b)) {
                parsedResult = b; 
                return false;
            } // end if
            
            // 根據運算符進行相應的運算，並更新 a 為運算結果
            parsedResult = evaluateOperation(a, b, op);
        } // end while 

        // 返回 true 表示成功解析 arithExp
        return true;
    } // end NOT_IDStartArithExpOrBexp()
    
    bool NOT_ID_StartTerm(Token &parsedResult) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == ERROR || currentToken.type == QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
    	
        // 首先解析一個 factor
        if (!NOT_ID_StartFactor(parsedResult)) {
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的乘法或除法運算
        Token a = parsedResult;

        // 當當前 token 是乘法或除法運算符時，進行運算
        while (currentTokenType() == MULTIPLY || currentTokenType() == DIVIDE) {
            // 保存運算符
            Type op = currentTokenType();
            match();  // 移動到下一個 token

            // 解析下一個 factor
            Token b;
            if (!factor(b)) {
                parsedResult = b ;
                return false;
            } // end if

            // 根據運算符進行相應的運算，並更新 a 為運算結果
            parsedResult = evaluateOperation(a, b, op);
        } // end while

        // 返回 true 表示成功解析 term
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
            currentToken = getCurrentToken(); // 更新 Token 類型
            
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
        if ( symbolTable.find( ident.tokenName ) != symbolTable.end() ) {   // IDENT有定義 
            ident = symbolTable[ident.tokenName];
        } // end if
        
        else {                                                              // IDENT無定義
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
            
            match();  // 移動到下一個 token，即 Term 或 Factor 的開始
            

            if (op == PLUS || op == MINUS) {
                if (!term(parsedResult)) {
                    return false;
                } // end if
                
                parsedResult = evaluateOperation(ident, parsedResult, op); // 執行運算
            } // end if
            
            else if (op == MULTIPLY || op == DIVIDE) {
                if (!factor(parsedResult)) {
                    return false;
                } // end if
                
                parsedResult = evaluateOperation(ident, parsedResult, op); // 執行運算
            } // end else if
            
            ident = parsedResult ;
        } // end while
        
        if (BooleanOperator()) {
        	  Type op = currentTokenType();
            match();  // 移動到 Boolean 表達式的開始
            
            Token nextOperand;
            if (!arithExp(nextOperand)) {
                return false;
            } // end if
            
            parsedResult = compareOperation(parsedResult, nextOperand, op); // 執行布林運算
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
            if ( BooleanOperator() ) {
                Type op = currentTokenType();  // 獲取比較運算符
                match();
                
                Token rightExpr;
                if (arithExp(rightExpr)) {
                    // 比較兩個算術表達式的結果
                    // 此處應該有比較邏輯的實現，這裡僅示範結構
                    parsedResult = compareOperation(parsedResult, rightExpr, op);
                    return true;
	      } // end if
	      
                else {
                	return false ;
	      } // end else
            } // end if
            
            else {
                parsedResult.type = ERROR;
                parsedResult.error.type = syntacticalError;
                parsedResult.error.errorValue = getCurrentToken().tokenName;
                return false ;	
	  } // end else
        } // end if
        
        else {
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
        
        // 解析第一個 term
        if (!term(parsedResult)) {
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的加法或減法運算
        Token a = parsedResult;
        // 當當前 token 是加號或減號時，進行運算
        while (currentTokenType() == SIGN ) {
            // 保存運算符
            Type op;
            if ( getCurrentToken().tokenName.compare( "-" ) == 0 ) op = MINUS;
            else op = PLUS ;
            match();  // 移動到下一個 token

            // 解析下一個 term
            Token b;
            
            if (!term(b)) {
                parsedResult = b;
                return false;
            } // end if
            
            // 根據運算符進行相應的運算，並更新 a 為運算結果
            parsedResult = evaluateOperation(a, b, op);

        } // end while 

        // 返回 true 表示成功解析 arithExp
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
    	
        // 首先解析一個 factor
        if (!factor(parsedResult)) {
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的乘法或除法運算
        Token a = parsedResult;

        // 當當前 token 是乘法或除法運算符時，進行運算
        while (currentTokenType() == MULTIPLY || currentTokenType() == DIVIDE) {
            // 保存運算符
            Type op = currentTokenType();
            match();  // 移動到下一個 token

            // 解析下一個 factor
            Token b;
            if (!factor(b)) {
                parsedResult = b ;
                return false;
            } // end if

            // 根據運算符進行相應的運算，並更新 a 為運算結果
            parsedResult = evaluateOperation(a, b, op);

        } // end while

        // 返回 true 表示成功解析 term
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
            currentToken = getCurrentToken(); // 更新 Token 類型
            
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
    
    // 如果當前token符合文法，則往後移一位 
    void match() {
        if ( currentTokenIndex < tokens.size() ) {
            currentTokenIndex++;
        } // end if

        else return  ;
    } // end match()
    
    // 獲取當前token的類型 
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
