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
    ifstream file(filename.c_str()) ;                            // ���}���
    
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
        case INT:
            std::cout << "INT\n";
            break;
        case FLOAT:
            std::cout << "FLOAT\n";
            break;

        case IDENT:
            std::cout << "Identifier\n";
            break;
            
        case SIGN:
            std::cout << "Sign\n";
            break;
        case RPAREN:
            std::cout << "RPAREN\n";
            break;
        case LPAREN:
            std::cout << "LPAREN\n";
            break;
        case ERROR:
        	  std::cout << "ERROR\n";
            break;
        case QUIT:
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
    vector<Token> tokenGroup = tokenizer.processTokens() ;
    
    for ( int i = 0 ; i < tokenGroup.size() ; i++ ) {
        cout << "token:" << tokenGroup[i].tokenName << " type:" << tokenGroup[i].type;
        if ( tokenGroup[i].type == ERROR )
            cout << "ErrorName:"  << tokenGroup[i].error.errorValue << endl ;
        cout << endl ;
    } 

    
    Parser parse( tokenGroup ) ;

         parse.parse() ;

    
    

    
}
