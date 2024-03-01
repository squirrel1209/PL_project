#include <iostream>
#include <vector>
#include <cctype> // 處理字符  
#include <string>
#include <fstream>
#include "Tokenizer.h"

using namespace std;

// 函數用於從文件中讀取內容並返回一個存儲行的向量
vector<string> read_lines_from_file( string filename ) {
    vector<string> input_lines ;
    ifstream file(filename) ;                            // 打開文件
    
    if ( file.is_open() ) {
        string line;         
        while ( getline( file, line ) ) {               // 逐行讀取文件內容
            input_lines.push_back( line );
        } // end while
         
        file.close();                                  // 關閉文件
    } // end if
    
    else cerr << "Unable to open file: " << filename << endl;
    return input_lines;
} // end read_lines_from_file

void printTokenType(TokenType type) {
    switch (type) {
        case TokenType::IDENT:
            std::cout << "Identifier\n";
            break;
        case TokenType::NUM:
            std::cout << "Number\n";
            break;
        case TokenType::SIGN:
            std::cout << "Sign\n";
            break;
        case TokenType::OTHER:
            std::cout << "Other\n";
            break;
        default:
            std::cout << "Unknown\n";
            break;
    }
}

int main() {
    vector<string> input = read_lines_from_file("input.txt");
    Tokenizer tokenizer(input);
    vector<TokenWithType> tokenGroup = tokenizer.processTokens() ;
    
    for ( int i = 0 ; i < tokenGroup.size() ; i++ ) {
        cout << "token:" << tokenGroup[i].token << " type:" ;
        printTokenType( tokenGroup[i].type ) ;
        cout << endl ;
    } 
    
}
