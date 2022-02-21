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

using namespace std;
int pos = 0;
bool status_goto = true;

ofstream pro_out("productions_out.txt");
ofstream midcode_out("midcode.txt");
stack<attr> symbol;
stack<int> state;
attr LR1_list[100];
quad quads[500];
int quad_len = 0;
int t_num = 0;
int r_num = 0;
string t_temp[9] = { "t1","t2","t3","t4","t5","t6","t7","t8","t9" };
string reg[10] = { "R0","R1","R2","R3","R4","R5","R6","R7","R8","R9" };
string r_temp[10];
string production[20] = {
    "0",
    "G' -> S'",
    "S' -> key id",
    "S' -> id = E",
    "S' -> id ( id )",
    "S' -> id ( int_num )",
    "S' -> id ( char_str )",
    "E -> E + A",
    "E -> E - A",
    "E -> A",
    "A -> A * B",
    "A -> A / B",
    "A -> B",
    "B -> ( E )",
    "B -> id",
    "B -> int_num",
};

void init_stack()
{
    attr bottom;
    bottom.sym = "$";
    while (!state.empty())
        state.pop();
    while (!symbol.empty())
        symbol.pop();
    state.push(0);
    symbol.push(bottom);
    return;
}

int load_list()
{
    ifstream fin("token.txt");
    if (!fin) {
        cerr << "token.txt" << " ERROR!" << endl;
        exit(2);
    }
    int len = 0;
    int k = 1;
    int curtype;
    string curname;
    string token;
    while (getline(fin, token)) {
        curtype = 0;
        curname = "\0";
        k = 1;
        while (token[k] != ',')
            curtype = curtype * 10 + token[k++] - 48;
        k++;
        while (token[k] != ')')
            curname = curname + token[k++];
        if (curtype == 81) curname = ")";
        //cout << "curname: " << curname << endl;
        if (curtype == 1)
        {
            LR1_list[len].sym = "id";
            LR1_list[len++].addr = curname;
        }
        else if (curtype == 2)
        {
            LR1_list[len].sym = "int_num";
            LR1_list[len].type = "int";
            LR1_list[len].addr = curname;
            LR1_list[len++].val = curname;
        }        
        else if (curtype == 4)
        {
            LR1_list[len].sym = "char_str";
            LR1_list[len].type = "char";
            LR1_list[len].addr = curname;
            LR1_list[len++].val = curname;
        }
            
        else if (curtype < 40 && curtype > 4)
        {
            LR1_list[len].sym = "key";
            LR1_list[len++].type = curname;
        }
        else switch (curtype)
        {
        case 40:
            LR1_list[len++].sym = "=";
            break;
        case 42:
            LR1_list[len++].sym = "+";
            break;
        case 45:
            LR1_list[len++].sym = "-";
            break;
        case 49:
            LR1_list[len++].sym = "*";
            break;
        case 51:
            LR1_list[len++].sym = "/";
            break;
        case 77:
            LR1_list[len++].sym = "{";
            break;
        case 78:
            LR1_list[len++].sym = "{";
            break;
        case 80:
            LR1_list[len++].sym = "(";
            break;
        case 81:
            LR1_list[len++].sym = ")";
            break;
        case 86:
            LR1_list[len++].sym = "$";
            break;
        default:
            LR1_list[len++].sym = "ERROR";
            break;
        }
    }
    return len;
}

void make_code(string op, string r1, string r2, string t)
{
    quads[quad_len].op = op;
    quads[quad_len].r1 = r1;
    quads[quad_len].r2 = r2;
    quads[quad_len].t = t;
    if (op != "assign")
    {
        midcode_out << quads[quad_len].t << "=" << quads[quad_len].r1 << quads[quad_len].op;
        midcode_out << quads[quad_len].r2 << ";" << endl;
    }
    else
    {
        midcode_out << quads[quad_len].t << "=" << quads[quad_len].r1 << ";" << endl;
    }
    quad_len++;
}

