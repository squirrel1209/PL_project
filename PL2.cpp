#include <iostream>
#include <vector>
#include <cctype> // �B�z�r��  
#include <string>
#include <fstream>
#include "Tokenizer.h"

using namespace std;

// ��ƥΩ�q���Ū�����e�ê�^�@�Ӧs�x�檺�V�q
vector<string> read_lines_from_file( string filename ) {
    vector<string> input_lines ;
    ifstream file(filename) ;                            // ���}���
    
    if ( file.is_open() ) {
        string line;         
        while ( getline( file, line ) ) {               // �v��Ū����󤺮e
            input_lines.push_back( line );
        } // end while
         
        file.close();                                  // �������
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
