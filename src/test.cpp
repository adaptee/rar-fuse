#include <fstream>
#include <iostream>
using std::ios;
using std::wcout;

#include <locale.h>

#include "archive.h"
#include "fs.h"

int main(int argc, char const* argv[])
{
    // initialize locale
    setlocale(LC_ALL, "");

    //Archive archive(argv[1]);
    //std::wcout<<archive.debugRepr();

    FileSystem fs(argv[1]);
    //std::wcout<<fs.debugRepr();
    //std::wcout<<fs.debugRepr2();
    std::wcout<<fs.debugRepr3();
    //std::wcout<<fs.archive()->debugRepr();

    return 0;
}
