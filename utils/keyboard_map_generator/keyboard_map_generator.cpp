#include "define.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

const vector<short> KEY_LIST = {
    0x08, 0x09, 0x0C, 0x0D, 0x10, 0x11, 0x12, 0x13, 0x14, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
    0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 
    0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x90, 0x91, 0xBA, 
    0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xDB, 0xDC, 0xDD, 0xDE};
short formalKey = 0;

string toHexString(short key) {
    string res = "";
    char s[2];
    sprintf(s, "%02x", key);
    res.append(s);
    return res;
}

string waitAKey() {
    bool isEnd = false;

    while (!isEnd) {
        for (int i = 0; i < KEY_LIST.size(); i++) {
            if (GetAsyncKeyState(KEY_LIST[i]) & 0x8000) {
                if (KEY_LIST[i] == 0x08) {
                    isEnd = true;
                    break;
                }
                if (formalKey == KEY_LIST[i]) {
                    continue;
                } else {
                    // printf("Get %02x!\n", KEY_LIST[i]);
                    formalKey = KEY_LIST[i];
                    return toHexString(KEY_LIST[i]);
                }
            }
        }
    }
}

int main() {
    fstream file;
    bool isEnd = false;
    int no = 32;

    file.open("kb.ini", ios::out);
    printf("现在是地键映射生成时间，请根据提示输入对应按键的值，大小写不限。\n\
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n\
| 31 | 29 | 27 | 25 | 23 | 21 | 19 | 17 | 15 | 13 | 11 |  9 |  7 |  5 |  3 |  1 |\n\
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n\
| 32 | 30 | 28 | 26 | 24 | 22 | 20 | 18 | 16 | 14 | 12 | 10 |  8 |  6 |  4 |  2 |\n\
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n\n");
    file << "[slider]" << endl;
    while (no >= 1) {
        cout << "cell" << no << "=";
        string str = "cell";
        str.append(to_string(no));
        str.append("=0x");
        string code = waitAKey();
        str.append(code);
        cout << "0x" << code << endl; 
        file << str << endl;
        no--;
    }
    
    file.close();
    return 0;
}