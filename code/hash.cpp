#include"hash.h"
#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;

void link_insert(string n, list p)
{
	list a;
	if (p->name == "0")
    {
        p->name = n;
		p->next = NULL;
    }
	else {
        a = new Node;
        a->name = n;
        a->next = NULL;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = a;
	}
}

bool link_search(string n, list p)
{
	while (p != NULL)
	{
		if (p->name == n)
			return true;
		else p = p->next;
	}
	return false;
}
