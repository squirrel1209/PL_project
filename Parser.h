#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <map>
#include "Tokenizer.h"
#include <stdexcept>

using namespace std;
map<string, double> symbolTable;

class Parser {
public:
    Parser( vector<Token> inputTokens ): tokens(inputTokens), currentTokenIndex(0) {}
    void parse() {
        Token parsedResult ;
        factor( parsedResult ) ;
        while ( parsedResult.type != QUIT ) {
        
        if ( parsedResult.type == ERROR ) {
            if ( parsedResult.error.type == lexicalError ) {
                cout << "unrecognized token with first char : '" << parsedResult.error.errorValue << "'" << endl;
	  } // end if
	  
	  else if ( parsedResult.error.type == syntacticalError ) {
	      cout << "unexpected token : '" << parsedResult.error.errorValue << "'" << endl;
	  } // end else if
	  
	  else if ( parsedResult.error.type == semanticError ) {
	      cout << "undefined identifier : '" << parsedResult.error.errorValue << "'" << endl;
	  } // end else if
	  
	  else cout << "unknowed Error" << endl ;
        } // end if
        
        else cout << "correct:" << parsedResult.tokenName << endl;
        parsedResult = createToken( "", NONE, {lexicalError, 0, ""} ) ;
        term( parsedResult ) ;
        }
    } // end parse

	
private:
    vector<Token> tokens;
    int currentTokenIndex;
    
    Token evaluateOperation( Token a, Token b, Type op ) {
        Token answer ;
        float floatA = atof( a.tokenName.c_str() );
        float floatB = atof( b.tokenName.c_str() );
        float floatAnswer ;
         
        if ( op == Type::MULTIPLY ) {
            floatAnswer = floatA * floatB;
        } // end if
                    
        else if ( op == Type::DIVIDE ) {
        	  floatAnswer = floatA / floatB;
        } // end else if
        
        else if ( op == Type::PLUS ) {
        	  floatAnswer = floatA + floatB;
        } // end else if
        
        else if ( op == Type::MINUS ) {
        	  floatAnswer = floatA - floatB;
        } // end else if
        
        if ( a.type == Type::INT && b.type == Type::INT ) {
            int intAnswer = (int)floatAnswer; 
            answer.tokenName = anyToString( intAnswer ) ;
            answer.type = Type::INT;
            return answer;
        } // end if
    
        else {
            answer.tokenName = anyToString( floatAnswer ) ;
            answer.type = Type::FLOAT; // Assuming FLOAT as the type for floating point numbers
            return answer;
        } // end else
    } // end evaluateOperation()
    
    Token compareOperation(const Token &left, const Token &right, Type op) {
        Token result;
        result.type = Type::BOOL;  // 假設 Token 有一個 BOOL 類型用於表示布林值

        // 將字面值轉換為數值進行比較
        double leftVal = atof(left.tokenName.c_str());
        double rightVal = atof(right.tokenName.c_str());
        bool comparisonResult;

        // 根據操作符進行比較，直接在 case 中返回結果，避免使用 break
        switch (op) {
            case Type::EQUAL:
                comparisonResult = leftVal == rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case Type::NOTEQUAL:
                comparisonResult = leftVal != rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case Type::GREATER:
                comparisonResult = leftVal > rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case Type::LESS:
                comparisonResult = leftVal < rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case Type::GREATEREQUAL:
                comparisonResult = leftVal >= rightVal;
                result.tokenName = comparisonResult ? "true" : "false";
                return result;
                
            case Type::LESSEQUAL:
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
            match();  // 移動到下一個 token，即 ASSIGN
            
            if (currentTokenType() == ASSIGN) {
                match();  // 移動到賦值表達式 
                
	      if ( arithExp( parsedResult ) ) {
                    // 將算術表達式的結果賦值給標識符
                    symbolTable[ident.tokenName] = atof(parsedResult.tokenName.c_str());
	          
	          if ( currentTokenType() == SEMICOLON ) {
	              match();  // 消耗掉分號
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
        } // end else if
	  /*
	  else if ( IDlessArithExpOrBexp(ident) ) {
	      parsedResult = ident; 
	      
                if ( currentTokenType() == Type::SEMICOLON ) {
	          match();
	          return true ;
	      } // end if
		
	      else {
	          throw runtime_error("Error: Missing ';' after IDlessArithExpOrBexp.");
                    return false ;	
                } // end else
	  } // end else if
	  
	  else {
                throw runtime_error("Error: Invalid IDlessArithExpOrBexp.");
                return false ;
	  } // end else
        } // end else if

        else if ( NOT_IDStartArithExpOrBexp(parsedResult) ) {
            if ( currentTokenType() == Type::SEMICOLON ) {
	      match();
	      return true ;
	  } // end if
		
	  else {
	      throw runtime_error("Error: Missing ';' after NOT_IDStartArithExpOrBexp.");
                return false ;	
            } // end else
        } // end else if 

        else {
            throw runtime_error("Error: Unexpected token in command.");
            return false ;
        } // end else
        */
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
                    throw runtime_error("Error: Invalid right-hand side expression in NOT_IDStartArithExpOrBexp.");
                    return false;
                } // end else
            } // end if
            
            else {
                // 如果沒有跟隨布爾運算符，則返回 true
                return true;
            } // end else
        }  // end if   
        
        else {
            throw runtime_error("Error: Unexpected token in NOT_IDStartArithExpOrBexp.");
            return false;
        } // end else
    } // end NOT_IDStartArithExpOrBexp

