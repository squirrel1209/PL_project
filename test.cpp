  void statement( Token &parsedResult ) {

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
        if ( !expression(parsedResult) ) return false;  // 解析表?式
        if ( !Match( RBRACKET, parsedResult ) ) return false;  // 消耗 ']'
    } // end if

    return romce_and_romloe( parsedResult );  // 解析后?表?式
  } // end rest_of_PPMM_Identifier_started_basic_exp()
  
  bool sign( Token &parsedResult ) {
    if ( nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT ) {
        return Match( nextToken.type, parsedResult );  // 消耗并?查符?
    } // end if
    
    return false;  // 如果不是?期的符?，返回 false
  } // end sign()

  bool actual_parameter_list( Token &parsedResult ) {
    if ( !basic_expression( parsedResult ) ) return false;  // 解析第一?基本表?式
    while ( nextToken.type == COMMA ) {
      Match( COMMA, parsedResult );  // 消耗 ','
      if ( !basic_expression( parsedResult ) ) return false;  // 解析后?的基本表?式
    } // end while
    
    return true;
  } // end actual_parameter_list()

  bool romce_and_romloe( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_OR_exp( parsedResult ) ) return false;  // ?理??或表?式

    if ( nextToken.type == QUESTION ) {
        Match( QUESTION, parsedResult );  // 消耗 '?'
        if ( !basic_expression( parsedResult ) ) return false;  // 解析?件?真?的表?式
        if ( !Match( COLON, parsedResult ) ) return false;  // 消耗 ':'
        if ( !basic_expression( parsedResult ) ) return false;  // 解析?件?假?的表?式
    } // end if

    return true;
  } // end romce_and_romloe(
  
  bool rest_of_maybe_logical_OR_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_logical_AND_exp( parsedResult ) ) return false;  // ?理??与表?式

    while ( nextToken.type == OR ) {
      Match( OR, parsedResult );  // 消耗 '||'
      if ( !maybe_logical_AND_exp( parsedResult ) ) return false;  // 解析??与表?式
    } // end while

    return true;
  } // end rest_of_maybe_logical_OR_exp()
  
  bool maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位或表?式

    while (nextToken.type == AND) {
        Match( AND, parsedResult );  // 消耗 '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end maybe_logical_AND_exp()
  
  bool rest_of_maybe_logical_AND_exp( Token &parsedResult ) {
    if ( !rest_of_maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理位或表?式的剩余部分

    while ( nextToken.type == AND ) {
        Match( AND, parsedResult );  // 消耗 '&&'
        if ( !maybe_bit_OR_exp( parsedResult ) ) return false;  // ?理后?的位或表?式
    } // end while

    return true;
  } // end rest_of_maybe_logical_AND_exp()
  
  bool maybe_bit_OR_exp( Token &parsedResult ) {
    if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理第一?可能的位异或表?式

    while ( nextToken.type == BIT_OR ) {
        Match( BIT_OR, parsedResult );  // 消耗 '|'
        if ( !maybe_bit_ex_OR_exp( parsedResult ) ) return false;  // ?理后?的位异或表?式
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
        Match(nextToken.type, parsedResult);  // 消耗 EQ 或 NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_equality_exp(Token &parsedResult) {
    if (!rest_of_maybe_relational_exp(parsedResult)) return false;

    while (nextToken.type == EQ || nextToken.type == NEQ) {
        Match(nextToken.type, parsedResult);  // 消耗 EQ 或 NEQ
        if (!maybe_relational_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_relational_exp(Token &parsedResult) {
    if (!maybe_shift_exp(parsedResult)) return false;

    while (nextToken.type == LESS || nextToken.type == GREATER || 
           nextToken.type == LE || nextToken.type == GE) {
        Match(nextToken.type, parsedResult);  // 消耗 <, >, <=, 或 >=
        if (!maybe_shift_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_relational_exp(Token &parsedResult) {
    if (!rest_of_maybe_shift_exp(parsedResult)) return false;

    while (nextToken.type == LESS || nextToken.type == GREATER || 
           nextToken.type == LE || nextToken.type == GE) {
        Match(nextToken.type, parsedResult);  // 消耗 <, >, <=, 或 >=
        if (!maybe_shift_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_shift_exp(Token &parsedResult) {
    if (!maybe_additive_exp(parsedResult)) return false;

    while (nextToken.type == LS || nextToken.type == RS) {
        Match(nextToken.type, parsedResult);  // 消耗 << 或 >>
        if (!maybe_additive_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_shift_exp(Token &parsedResult) {
    if (!rest_of_maybe_additive_exp(parsedResult)) return false;

    while (nextToken.type == LS || nextToken.type == RS) {
        Match(nextToken.type, parsedResult);  // 消耗 LS 或 RS
        if (!maybe_additive_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::maybe_additive_exp(Token &parsedResult) {
    if (!maybe_mult_exp(parsedResult)) return false;

    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // 消耗 '+' 或 '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
}

bool Parser::rest_of_maybe_additive_exp(Token &parsedResult) {
    if (!rest_of_maybe_mult_exp(parsedResult)) return false;

    while (nextToken.type == PLUS || nextToken.type == MINUS) {
        Match(nextToken.type, parsedResult);  // 消耗 '+' 或 '-'
        if (!maybe_mult_exp(parsedResult)) return false;
    }

    return true;
}


bool Parser::maybe_mult_exp(Token &parsedResult) {
    if (!unary_exp(parsedResult)) return false;
    return rest_of_maybe_mult_exp(parsedResult);  // ?理后?的乘法、除法或取模操作
}


bool Parser::rest_of_maybe_mult_exp(Token &parsedResult) {
    while (nextToken.type == MUL || nextToken.type == DIV || nextToken.type == MOD) {
        Match(nextToken.type, parsedResult);  // 消耗 '*', '/', 或 '%'
        if (!unary_exp(parsedResult)) return false;
    }
    return true;  // ?部分可以?空，因此默?返回 true
}

bool Parser::unary_exp(Token &parsedResult) {
    if (nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT) {
        while (nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == NOT) {
            sign(parsedResult);  // ?理所有前置的符?
        }
        return signed_unary_exp(parsedResult);
    } else if (nextToken.type == IDENTIFIER || nextToken.type == CONSTANT || nextToken.type == LPAREN) {
        return unsigned_unary_exp(parsedResult);
    } else if (nextToken.type == PP || nextToken.type == MM) {
        Match(nextToken.type, parsedResult); // 消耗 PP 或 MM
        if (Match(IDENTIFIER, parsedResult)) {
            if (nextToken.type == LBRACKET) {
                Match(LBRACKET, parsedResult); // 消耗 '['
                if (!expression(parsedResult)) return false; 
                if (!Match(RBRACKET, parsedResult)) return false; // 消耗 ']'
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
            Match(LPAREN, parsedResult); // 消耗 '('
            if (nextToken.type != RPAREN) { // 存在??列表
                if (!actual_parameter_list(parsedResult)) return false;
            }
            if (!Match(RPAREN, parsedResult)) return false; // 消耗 ')'
        } else if (nextToken.type == LBRACKET) {
            Match(LBRACKET, parsedResult); // 消耗 '['
            if (!expression(parsedResult)) return false;
            if (!Match(RBRACKET, parsedResult)) return false; // 消耗 ']'
        }
        return true;
    } else if (nextToken.type == CONSTANT) {
        return Match(CONSTANT, parsedResult);
    } else if (nextToken.type == LPAREN) {
        Match(LPAREN, parsedResult); // 消耗 '('
        if (!expression(parsedResult)) return false;
        return Match(RPAREN, parsedResult); // 消耗 ')'
    }
    return false;
}

bool Parser::unsigned_unary_exp(Token &parsedResult) {
    if (nextToken.type == IDENTIFIER) {
        Match(IDENTIFIER, parsedResult);
        if (nextToken.type == LPAREN) {
            Match(LPAREN, parsedResult); // 消耗 '('
            if (nextToken.type != RPAREN) { // 存在??列表
                if (!actual_parameter_list(parsedResult)) return false;
            }
            if (!Match(RPAREN, parsedResult)) return false; // 消耗 ')'
        } else if (nextToken.type == LBRACKET) {
            Match(LBRACKET, parsedResult); // 消耗 '['
            if (!expression(parsedResult)) return false;
            if (!Match(RBRACKET, parsedResult)) return false; // 消耗 ']'
            if (nextToken.type == PP || nextToken.type == MM) {
                Match(nextToken.type, parsedResult); // ?理后置的 PP 或 MM
            }
        }
        return true;
    } else if (nextToken.type == CONSTANT) {
        return Match(CONSTANT, parsedResult);
    } else if (nextToken.type == LPAREN) {
        Match(LPAREN, parsedResult); // 消耗 '('
        if (!expression(parsedResult)) return false;
        return Match(RPAREN, parsedResult); // 消耗 ')'
    }
    return false;
}

