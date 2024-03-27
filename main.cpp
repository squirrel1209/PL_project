# include <iostream>
# include <vector>
# include <cctype> // �B�z�r��  
# include <string>
# include <fstream>
# include "Tokenizer.h"
//# include "Parser.h"
 
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


int main() {
  vector<string> input = read_lines_from_file("input.txt");
  Tokenizer tokenizer(input);
  vector<Token> tokenGroup = tokenizer.ProcessTokens() ;
  
    for ( int i = 0 ; i < tokenGroup.size() ; i++ ) {
        cout << "line:" << tokenGroup[i].line << " token:" << tokenGroup[i].tokenName << " type:" << tokenGroup[i].type;
        if ( tokenGroup[i].type == ERROR )
            cout << "ErrorName:"  << tokenGroup[i].tokenName << endl ;
        cout << endl ;
    }
  /*
  cout << "Program starts..." << endl ;
  Parser parse( tokenGroup ) ;
  parse.Parse() ;
  cout << "Program exits..." << endl ;  
  */
} // end main()
