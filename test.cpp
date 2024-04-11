  void statement( Token &parsedResult ) {

    if ( nextToken.type == ';' ) {
        if ( !Match(';', parsedResult) ) 
          return; 

    } // end if 
    
    else if (nextToken.type == IDENTIFIER || nextToken.type == CONSTANT) {
        expression();
        if (!Match(';', parsedResult)) {
            return; // �Ϊ̶i���L���~�B�z
        }
    } 
    
    else if (nextToken.type == RETURN) {
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
        }
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
} // end statement()

  bool expression( Token &parsedResult ) {
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
  
  bool basic_expression( Token &parsedResult ) {
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
        if ( !romce_and_romloe( parsedResult ) ) return false;
    } // end else if
    
    if ( nextToken.type == LPAREN ) {
      Match( LPAREN, parsedResult );
      if ( !actual_parameter_list( parsedResult ) ) return false;
        
      if ( !Match( RPAREN, parsedResult ) ) return false;
      return romce_and_romloe( parsedResult );
    } // end if
    return true;
  } // end rest_of_Identifier_started_basic_exp()
  
  bool rest_of_PPMM_Identifier_started_basic_exp( Token &parsedResult ) {
    if ( nextToken.type == LBRACKET ) {
        Match( LBRACKET, parsedResult );
        if ( !expression(parsedResult) ) return false;  // �ѪR��?��
        if ( !Match( RBRACKET, parsedResult ) ) return false;  // ���� ']'
    } // end if

    return romce_and_romloe( parsedResult );  // �ѪR�Z?��?��
  } // end rest_of_PPMM_Identifier_started_basic_exp()
  
  bool sign( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        return Match( nextToken.type, parsedResult );  // ���Ӧ}?�d��?
    } // end if
    
    return false;  // �p�G���O?������?�A��^ false
  } // end sign()

  bool actual_parameter_list( Token &parsedResult ) {
    if ( !basic_expression( parsedResult ) ) return false;  // �ѪR�Ĥ@?�򥻪�?��
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // ���� ','
      if ( !basic_expression( parsedResult ) ) return false;  // �ѪR�Z?���򥻪�?��
    } // end while
    
    return true;
  } // end actual_parameter_list()

  bool romce_and_romloe( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_OR_exp( parsedResult ) ) return false;  // ?�z??�Ϊ�?��

    if ( nextToken.type == QUESTION ) {
        Match( QUESTION, parsedResult );  // ���� '?'
        if ( !basic_expression( parsedResult ) ) return false;  // �ѪR?��?�u?����?��
        if ( !Match( COLON, parsedResult ) ) return false;  // ���� ':'
        if ( !basic_expression( parsedResult ) ) return false;  // �ѪR?��?��?����?��
    } // end if

    return true;
  } // end romce_and_romloe(
  
  bool rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?�z??�O��?��

    while ( nextToken.type == OR ) {
      Match( OR, parsedResult );  // ���� '||'
      if ( !maybe_logical_AND_exp( parsedResult ) ) return false;  // �ѪR??�O��?��
    } // end while

    return true;
  } // end rest_of_maybe_logical_OR_exp()
  
  bool maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Ĥ@?�i�઺��Ϊ�?��

    while (nextToken.type == AND) {
        Match( AND, parsedResult );  // ���� '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end maybe_logical_AND_exp()
  
  bool rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z��Ϊ�?�����ѧE����

    while ( nextToken.type == AND ) {
        Match( AND, parsedResult );  // ���� '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?�z�Z?����Ϊ�?��
    } // end while

    return true;
  } // end rest_of_maybe_logical_AND_exp()
  
  bool maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?�z�Ĥ@?�i�઺���ݩΪ�?��

    while ( nextToken.type == BIT_OR ) {
        Match( BIT_OR, parsedResult );  // ���� '|'
        if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?�z�Z?�����ݩΪ�?��
    } // end while

    return true;
  } // end maybe_bit_OR_exp()

