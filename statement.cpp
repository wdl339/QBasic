#include "statement.h"

Statement::Statement(string ss)
{
    s = ss;
    lineNum = 0;
    for(int i = 0; ss[i] != ' '; i++) {
        lineNum = lineNum * 10 + ss[i] - '0';
    }

}
