#include "exp.h"


Exp::~Exp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}

StringExp::~StringExp()
{
    for (Exp* ch : child) {
        delete ch;
    }
}