bool goto_state(int top_state, string top_sym)
{
    //cout << top_state << " " << top_sym << endl;
    switch (top_state)
    {
    case 0:
        if (top_sym == "S'")
            state.push(1);
        else return false;
        break;
    case 5:
        switch (top_sym[0])
        {
        case 'A': state.push(7); break;
        case 'B': state.push(8); break;
        case 'E': state.push(9); break;
        default: return false; break;
        }
        break;
    case 12:
        switch (top_sym[0])
        {
        case 'A': state.push(20); break;
        case 'B': state.push(21); break;
        case 'E': state.push(22); break;
        default: return false; break;
        }
        break;
    case 16:
        if (top_sym == "B")
            state.push(29);
        else return false;
        break;
    case 17:
        if (top_sym == "B")
            state.push(30);
        else return false;
        break;
    case 18:
        switch (top_sym[0])
        {
        case 'A': state.push(31); break;
        case 'B': state.push(8); break;
        default: return false; break;
        }
        break;
    case 19:
        switch (top_sym[0])
        {
        case 'A': state.push(32); break;
        case 'B': state.push(8); break;
        default: return false; break;
        }
        break;
    case 25:
        switch (top_sym[0])
        {
        case 'A': state.push(20); break;
        case 'B': state.push(21); break;
        case 'E': state.push(38); break;
        default: return false; break;
        }
        break;
    case 33:
        if (top_sym == "B")
            state.push(39);
        else return false;
        break;
    case 34:
        if (top_sym == "B")
            state.push(40);
        else return false;
        break;
    case 35:
        switch (top_sym[0])
        {
        case 'A': state.push(41); break;
        case 'B': state.push(21); break;
        default: return false; break;
        }
        break;
    case 36:
        switch (top_sym[0])
        {
        case 'A': state.push(42); break;
        case 'B': state.push(21); break;
        default: return false; break;
        }
        break;
    default:
        return false;
        break;
    }
    return true;
}

void shift(int next_state)
{
    //cout << "shift " << next_state << endl;
    state.push(next_state);
    symbol.push(LR1_list[pos]);
    pos++;
    return;
}

void reduce(int pro_code)
{
    //cout << "reduce " << pro_code << endl;
    attr top;
    attr newtop;
    attr temp;
    int t;
    switch (pro_code)
    {
    case 1:
        top = symbol.top();
        newtop.sym = "G'";
        symbol.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 2:
        top = symbol.top();
        symbol.pop();
        temp = symbol.top();
        symbol.pop();
        link_update(top.addr, temp.type);
        newtop.sym = "S'";
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 3:
        top = symbol.top();
        symbol.pop();
        symbol.pop();    
        temp = symbol.top();
        symbol.pop();
        newtop.sym = "S'";
        make_code("assign", top.addr, "\0", temp.addr);
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 4:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        symbol.pop();
        symbol.pop();
        newtop.sym = "S'";
        state.pop();
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 5:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        symbol.pop();
        symbol.pop();
        newtop.sym = "S'";
        state.pop();
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 6:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        symbol.pop();
        symbol.pop();
        newtop.sym = "S'";
        state.pop();
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 7:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        temp = symbol.top();
        symbol.pop();
        newtop.sym = "E";
        newtop.addr = t_temp[t_num++];
        newtop.type = temp.type;
        newtop.intval = top.intval + temp.intval;
        make_code("+", temp.addr, top.addr, newtop.addr);
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 8:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        temp = symbol.top();
        symbol.pop();
        newtop.sym = "E";
        newtop.addr = t_temp[t_num++];
        newtop.type = top.type;
        newtop.intval = top.intval + temp.intval;
        make_code("-", temp.addr, top.addr, newtop.addr);
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 9:
        top = symbol.top();
        symbol.pop();
        newtop.sym = "E";
        newtop.addr = top.addr;
        newtop.type = top.type;
        newtop.intval = top.intval;
        state.pop();
        symbol.push(newtop);
        break;
    case 10:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        temp = symbol.top();
        symbol.pop();
        newtop.sym = "A";
        newtop.addr = t_temp[t_num++];
        newtop.type = top.type;
        newtop.intval = top.intval + temp.intval;
        make_code("*", temp.addr, top.addr, newtop.addr);
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 11:
        top = symbol.top();
        symbol.pop();
        symbol.pop();
        temp = symbol.top();
        symbol.pop();
        newtop.sym = "A";
        newtop.addr = t_temp[t_num++];
        newtop.type = top.type;
        newtop.intval = top.intval + temp.intval;
        make_code("/", temp.addr, top.addr, newtop.addr);
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 12:
        top = symbol.top();
        symbol.pop();
        newtop.sym = "A";
        newtop.addr = top.addr;
        newtop.type = top.type;
        newtop.intval = top.intval;
        state.pop();
        symbol.push(newtop);
        break;
    case 13:
        top = symbol.top();
        symbol.pop();
        temp = symbol.top();
        symbol.pop();
        symbol.pop();
        newtop.sym = "B";
        newtop.addr = temp.addr;
        newtop.type = temp.type;
        newtop.intval = temp.intval;
        state.pop();
        state.pop();
        state.pop();
        symbol.push(newtop);
        break;
    case 14:
        top = symbol.top();
        symbol.pop();
        newtop.sym = "B";
        newtop.addr = top.addr;
        newtop.type = top.type;
        newtop.intval = top.intval;
        state.pop();
        symbol.push(newtop);
        break;
    case 15:
        top = symbol.top();
        symbol.pop();
        newtop.sym = "B";
        newtop.addr = top.addr;
        newtop.type = top.type;
        newtop.intval = top.intval;
        state.pop();
        symbol.push(newtop);
        break;
    default:
        break;
    }
    pro_out << production[pro_code] << endl;
    top = symbol.top();
    t = state.top();
    if (!goto_state(t, top.sym))
        status_goto = false;
    return;
}

