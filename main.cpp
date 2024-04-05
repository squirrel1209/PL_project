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


string TypeToString(Type type) {
    switch (type) {
        case QUIT: return "QUIT";
        case ERROR: return "ERROR";
        case IDENTIFIER: return "IDENTIFIER";
        case CONSTANT: return "CONSTANT";
        case INT: return "INT";
        case FLOAT: return "FLOAT";
        case CHAR: return "CHAR";
        case BOOL: return "BOOL";
        case STRING: return "STRING";
        case VOID: return "VOID";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case DO: return "DO";
        case RETURN: return "RETURN";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACKET: return "LBRACKET";
        case RBRACKET: return "RBRACKET";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case ASTERISK: return "ASTERISK";
        case SLASH: return "SLASH";
        case PERCENT: return "PERCENT";
        case CARET: return "CARET";
        case GT: return "GT";
        case LT: return "LT";
        case GE: return "GE";
        case LE: return "LE";
        case EQ: return "EQ";
        case NEQ: return "NEQ";
        case AMPERSAND: return "AMPERSAND";
        case PIPE: return "PIPE";
        case ASSIGN: return "ASSIGN";
        case EXCLAMATION: return "EXCLAMATION";
        case AND: return "AND";
        case OR: return "OR";
        case PE: return "PE";
        case ME: return "ME";
        case TE: return "TE";
        case DE: return "DE";
        case RE: return "RE";
        case PP: return "PP";
        case MM: return "MM";
        case RS: return "RS";
        case LS: return "LS";
        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        case QUESTION: return "QUESTION";
        case COLON: return "COLON";
        default: return "UNKNOWN";
    }
}


int main() {
  vector<string> input = read_lines_from_file("input.txt");
  Tokenizer tokenizer(input);
  /*
  vector<Token> tokenGroup = tokenizer.ProcessTokens() ;
    for ( int i = 0 ; i < tokenGroup.size() ; i++ ) {
        cout << "line:" << tokenGroup[i].line << " token:" << tokenGroup[i].tokenName << " type:" << TypeToString(tokenGroup[i].type);
        if ( tokenGroup[i].type == ERROR )
            cout << "ErrorName:"  << tokenGroup[i].tokenName << endl ;
        cout << endl ;
    }
  */
  cout << "Program starts..." << endl ;
  Parser parse( tokenizer ) ;
  parse.Parse() ;
  cout << "Program exits..." << endl ;  
  
} // end main()
