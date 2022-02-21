#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include <stack>
#include "lexer.h"
#include "trans.h"

int main()
{
	int option;
	int islexed = 0;
	int isgramed = 0;
	char filename[15];
	cout << "请输入编译选项：" << endl;
	cout << "1.词法分析（生成符号串）" << endl;
	cout << "2.语法分析（生成产生式）" << endl;
	cout << "3.语义分析和翻译（生成中间代码）" << endl;
	cout << "4.目标代码生成：" << endl;
	cout << "5.退出：" << endl;
	while (1)
	{
		cin >> option;
		switch (option)
		{
		case 1:
			islexed = 0;
			isgramed = 0;
			cout << "请输入源代码文件名（.txt）" << endl;
			cin >> filename;
			make_token_list(filename);
			list_printf(0);
			islexed = 1;
			break;
		case 2:
			if (!islexed)
			{
				cout << "请先进行词法分析!" << endl;
				break;
			}
			grammar_midcode();
			isgramed = 1;
			break;
		case 3:
			if (!islexed)
			{
				cout << "请先进行词法分析!" << endl;
				break;
			}
			if (!isgramed)
			{
				cout << "请先进行语法分析!" << endl;
				break;
			}
			cout << "MAKE MIDDLE_CODE SUCCEED!" << endl;
			link_make_addr();
			list_printf(1);
			cout << "MAKE SYMBOL_LIST SUCCEED!" << endl;
			break;
		case 4:
			if (!islexed)
			{
				cout << "请先进行词法分析!" << endl;
				break;
			}
			if (!isgramed)
			{
				cout << "请先进行语法分析!" << endl;
				break;
			}
			make_aidcode();
			cout << "MAKE AID_CODE SUCCEED!" << endl;
			break;
		case 5:
			return 0;
			break;
		default:
			cout << "错误输入!" << endl;
			break;
		}
	}
	return 0;
}