    bool NOT_ID_StartArithExp(Token &parsedResult) {
        // 解析第一個 term
        if (!NOT_ID_StartTerm(parsedResult)) {
            throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的加法或減法運算
        Token a = parsedResult;

        // 當當前 token 是加號或減號時，進行運算
        while (currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS) {
            // 保存運算符
            Type op = currentTokenType();
            match();  // 移動到下一個 token

            // 解析下一個 term
            Token b;
            if (!term(b)) {
                throw runtime_error("Error: Unexpected token in arithExp after operator.");
                return false;
            } // end if

            // 根據運算符進行相應的運算，並更新 a 為運算結果
            a = evaluateOperation(a, b, op);

            // 更新 parsedResult 為目前的結果
            parsedResult = a;
        } // end while 

        // 返回 true 表示成功解析 arithExp
        return true;
    } // end NOT_IDStartArithExpOrBexp()
    
    bool NOT_ID_StartTerm(Token &parsedResult) {
        // 首先解析一個 factor
        if (!NOT_ID_StartFactor(parsedResult)) {
            throw runtime_error("Error: Unexpected token in term, expecting a factor.");
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的乘法或除法運算
        Token a = parsedResult;

        // 當當前 token 是乘法或除法運算符時，進行運算
        while (currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE) {
            // 保存運算符
            Type op = currentTokenType();
            match();  // 移動到下一個 token

            // 解析下一個 factor
            Token b;
            if (!factor(b)) {
                throw runtime_error("Error: Unexpected token in term after operator.");
                return false;
            } // end if

            // 根據運算符進行相應的運算，並更新 a 為運算結果
            a = evaluateOperation(a, b, op);

            // 更新 rule 為目前的結果
            parsedResult = a;
        } // end while

        // 返回 true 表示成功解析 term
        return true;
    } // end NOT_ID_StartTerm()
    
    bool NOT_ID_StartFactor(Token &parsedResult) {        
        Token currentToken = getCurrentToken(); 
        
        if ( currentToken.type == Type::ERROR ) {
            return false ;
        } // end if
    	
        else if ( currentToken.type == Type::SIGN ) {
        	  parsedResult = currentToken ;
            match();
            currentToken = getCurrentToken(); // 更新 Token 類型
            
            if ( currentToken.type == Type::INT || currentToken.type == Type::FLOAT ) {
                parsedResult.tokenName += currentToken.tokenName;
                parsedResult.type = currentToken.type ;
                match();
                return true ;
            } // end if
            
            else {
                throw runtime_error("Error: Unexpected token in factor.");
	      return false ;	
	  } // end else
        } // end else if
        
        else if ( currentToken.type == Type::INT || currentToken.type == Type::FLOAT ) {
        	  parsedResult = currentToken ;
            match();
            return true ;
        } // end else if
        
        else if ( currentToken.type == Type::LPAREN ) {
            match();
            
            if ( arithExp(parsedResult) ) {
                match();
                currentToken = getCurrentToken();
                
                if ( currentToken.type == Type::RPAREN ) {
                    match();
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
    
    // <IDlessArithExpOrBexp>  ::= {   '+' <Term>   | '-' <Term> | '*' <Factor> | '/' <Factor> }
    //                             [ <BooleanOperator> <ArithExp> ]
    bool IDlessArithExpOrBexp( Token &parsedResult ) {
        Token tempResult = parsedResult; // 使用一個臨時變數還儲存運算結果
         
        while ( currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS ||
                currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE ) {
            Type op = currentTokenType();
            match();  // 移動到下一個 token，即 Term 或 Factor 的開始
            
            Token nextOperand;
            if (op == Type::PLUS || op == Type::MINUS) {
                if (!term(parsedResult)) {
                    throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp after PLUS or MINUS.");
                    return false;
                } // end if
                
                tempResult = evaluateOperation(tempResult, nextOperand, op); // 執行運算
            } // end if
            
            else if (op == Type::MULTIPLY || op == Type::DIVIDE) {
                if (!factor(parsedResult)) {
                    throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp after MULTIPLY or DIVIDE.");
                    return false;
                } // end if
                
                tempResult = evaluateOperation(tempResult, nextOperand, op); // 執行運算
            } // end else if
        } // end while
        
        if (BooleanOperator()) {
        	  Type op = currentTokenType();
            match();  // 移動到 Boolean 表達式的開始
            
            Token nextOperand;
            if (!arithExp(nextOperand)) {
                throw runtime_error("Error: Unexpected token in IDlessArithExpOrBexp Boolean expression.");
                return false;
            } // end if
            
            tempResult = compareOperation(tempResult, nextOperand, op); // 執行布林運算
        } // end if
    
        parsedResult = tempResult; // 將臨時結果賦值回傳入結果變量
        return true;
    } // end IDlessArithExpOrBexp

    bool BooleanOperator() {
        if ( currentTokenType() == Type::EQUAL || currentTokenType() == Type::NOTEQUAL || currentTokenType() == Type::GREATER ||
             currentTokenType() == Type::LESS || currentTokenType() == Type::GREATEREQUAL || currentTokenType() == Type::LESSEQUAL ) 
	   return true ;
        else return false ;	
    } // end BooleanOperator()
/*
    // <Statement> ::= IDENT ':=' <ArithExp>
    bool statement ( Token &parsedResult ) {
        Token identToken, resultToken;
        
        if ( currentTokenType() == Type::IDENT ) {
            identToken = getCurrentToken();  // 獲取標識符 Token
            match();  // 移過標識符
            
            if ( currentTokenType() == Type::ASSIGN ) {
                match();  // 移過賦值運算符 ':='
                
                if ( arithExp(resultToken) ) { // 解析算術表達式並獲取結果
                    // 將算術表達式的結果賦值給標識符
                    symbolTable[identToken.tokenName] = stod(resultToken.tokenName);
                    parsedResult = resultToken;
                    match();
                    return true ;    
	      } // end if
	      
                else {
                    throw runtime_error("Error: Invalid arithmetic expression in statement.");
                    return false ;
	      } // end else
            } // end if
            
            else {
                parsedResult.type = ERROR;
                parsedResult.error.type = syntacticalError;
                parsedResult.error.errorValue = resultToken.tokenName;
                throw runtime_error("Error: Unexpected token in statement.");
                return false ;
	  } // end else 
        } // end if
        
        else {
        	  parsedResult.type = ERROR;
            parsedResult.error.type = syntacticalError;
            parsedResult.error.errorValue = resultToken.tokenName;
            throw runtime_error("Error: Expected an identifier in statement.");
	  return false ;   
        } // end else
    } // end statement()
*/
    // <BooleanExp> ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
    bool booleanExp( Token &parsedResult ) {
        if ( arithExp(parsedResult) ) {
        	  match();
        	  
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
                	throw runtime_error("Error: Invalid right-hand side expression in booleanExp.");
                	return false ;
	      } // end else
            } // end if
            
            else {
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
        
        if ( currentToken.type == Type::ERROR || currentToken.type == Type::QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
        
        // 解析第一個 term
        if (!term(parsedResult)) {
            throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的加法或減法運算
        Token a = parsedResult;
        
        // 當當前 token 是加號或減號時，進行運算
        while (currentTokenType() == Type::PLUS || currentTokenType() == Type::MINUS) {
            // 保存運算符
            Type op = currentTokenType();
            match();  // 移動到下一個 token

            // 解析下一個 term
            
            Token b;
            if (!term(b)) {
                throw runtime_error("Error: Unexpected token in arithExp, expecting a term.");
                return false;
            } // end if

            // 根據運算符進行相應的運算，並更新 a 為運算結果
            a = evaluateOperation(a, b, op);

            // 更新 parsedResult 為目前的結果
            parsedResult = a;
        } // end while 

        // 返回 true 表示成功解析 arithExp
        match();
        return true;
    } // end arithExp()

    // <Term> ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
    bool term(Token &parsedResult) {
        Token currentToken = getCurrentToken();
        
        if ( currentToken.type == Type::ERROR || currentToken.type == Type::QUIT ) {
            parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if
    	
        // 首先解析一個 factor
        if (!factor(parsedResult)) {
            throw runtime_error("Error: Unexpected token in term, expecting a factor.");
            return false;
        } // end if

        // 保存當前解析到的 token，可能用於後續的乘法或除法運算
        Token a = parsedResult;

        // 當當前 token 是乘法或除法運算符時，進行運算
        while (currentTokenType() == Type::MULTIPLY || currentTokenType() == Type::DIVIDE) {
            // 保存運算符
            Type op = currentTokenType();
            match();  // 移動到下一個 token

            // 解析下一個 factor
            Token b;
            if (!factor(b)) {
                parsedResult = b ;
                throw runtime_error("Error: Unexpected token in term, expecting a factor.");
                return false;
            } // end if

            // 根據運算符進行相應的運算，並更新 a 為運算結果
            a = evaluateOperation(a, b, op);

            // 更新 rule 為目前的結果
            parsedResult = a;
        } // end while

        // 返回 true 表示成功解析 term
        match();
        return true;
    } // end term()
    
    // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'  	
    bool factor( Token &parsedResult ) {
        Token currentToken = getCurrentToken();
        if ( currentToken.type == Type::ERROR || currentToken.type == Type::QUIT ) {
        	  parsedResult = currentToken;
        	  match();  // match ERROR || QUIT
            return false ;
        } // end if

        if ( currentToken.type == Type::IDENT ) {
        	 if ( symbolTable.find(currentToken.tokenName) != symbolTable.end() ) {
        	     parsedResult = currentToken ;
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

        else if ( currentToken.type == Type::SIGN ) {
        	  if ( currentToken.tokenName.compare( "-" ) == 0 )
        	      parsedResult = currentToken ;

            match();
            currentToken = getCurrentToken(); // 更新 Token 類型
            
            if ( currentToken.type == Type::INT || currentToken.type == Type::FLOAT ) {
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
        
        else if ( currentToken.type == Type::INT || currentToken.type == Type::FLOAT ) {
        	  parsedResult = currentToken ;
            match();
            return true ;
        } // end else if
        
        else if ( currentToken.type == Type::LPAREN ) {
            match();
            
            if ( arithExp(parsedResult) ) { 
	      currentToken = getCurrentToken();
                
                if ( currentToken.type == Type::RPAREN ) {
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
                throw runtime_error("Error: arithExp，Unexpected token in factor. ");
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
            return Type::QUIT;
        } // end else
    } // end currentTokenType
    
    Token getCurrentToken() {
        Token quit;
        quit.type = Type::QUIT;

        if ( currentTokenIndex < tokens.size() ) {
            return tokens[currentTokenIndex];
        } // end if
         
        else {
        	return quit;
        }
        
    } // end currentTokenValue()
    
};

#endif // PARSER_H
