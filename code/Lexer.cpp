#include "lexer.h"

list table[26];
string program;
int cur = 0;

string keyWord[40] =
{
   "begin",
   "bool",
   "break",
   "case",
   "char",
   "const",
   "continue",
   "default",
   "do",
   "double",
   "else",
   "end",
   "float",
   "for",
   "goto",
   "if",
   "int",
   "long",
   "main",
   "return",
   "short",
   "signed",
   "sizeof",
   "static",
   "string",
   "struct",
   "switch",
   "typedef",
   "unsigned",
   "until",
   "void",
   "while",
};

ofstream en_symbol_Out("entended_symbol.txt");
ofstream pr_symbol_Out("pre_symbol.txt");

void init_table()
{
    for(int i = 0; i < 26; i++)
    {
        table[i] = new Node;
        table[i]->name = "0";
        table[i]->next = NULL;
    }
}

int iskeyword(string s)
{
	for (int i = 0; i < 32; i++)
	{
		if (keyWord[i] == s)
			return i + 5;
	}
	return 0;
}

void go_forward(char &ch)
{
	cur++;
	ch = program[cur];
	//cout << cur << ch << endl;
}

token token_scan()
{
	int start,num,addr;
	char ch;
	string gettoken;
	token t;
    ch = program[cur];
    //cout << cur << ch << endl;
	while (ch == ' '||ch == '\n'||ch == '\t')
    {
        go_forward(ch);
    }
    if (cur >= program.size())
	{
		t.code = 0;
		return t;
	}
	if (isalpha(ch)||ch == '_')
	{
		start = cur;
		go_forward(ch);
		while (isalpha(ch)||isdigit(ch)||ch == '_')
			go_forward(ch);
		gettoken = program.substr(start, cur - start);
		num = iskeyword(gettoken);
		if (num > 0)
		{
			t.code = num;
		}
		else
		{
			t.code = 1;
            addr = gettoken[0] % 26;
			if (!link_search(gettoken,table[addr]))
			{
				link_insert(gettoken,table[addr]);
			}
		}
		t.value = gettoken;
	}
	else if (isdigit(ch))
	{
		start = cur;
		go_forward(ch);
		while (isdigit(ch))
			go_forward(ch);
		gettoken = program.substr(start, cur - start);
		t.code = 2;
		t.value = gettoken;
	}
	else
	{
		switch (ch) {
		    case '=':
				go_forward(ch);
			    if (ch == '='){
					cur++;
				    t.code = 41;
				    t.value = "==";
			    }
			    else
			    {
				    t.code = 40;
				    t.value = "=";
			    }
				break;
			case '+':
				go_forward(ch);
				if (ch == '=')
				{
					cur++;
					t.code = 43;
					t.value = "+=";
				}
				else if (ch == '+')
				{
					cur++;
					t.code = 44;
					t.value = "++";
				}
				else
				{
					t.code = 42;
					t.value = "+";
				}
				break;
			case '-':
				go_forward(ch);
				if (ch == '=')
				{
					cur++;
					t.code = 46;
					t.value = "-=";
				}
				else if (ch == '-')
				{
					cur++;
					t.code = 47;
					t.value = "--";
				}
				else if (ch == '>')
				{
					cur++;
					t.code = 48;
					t.value = "->";
				}
				else
				{
					t.code = 45;
					t.value = "-";
				}
				break;
			case '*':
				go_forward(ch);
				if (ch == '=') {
					cur++;
					t.code = 50;
					t.value = "*=";
				}
				else
				{
					t.code = 49;
					t.value = "*";
				}
				break;
			case '/':
				go_forward(ch);
				if (ch == '=') {
					cur++;
					t.code = 52;
					t.value = "/=";
				}
				else
				{
					t.code = 51;
					t.value = "/";
				}
				break;
			case '%':
				go_forward(ch);
				if (ch == '=') {
					cur++;
					t.code = 54;
					t.value = "%=";
				}
				else
				{
					t.code = 53;
					t.value = "%";
				}
				break;
			case '!':
				go_forward(ch);
				if (ch == '=') {
					cur++;
					t.code = 56;
					t.value = "!=";
				}
				else
				{
					t.code = 55;
					t.value = "!";
				}
				break;
			case '^':
				go_forward(ch);
				if (ch == '=') {
					cur++;
					t.code = 58;
					t.value = "^=";
				}
				else
				{
					t.code = 57;
					t.value = "^";
				}
				break;
			case '&':
				go_forward(ch);
				if (ch == '=')
				{
					cur++;
					t.code = 60;
					t.value = "&=";
				}
				else if (ch == '-')
				{
					cur++;
					t.code = 61;
					t.value = "&&";
				}
				else
				{
					t.code = 59;
					t.value = "&";
				}
				break;
			case '|':
				go_forward(ch);
				if (ch == '=')
				{
					cur++;
					t.code = 63;
					t.value = "|=";
				}
				else if (ch == '-')
				{
					cur++;
					t.code = 64;
					t.value = "||";
				}
				else
				{
					t.code = 62;
					t.value = "|";
				}
				break;
			case ':':
				go_forward(ch);
				if (ch == ':') {
					cur++;
					t.code = 66;
					t.value = "::";
				}
				else
				{
					t.code = 65;
					t.value = ":";
				}
				break;
			case '<':
				go_forward(ch);
				if (ch == '<')
				{
					go_forward(ch);
					if (ch == '=')
					{
						cur++;
						t.code = 70;
						t.value = "<<=";
					}
					else
					{
						t.code = 69;
						t.value = "<<";
					}
				}
				else if (ch == '=')
				{
					cur++;
					t.code = 68;
					t.value = "<=";
				}
				else
				{
					t.code = 67;
					t.value = "<";
				}
				break;
			case '>':
				go_forward(ch);
				if (ch == '>')
				{
					go_forward(ch);
					if (ch == '=')
					{
						cur++;
						t.code = 74;
						t.value = ">>=";
					}
					else
					{
						t.code = 73;
						t.value = ">>";
					}
				}
				else if (ch == '=')
				{
					cur++;
					t.code = 72;
					t.value = ">=";
				}
				else
				{
					t.code = 71;
					t.value = ">";
				}
				break;
			case '[':
				cur++;
				t.code = 75;
				t.value = "[";
				break;
			case ']':
				cur++;
				t.code = 76;
				t.value = "]";
				break;
			case '{':
				cur++;
				t.code = 77;
				t.value = "{";
				break;
			case '}':
				cur++;
				t.code = 78;
				t.value = "}";
				break;
			case '\'':
				cur++;
				t.code = 79;
				t.value = "\'";
				break;
			case '(':
				cur++;
				t.code = 80;
				t.value = "(";
				break;
			case ')':
				cur++;
				t.code = 81;
				t.value = ")";
				break;
			case '#':
				cur++;
				t.code = 82;
				t.value = "#";
				break;
			case ',':
				cur++;
				t.code = 83;
				t.value = ",";
				break;
			case '.':
				cur++;
				t.code = 84;
				t.value = ".";
				break;
			case '?':
				cur++;
				t.code = 85;
				t.value = "?";
				break;
			case ';':
				cur++;
				t.code = 86;
				t.value = ";";
				break;
			case '~':
				cur++;
				t.code = 87;
				t.value = "~";
				break;
			case '"':
			    gettoken += ch;
				do
                {
                    go_forward(ch);
                    gettoken += ch;
                }while(ch != '\"');
                cur++;
				t.code = 4;
				t.value = gettoken;
				break;
			default:
				t.code = -1;
				break;
		}
	}
	return t;
}

void get_program(char* file)
{
	ifstream fin(file);
	if (!fin) {
		cerr << file << " ERROR!" << endl;
		exit(2);
	}
	program.clear();
	string line;
	while (getline(fin, line)) {
		program += line + '\n';
	}
	//cout << program << endl;
}

void list_printf(int n)
{
	list p;
	for (int i = 0; i < 26; i++)
	{
		p = table[i];
		while (p != NULL)
		{
			if (p->name != "0")
			{
				if (n == 1)
					en_symbol_Out << "(" << p->name << "," << p->type << "," << p->addr << ")" << endl;
				else
					pr_symbol_Out << "(" << p->name << "," << "-1" << "," << "?" << ")" << endl;
			}
			p = p->next;
		}
	}
}

void link_update(string n, string type)
{
	list p = table[n[0] % 26];
	while (p != NULL)
	{
		if (p->name == n)
		{
			p->type = type;
			return;
		}
		else p = p->next;
	}
}

void link_make_addr()
{
	list p;
	int addr = 0;
	for (int i = 0; i < 26; i++)
	{
		p = table[i];
		while (p != NULL)
		{
			if (p->name != "0")
			{
				p->addr = addr;
				addr += 4;
			}
			p = p->next;
		}
	}
}