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
            return; // �Ϊ̶i���L���~�B�z
        }
        if (nextToken.type != ';') {
            expression();
        }
        if (!Match(';', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
    } else if (nextToken.type == '{') {
        compound_statement();
    } else if (nextToken.type == IF) {
        if (!Match(IF, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match('(', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        statement();
        if (nextToken.type == ELSE) {
            if (!Match(ELSE, parsedResult)) {
                return; // �Ϊ̶i���L���~�B�z
            }
            statement();
        }
    } else if (nextToken.type == WHILE) {
        if (!Match(WHILE, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match('(', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        statement();
    } else if (nextToken.type == DO) {
        if (!Match(DO, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        statement();
        if (!Match(WHILE, parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }maybe_equality_exp
        if (!Match('(', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        expression();
        if (!Match(')', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
        if (!Match(';', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
    } else {
        // �B�z���w���� token
        std::cerr << "Unexpected token in statement" << std::endl;
        return; // �פ�B�z
    }
*/
  } // end statement()




bool Parser::maybe_mult_exp(Token &parsedResult) {
    if (!unary_exp(parsedResult)) return false;
    return rest_of_maybe_mult_exp(parsedResult);  // ?�z�Z?�����k�B���k�Ψ��Ҿާ@
}







