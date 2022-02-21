#pragma once
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "hash.h"

using namespace std;

typedef struct token
{
	int  code = 0;
	string value = "\0";
}token;

extern list table[26];

void init_table();

int iskeyword(string s);

//扫描指针前进
void go_forward(char& ch);

//扫描输入缓冲区（即源程序string）
token token_scan();

//将原程序转化为string形式
void get_program(char* file);

//符号表打印
void list_printf(int n);

//符号表更新
void link_update(string n, string type);

void link_make_addr();
