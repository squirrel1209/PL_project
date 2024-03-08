#include <iostream>
#include <vector>
#include <cctype> // �B�z�r��  
#include <string>
#include <fstream>
#include "Tokenizer.h"
#include "Parser.h"
 
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

void printTokenType(Type type) {
    switch (type) {
        case Type::INT:
            std::cout << "INT\n";
            break;
        case Type::FLOAT:
            std::cout << "FLOAT\n";
            break;

        case Type::IDENT:
            std::cout << "Identifier\n";
            break;
        case Type::NUM:
            std::cout << "Number\n";
            break;
        case Type::SIGN:
            std::cout << "Sign\n";
            break;
        case Type::RPAREN:
            std::cout << "RPAREN\n";
            break;
        case Type::LPAREN:
            std::cout << "LPAREN\n";
            break;
        case Type::OTHER:
            std::cout << "Other\n";
            break;
        case Type::QUIT:
            std::cout << "QUIT\n";
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

/*
    Parser parse( tokenGroup ) ;
    try {
        parse.parse() ;
    } 
    catch (runtime_error e) {
        cerr << e.what() << std::endl;
    }
*/

    for ( int i = 0 ; i < tokenGroup.size() ; i++ ) {
        cout << "token:" << tokenGroup[i].token << " type:" ;
        printTokenType( tokenGroup[i].type ) ;
        cout << endl ;
    } 

}