bool action(int cur_state, string next_sym)
{
    //cout << cur_state << " " << next_sym << endl;
    if (cur_state == 1 && next_sym == "$")
    {
        pro_out << "accept" << endl;
        pos++;
        init_stack();
        return true;
    }
    else
        switch (cur_state)
        {

        case 0:
            if (next_sym == "key")
                shift(2);
            else if (next_sym == "id")
                shift(3);
            else
                return false;
            break;
        case 2:
            if (next_sym == "id")
                shift(4);
            else
                return false;
            break;
        case 3:
            if (next_sym == "=")
                shift(5);
            else if (next_sym == "(")
                shift(6);
            else
                return false;
            break;
        case 4:
            if (next_sym == "$")
                reduce(2);
            else
                return false;
            break;
        case 5:
            if (next_sym == "id")
                shift(10);
            else if (next_sym == "int_num")
                shift(11);
            else if (next_sym == "(")
                shift(12);
            else
                return false;
            break;
        case 6:
            if (next_sym == "id")
                shift(13);
            else if (next_sym == "int_num")
                shift(14);
            else if (next_sym == "char_str")
                shift(15);
            else
                return false;
            break;
        case 7:
            switch (next_sym[0])
            {
            case '+': reduce(9); break;
            case '-': reduce(9); break;
            case '*': shift(16); break;
            case '/': shift(17); break;
            case '$': reduce(9); break;
            default: return false; break;
            }
            break;
        case 8:
            switch (next_sym[0])
            {
            case '+': reduce(12); break;
            case '-': reduce(12); break;
            case '*': reduce(12); break;
            case '/': reduce(12); break;
            case '$': reduce(12); break;
            default: return false; break;
            }
            break;
        case 9:
            switch (next_sym[0])
            {
            case '+': shift(18); break;
            case '-': shift(19); break;
            case '$': reduce(3); break;
            default: return false; break;
            }
            break;
        case 10:
            switch (next_sym[0])
            {
            case '+': reduce(14); break;
            case '-': reduce(14); break;
            case '*': reduce(14); break;
            case '/': reduce(14); break;
            case '$': reduce(14); break;
            default: return false; break;
            }
            break;
        case 11:
            switch (next_sym[0])
            {
            case '+': reduce(15); break;
            case '-': reduce(15); break;
            case '*': reduce(15); break;
            case '/': reduce(15); break;
            case '$': reduce(15); break;
            default: return false; break;
            }
            break;
        case 12:
            if (next_sym == "id")
                shift(23);
            else if (next_sym == "int_num")
                shift(24);
            else if (next_sym == "(")
                shift(25);
            else
                return false;
            break;
        case 13:
            if (next_sym == ")")
                shift(26);
            else
                return false;
            break;
        case 14:
            if (next_sym == ")")
                shift(27);
            else
                return false;
            break;
        case 15:
            if (next_sym == ")")
                shift(28);
            else
                return false;
            break;
        case 16:
            if (next_sym == "id")
                shift(10);
            else if (next_sym == "int_num")
                shift(11);
            else if (next_sym == "(")
                shift(12);
            else
                return false;
            break;
        case 17:
            if (next_sym == "id")
                shift(10);
            else if (next_sym == "int_num")
                shift(11);
            else if (next_sym == "(")
                shift(12);
            else
                return false;
            break;
        case 18:
            if (next_sym == "id")
                shift(10);
            else if (next_sym == "int_num")
                shift(11);
            else if (next_sym == "(")
                shift(12);
            else
                return false;
            break;
        case 19:
            if (next_sym == "id")
                shift(10);
            else if (next_sym == "int_num")
                shift(11);
            else if (next_sym == "(")
                shift(12);
            else
                return false;
            break;
        case 20:
            switch (next_sym[0])
            {
            case '+': reduce(9); break;
            case '-': reduce(9); break;
            case '*': shift(33); break;
            case '/': shift(34); break;
            case ')': reduce(9); break;
            default: return false; break;
            }
            break;
        case 21:
            switch (next_sym[0])
            {
            case '+': reduce(12); break;
            case '-': reduce(12); break;
            case '*': reduce(12); break;
            case '/': reduce(12); break;
            case ')': reduce(12); break;
            default: return false; break;
            }
            break;
        case 22:
            switch (next_sym[0])
            {
            case '+': shift(35); break;
            case '-': shift(36); break;
            case ')': shift(37); break;
            default: return false; break;
            }
            break;
        case 23:
            switch (next_sym[0])
            {
            case '+': reduce(14); break;
            case '-': reduce(14); break;
            case '*': reduce(14); break;
            case '/': reduce(14); break;
            case ')': reduce(14); break;
            default: return false; break;
            }
            break;
        case 24:
            switch (next_sym[0])
            {
            case '+': reduce(15); break;
            case '-': reduce(15); break;
            case '*': reduce(15); break;
            case '/': reduce(15); break;
            case ')': reduce(15); break;
            default: return false; break;
            }
            break;
        case 25:
            if (next_sym == "id")
                shift(23);
            else if (next_sym == "int_num")
                shift(24);
            else if (next_sym == "(")
                shift(25);
            else
                return false;
            break;
        case 26:
            if (next_sym == "$")
                reduce(4);
            else
                return false;
            break;
        case 27:
            if (next_sym == "$")
                reduce(5);
            else
                return false;
            break;
        case 28:
            if (next_sym == "$")
                reduce(6);
            else
                return false;
            break;
        case 29:
            switch (next_sym[0])
            {
            case '+': reduce(10); break;
            case '-': reduce(10); break;
            case '*': reduce(10); break;
            case '/': reduce(10); break;
            case '$': reduce(10); break;
            default: return false; break;
            }
            break;
        case 30:
            switch (next_sym[0])
            {
            case '+': reduce(11); break;
            case '-': reduce(11); break;
            case '*': reduce(11); break;
            case '/': reduce(11); break;
            case '$': reduce(11); break;
            default: return false; break;
            }
            break;
        case 31:
            switch (next_sym[0])
            {
            case '+': reduce(7); break;
            case '-': reduce(7); break;
            case '*': shift(16); break;
            case '/': shift(17); break;
            case '$': reduce(7); break;
            default: return false; break;
            }
            break;
        case 32:
            switch (next_sym[0])
            {
            case '+': reduce(8); break;
            case '-': reduce(8); break;
            case '*': shift(16); break;
            case '/': shift(17); break;
            case '$': reduce(8); break;
            default: return false; break;
            }
            break;
        case 33:
            if (next_sym == "id")
                shift(23);
            else if (next_sym == "int_num")
                shift(24);
            else if (next_sym == "(")
                shift(25);
            else
                return false;
            break;
        case 34:
            if (next_sym == "id")
                shift(23);
            else if (next_sym == "int_num")
                shift(24);
            else if (next_sym == "(")
                shift(25);
            else
                return false;
            break;
        case 35:
            if (next_sym == "id")
                shift(23);
            else if (next_sym == "int_num")
                shift(24);
            else if (next_sym == "(")
                shift(25);
            else
                return false;
            break;
        case 36:
            if (next_sym == "id")
                shift(23);
            else if (next_sym == "int_num")
                shift(24);
            else if (next_sym == "(")
                shift(25);
            else
                return false;
            break;
        case 37:
            switch (next_sym[0])
            {
            case '+': reduce(13); break;
            case '-': reduce(13); break;
            case '*': reduce(13); break;
            case '/': reduce(13); break;
            case '$': reduce(13); break;
            default: return false; break;
            }
            break;
        case 38:
            switch (next_sym[0])
            {
            case '+': shift(35); break;
            case '-': shift(36); break;
            case ')': shift(43); break;
            default: return false; break;
            }
            break;
        case 39:
            switch (next_sym[0])
            {
            case '+': reduce(10); break;
            case '-': reduce(10); break;
            case '*': reduce(10); break;
            case '/': reduce(10); break;
            case ')': reduce(10); break;
            default: return false; break;
            }
            break;
        case 40:
            switch (next_sym[0])
            {
            case '+': reduce(11); break;
            case '-': reduce(11); break;
            case '*': reduce(11); break;
            case '/': reduce(11); break;
            case ')': reduce(11); break;
            default: return false; break;
            }
            break;
        case 41:
            switch (next_sym[0])
            {
            case '+': reduce(7); break;
            case '-': reduce(7); break;
            case '*': shift(33); break;
            case '/': shift(34); break;
            case ')': reduce(7); break;
            default: return false; break;
            }
            break;
        case 42:
            switch (next_sym[0])
            {
            case '+': reduce(8); break;
            case '-': reduce(8); break;
            case '*': shift(33); break;
            case '/': shift(34); break;
            case ')': reduce(8); break;
            default: return false; break;
            }
            break;
        case 43:
            switch (next_sym[0])
            {
            case '+': reduce(13); break;
            case '-': reduce(13); break;
            case '*': reduce(13); break;
            case '/': reduce(13); break;
            case ')': reduce(13); break;
            default: return false; break;
            }
            break;
        default:
            return false;
            break;
        }
    return true;
}

