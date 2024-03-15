#include <iostream>
#include <string>
using namespace std;

// �T�O�N���e�� isIDENT ��Ʃw�q�b�o��
    bool isIDENT( string str ) {
        if ( str.empty() ) // �Ŧr�ꤣ�O�X�k�����Ѳ�
            return false;

        if ( !isalpha( str[0] ) ) // �Ĥ@�Ӧr�������O�r��
            return false;

        if ( str == "quit" ) // �ˬd�O�_�� "quit"
            return false;

        for ( size_t i = 1; i < str.length(); i++ ) {
            if ( !isalnum( str[i] ) && str[i] != '_' ) // ��l�r�������O�Ʀr�B�r���Ω��u
                return false;
        } // end for

        return true;
    } // end isIDENT

int main() {
    // ���ծר�
    string testCases[] = {"bcd", "_ident", "2ndIdent", "quit", "alpha123", "a_", "A1_"};
    bool expectedResults[] = {true, false, false, false, true, true, true};

    for (int i = 0; i < sizeof(testCases) / sizeof(string); i++) {
        bool result = isIDENT(testCases[i]);
        cout << "���� \"" << testCases[i] << "\": ";
        if (result == expectedResults[i]) {
            cout << "OK" << endl;
        } else {
            cout << "����" << endl;
        }
    }

    return 0;
}

