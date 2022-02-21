#pragma once
#include<string>
using namespace std;

typedef struct Node
{
	string name = "\0";
	string type = "\0";
	int addr = 0;
	Node *next;
}*list;

void link_insert(string n, list p);

bool link_search(string n, list p);



