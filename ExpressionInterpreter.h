#ifndef EXPRESSION_INTERPRETER_H
#define EXPRESSION_INTERPRETER_H

#include <iostream>
#include <vector>
#include "Parser.h" // 假設 Parser 類已經被定義在 Parser.h 中

struct Grammer {
    vector<TokenWithType> tokens;
    
};

class ExpressionInterpreter {
private:
    Parser parser;

    int evaluateArithmeticExpression( vector<Token>& tokens );
    bool evaluateBooleanExpression( vector<Token>& tokens );

public:
    void interpretExpression( vector<Token>& tokens );
};

#endif /* EXPRESSION_INTERPRETER_H */