bool Parser::rest_of_maybe_bit_OR_exp(Token &parsedResult) {
    if (!rest_of_maybe_bit_ex_OR_exp(parsedResult)) return false;

    while (nextToken.type == BIT_OR) {
        Match(BIT_OR, parsedResult);
        if (!maybe_bit_ex_OR_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_bit_ex_OR_exp(Token &parsedResult) {
    if (!maybe_bit_AND_exp(parsedResult)) return false;

    while (nextToken.type == BIT_XOR) {
        Match(BIT_XOR, parsedResult);
        if (!maybe_bit_AND_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_bit_ex_OR_exp(Token &parsedResult) {
    if (!rest_of_maybe_bit_AND_exp(parsedResult)) return false;

    while (nextToken.type == BIT_XOR) {
        Match(BIT_XOR, parsedResult);
        if (!maybe_bit_AND_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_bit_AND_exp(Token &parsedResult) {
    if (!maybe_equality_exp(parsedResult)) return false;

    while (nextToken.type == BIT_AND) {
        Match(BIT_AND, parsedResult);
        if (!maybe_equality_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_bit_AND_exp(Token &parsedResult) {
    if (!rest_of_maybe_equality_exp(parsedResult)) return false;

    while (nextToken.type == BIT_AND) {
        Match(BIT_AND, parsedResult);
        if (!maybe_equality_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_equality_exp(Token &parsedResult) {
    if (!maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // ���� EQ �� NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_equality_exp(Token &parsedResult) {
    if (!rest_of_maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // ���� EQ �� NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_relational_exp(Token &parsedResult) {
    if (!maybe_shift_exp(parsedResult)) return false;

    while (nextToken.type == LESS || nextToken.type == GREATER || 
           nextToken.type == LE || nextToken.type == GE) {
        Match(nextToken.type, parsedResult);  // ���� <, >, <=, �� >=
        if (!maybe_shift_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_relational_exp(Token &parsedResult) {
    if (!rest_of_maybe_shift_exp(parsedResult)) return false;

    while (nextToken.type == LESS || nextToken.type == GREATER || 
           nextToken.type == LE || nextToken.type == GE) {
        Match(nextToken.type, parsedResult);  // ���� <, >, <=, �� >=
        if (!maybe_shift_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_shift_exp(Token &parsedResult) {
    if (!maybe_additive_exp(parsedResult)) return false;

    while (nextToken.type == LS || nextToken.type == RS) {
        Match(nextToken.type, parsedResult);  // ���� << �� >>
        if (!maybe_additive_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_shift_exp(Token &parsedResult) {
    if (!rest_of_maybe_additive_exp(parsedResult)) return false;

    while (nextToken.type == LS || nextToken.type == RS) {
        Match(nextToken.type, parsedResult);  // ���� LS �� RS
        if (!maybe_additive_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_additive_exp(Token &parsedResult) {
    if (!maybe_mult_exp(parsedResult)) return false;

    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // ���� '+' �� '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_additive_exp(Token &parsedResult) {
    if (!rest_of_maybe_mult_exp(parsedResult)) return false;

    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // ���� '+' �� '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
}


bool Parser::maybe_mult_exp(Token &parsedResult) {
    if (!unary_exp(parsedResult)) return false;
    return rest_of_maybe_mult_exp(parsedResult);  // ?�z�Z?�����k�B���k�Ψ��Ҿާ@
}


bool Parser::rest_of_maybe_mult_exp(Token &parsedResult) {
    while (nextToken.type == MUL || nextToken.type == DIV || nextToken.type == MOD) {
        Match(nextToken.type, parsedResult);  // ���� '*', '/', �� '%'
        if (!unary_exp(parsedResult)) return false;
    }
    return true;  // ?�����i�H?�šA�]���q?��^ true
}

bool Parser::unary_exp(Token &parsedResult) {
    if (nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT) {
        while (nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT) {
            sign(parsedResult);  // ?�z�Ҧ��e�m����?
        }
        return signed_unary_exp(parsedResult);
    } else if (nextToken.type == IDENTIFIER || nextToken.type == CONSTANT || nextToken.type == LPAREN) {
        return unsigned_unary_exp(parsedResult);
    } else if (nextToken.type == PP || nextToken.type == MM) {
        Match(nextToken.type, parsedResult); // ���� PP �� MM
        if (Match(IDENTIFIER, parsedResult)) {
            if (nextToken.type == LBRACKET) {
                Match(LBRACKET, parsedResult); // ���� '['
                if (!expression(parsedResult)) return false; 
                if (!Match(RBRACKET, parsedResult)) return false; // ���� ']'
            }
            return true;
        }
        return false;
    }
    return false;
}

bool Parser::signed_unary_exp(Token &parsedResult) {
    if (nextToken.type == IDENTIFIER) {
        Match(IDENTIFIER, parsedResult);
        if (nextToken.type == LPAREN) {
            Match(LPAREN, parsedResult); // ���� '('
            if (nextToken.type != RPAREN) { // �s�b??�C��
                if (!actual_parameter_list(parsedResult)) return false;
            }
            if (!Match(RPAREN, parsedResult)) return false; // ���� ')'
        } else if (nextToken.type == LBRACKET) {
            Match(LBRACKET, parsedResult); // ���� '['
            if (!expression(parsedResult)) return false;
            if (!Match(RBRACKET, parsedResult)) return false; // ���� ']'
        }
        return true;
    } else if (nextToken.type == CONSTANT) {
        return Match(CONSTANT, parsedResult);
    } else if (nextToken.type == LPAREN) {
        Match(LPAREN, parsedResult); // ���� '('
        if (!expression(parsedResult)) return false;
        return Match(RPAREN, parsedResult); // ���� ')'
    }
    return false;
}

bool Parser::unsigned_unary_exp(Token &parsedResult) {
    if (nextToken.type == IDENTIFIER) {
        Match(IDENTIFIER, parsedResult);
        if (nextToken.type == LPAREN) {
            Match(LPAREN, parsedResult); // ���� '('
            if (nextToken.type != RPAREN) { // �s�b??�C��
                if (!actual_parameter_list(parsedResult)) return false;
            }
            if (!Match(RPAREN, parsedResult)) return false; // ���� ')'
        } else if (nextToken.type == LBRACKET) {
            Match(LBRACKET, parsedResult); // ���� '['
            if (!expression(parsedResult)) return false;
            if (!Match(RBRACKET, parsedResult)) return false; // ���� ']'
            if (nextToken.type == PP || nextToken.type == MM) {
                Match(nextToken.type, parsedResult); // ?�z�Z�m�� PP �� MM
            }
        }
        return true;
    } else if (nextToken.type == CONSTANT) {
        return Match(CONSTANT, parsedResult);
    } else if (nextToken.type == LPAREN) {
        Match(LPAREN, parsedResult); // ���� '('
        if (!expression(parsedResult)) return false;
        return Match(RPAREN, parsedResult); // ���� ')'
    }
    return false;
}