string getreg(string r)
{
    if (link_search(r, table[r[0] % 26]) || isdigit(r[0]))
        return reg[r_num++];
    else
        return r_temp[r[1] - 48];
}

void make_token_list(char* filename)
{
    token t;
    ofstream token_Out("token.txt");
    get_program(filename);
    init_table();
    while (1)
    {
        t = token_scan();
        if (t.code == 0)
            break;
        else if (t.code > 0)
            token_Out << "(" << t.code << "," << t.value << ")" << endl;
        else
        {
            if (t.code == -1)
                cout << "LEXICAL ERROR!" << endl;
            return;
        }
    }
    cout << "LEXICAL ANALYSIS SUCCEED!" << endl;
}

void grammar_midcode()
{
    int top_state;
    int len;
    string top_symbol;
    len = load_list();
    pos = 0;
    init_stack();
    while (pos != len)
    {
        top_state = state.top();
        if (!action(top_state, LR1_list[pos].sym))
        {
            cout << "ERROR in action" << endl;
            return;
        }
        if (!status_goto)
        {
            cout << "ERROR in goto" << endl;
            return;
        }
    }
    cout << "GRAMMER ANALYSIS SUCCEED!" << endl;
    return;
}

void make_aidcode()
{
    quad temp;
    ofstream result_out("result.txt");
    string L, y, z;
    string reg_info[10];
    string addr_info[10];
    for (int i = 0; i < quad_len; i++)
    {
        temp = quads[i];
        switch (temp.op[0])
        {
        case 'a':
            if (!link_search(temp.r1, table[temp.r1[0] % 26]) && !isdigit(temp.r1[0]))
                result_out << "MOV " << r_temp[temp.r1[1] - 48] << " " << temp.t << endl;
            else
                result_out << "MOV " << temp.r1 << " " << temp.t << endl;
            break;
        case '+':
            y = getreg(temp.r1);
            z = (link_search(temp.r2, table[temp.r2[0] % 26]) || isdigit(temp.r2[0])) ? temp.r2 : r_temp[temp.r2[1] - 48];
            if (link_search(temp.r1, table[temp.r1[0] % 26]) || isdigit(temp.r1[0]))
            {
                result_out << "MOV " << temp.r1 << " " << y << endl;
            }
            L = y;
            r_temp[temp.t[1] - 48] = y;
            result_out << "ADD " << z << " " << L << endl;
            break;
        case '-':
            y = getreg(temp.r1);
            z = (link_search(temp.r2, table[temp.r2[0] % 26]) || isdigit(temp.r2[0])) ? temp.r2 : r_temp[temp.r2[1] - 48];
            if (link_search(temp.r1, table[temp.r1[0] % 26]) || isdigit(temp.r1[0]))
            {
                result_out << "MOV " << temp.r1 << " " << y << endl;
            }
            L = y;
            r_temp[temp.t[1] - 48] = y;
            result_out << "SUB " << z << " " << L << endl;
            break;
        case '*':
            y = getreg(temp.r1);
            z = (link_search(temp.r2, table[temp.r2[0] % 26]) || isdigit(temp.r2[0])) ? temp.r2 : r_temp[temp.r2[1] - 48];
            if (link_search(temp.r1, table[temp.r1[0] % 26]) || isdigit(temp.r1[0]))
            {
                result_out << "MOV " << temp.r1 << " " << y << endl;
            }
            L = y;
            r_temp[temp.t[1] - 48] = y;
            result_out << "MUL " << z << " " << L << endl;
            break;
        case '/':
            y = getreg(temp.r1);
            z = (link_search(temp.r2, table[temp.r2[0] % 26]) || isdigit(temp.r2[0])) ? temp.r2 : r_temp[temp.r2[1] - 48];
            if (link_search(temp.r1, table[temp.r1[0] % 26]) || isdigit(temp.r1[0]))
            {
                result_out << "MOV " << temp.r1 << " " << y << endl;
            }
            L = y;
            r_temp[temp.t[1] - 48] = y;
            result_out << "DIV " << z << " " << L << endl;
            break;
        default:
            break;
        }
    }
}