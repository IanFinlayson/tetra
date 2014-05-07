#include "exit_funcs.h"
#include <iostream>
#include <string>

void err_exit(const std::string& msg)
{
    cerr <<  msg << endl;
    exit(1);
}

