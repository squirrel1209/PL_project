  bool statement( Token &parsedResult ) {
    if ( nextToken.type == ';' ) {
      if ( Match( ';', parsedResult ) ) 
        return true;
    } // end if 

    else if ( StartExpression() ) {
      if ( !expression() ) return false;
      if ( !Match( ';', parsedResult ) ) return false;
      return true; 
    } // end else if
/*
    else if ( nextToken.type == RETURN ) {
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
        }maybe_equality_exp
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
*/
  } // end statement()




bool Parser::maybe_mult_exp(Token &parsedResult) {
    if (!unary_exp(parsedResult)) return false;
    return rest_of_maybe_mult_exp(parsedResult);  // ?理后?的乘法、除法或取模操作
}







