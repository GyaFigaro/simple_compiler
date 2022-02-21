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
	cout << "���������ѡ�" << endl;
	cout << "1.�ʷ����������ɷ��Ŵ���" << endl;
	cout << "2.�﷨���������ɲ���ʽ��" << endl;
	cout << "3.��������ͷ��루�����м���룩" << endl;
	cout << "4.Ŀ��������ɣ�" << endl;
	cout << "5.�˳���" << endl;
	while (1)
	{
		cin >> option;
		switch (option)
		{
		case 1:
			islexed = 0;
			isgramed = 0;
			cout << "������Դ�����ļ�����.txt��" << endl;
			cin >> filename;
			make_token_list(filename);
			list_printf(0);
			islexed = 1;
			break;
		case 2:
			if (!islexed)
			{
				cout << "���Ƚ��дʷ�����!" << endl;
				break;
			}
			grammar_midcode();
			isgramed = 1;
			break;
		case 3:
			if (!islexed)
			{
				cout << "���Ƚ��дʷ�����!" << endl;
				break;
			}
			if (!isgramed)
			{
				cout << "���Ƚ����﷨����!" << endl;
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
				cout << "���Ƚ��дʷ�����!" << endl;
				break;
			}
			if (!isgramed)
			{
				cout << "���Ƚ����﷨����!" << endl;
				break;
			}
			make_aidcode();
			cout << "MAKE AID_CODE SUCCEED!" << endl;
			break;
		case 5:
			return 0;
			break;
		default:
			cout << "��������!" << endl;
			break;
		}
	}
	return 0;
}