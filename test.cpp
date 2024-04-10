  void statement( Token& parsedResult ) {

    if ( nextToken.type == ';' ) {
        if ( !Match(';', parsedResult) ) 
          return; 

    } // end if 
    
    else if (nextToken.type == IDENTIFIER || nextToken.type == CONSTANT) {
        expression();
        if (!Match(';', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
    } 
    
    else if (nextToken.type == RETURN) {
        if (!Match(RETURN, parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        if (nextToken.type != ';') {
            expression();
        }
        if (!Match(';', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
    } else if (nextToken.type == '{') {
        compound_statement();
    } else if (nextToken.type == IF) {
        if (!Match(IF, parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        if (!Match('(', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        statement();
        if (nextToken.type == ELSE) {
            if (!Match(ELSE, parsedResult)) {
                return; // 或者進行其他錯誤處理
            }
            statement();
        }
    } else if (nextToken.type == WHILE) {
        if (!Match(WHILE, parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        if (!Match('(', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        statement();
    } else if (nextToken.type == DO) {
        if (!Match(DO, parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        statement();
        if (!Match(WHILE, parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        if (!Match('(', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
        if (!Match(';', parsedResult)) {
            return; // 或者進行其他錯誤處理
        }
    } else {
        // 處理未預期的 token
        std::cerr << "Unexpected token in statement" << std::endl;
        return; // 終止處理
    }
} // end statement()

  bool expression( Token& parsedResult ) {
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
  
  bool basic_expression( Token& parsedResult ) {
    if ( nextToken.type == IDENTIFIER ) {
      return Match( IDENTIFIER, parsedResult )
      && rest_of_Identifier_started_basic_exp( parsedResult );
    } // end if 
    
    else if ( nextToken.type == PP || nextToken.type == MM ) {
      return Match( nextToken.type, parsedResult ) &&
        Match( IDENTIFIER, parsedResult ) &&
        rest_of_PPMM_Identifier_started_basic_exp( parsedResult );
    } // end else if 
    
    else if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
      sign( parsedResult );
      while ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        sign( parsedResult ); 
      } // end while

      return signed_unary_exp( parsedResult ) && romce_and_romloe( parsedResult );
    } // end else if 
    
    else if ( nextToken.type == CONSTANT ) 
      return Match( CONSTANT, parsedResult ) && romce_and_romloe( parsedResult );

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
        return romce_and_romloe( parsedResult );
    } // end else if
    
    else if ( nextToken.type == LPAREN ) {
        Match( LPAREN, parsedResult );
        if ( nextToken.type != RPAREN ) {
          if ( !actual_parameter_list( parsedResult ) ) return false;
        } // end if
        
        if ( !Match(RPAREN, parsedResult) ) return false;
        return romce_and_romloe(parsedResult);
    } // end else if

    return true;
  }
  
  bool assignment_operator( Token &parsedResult ) {
    if ( nextToken.type == ASSIGN || nextToken.type == TE || nextToken.type == DE ||
        nextToken.type == RE || nextToken.type == PE || nextToken.type == ME) {
        Match( nextToken.type, parsedResult );
        return true;
    } // end if

    return false;
  } // end assignment_operator()



