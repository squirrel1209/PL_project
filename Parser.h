#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
//#include <stdexcept>

using namespace std;

enum class TokenType {
    IDENT,
    NUM,
    SIGN,
    OTHER
};

struct TokenWithType {
    string token;
    TokenType type;
};

class Parser {
private:
    vector<TokenWithType> tokens;
    int currentTokenIndex;

public:
    Parser( vector<TokenWithType> inputTokens ): tokens(inputTokens), currentTokenIndex(0) {}
	
    void parse() {  // 呼叫此function作為開始 
        command();
    } // end parse
};

#endif // PARSER_H
