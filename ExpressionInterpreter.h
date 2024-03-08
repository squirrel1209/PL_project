#ifndef EXPRESSION_INTERPRETER_H
#define EXPRESSION_INTERPRETER_H

#include <iostream>
#include <vector>
#include "Parser.h" // ���] Parser ���w�g�Q�w�q�b Parser.h ��

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

