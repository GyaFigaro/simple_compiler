#include<string>
using namespace std;

typedef struct quad
{
    string op;
    string r1;
    string r2;
    string t;
}quad;

typedef struct attribute
{
    string sym = "\0";
    string type = "\0";
    string addr = "\0";
    string val = "\0";
    int intval = 0;
}attr;

extern quad quads[500];

void init_stack();

int load_list();

//根据归约情况和产生式创建三地址码
void make_code(string op, string r1, string r2, string t);

//归约后转移到目标状态
bool goto_state(int top_state, string top_sym);

//移进并完成状态转移
void shift(int next_state);

//归约，同时生成产生式和目标代码
void reduce(int pro_code);

//确定动作，依据动作表和输入区首符号
bool action(int cur_state, string next_sym);

void make_token_list(char* filename);

void grammar_midcode();

void make_aidcode();


