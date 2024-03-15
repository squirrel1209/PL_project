#include <iostream>
#include <string>
using namespace std;

// 確保將之前的 isIDENT 函數定義在這裡
    bool isIDENT( string str ) {
        if ( str.empty() ) // 空字串不是合法的標識符
            return false;

        if ( !isalpha( str[0] ) ) // 第一個字元必須是字母
            return false;

        if ( str == "quit" ) // 檢查是否為 "quit"
            return false;

        for ( size_t i = 1; i < str.length(); i++ ) {
            if ( !isalnum( str[i] ) && str[i] != '_' ) // 其餘字元必須是數字、字母或底線
                return false;
        } // end for

        return true;
    } // end isIDENT

int main() {
    // 測試案例
    string testCases[] = {"bcd", "_ident", "2ndIdent", "quit", "alpha123", "a_", "A1_"};
    bool expectedResults[] = {true, false, false, false, true, true, true};

    for (int i = 0; i < sizeof(testCases) / sizeof(string); i++) {
        bool result = isIDENT(testCases[i]);
        cout << "測試 \"" << testCases[i] << "\": ";
        if (result == expectedResults[i]) {
            cout << "OK" << endl;
        } else {
            cout << "失敗" << endl;
        }
    }

    return 0;
}

