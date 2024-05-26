# include <iostream>
# include <vector>
# include <cctype> // 處理字符  
# include <string>
# include <fstream>
# include "Tokenizer.h"
# include "Parser.h"
 
using namespace std;

// 函數用於從文件中讀取內容並返回一個存儲行的向量
vector<string> read_lines_from_file( string filename ) {

  vector<string> input_lines ;
  ifstream file(filename.c_str()) ;                            // 打開文件
    
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


int main() {
  vector<string> input = read_lines_from_file("input.txt");
  InitializegTypeNameMap();

  /*
  vector<Token> tokenGroup = tokenizer.ProcessTokens() ;
    for ( int i = 0 ; i < tokenGroup.size() ; i++ ) {
        cout << "line:" << tokenGroup[i].line << " token:" << tokenGroup[i].tokenName << " type:" << TypeToString(tokenGroup[i].type);
        if ( tokenGroup[i].type == ERROR )
            cout << "ErrorName:"  << tokenGroup[i].tokenName << endl ;
        cout << endl ;
    }
  */
  cout << "Our-C running ..." << endl ;
  
  Parser parse ;
  parse.Parse() ;
  cout << "> Our-C exited ..." << endl ; 
  
} // end main()
