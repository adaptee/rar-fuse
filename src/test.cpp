#include <fstream>
#include <iostream>
using std::ios;
using std::cout;

#include <locale.h>

#include "archive.h"
#include "fs.h"

int main(int argc, char const* argv[])
{
    // initialize locale
    setlocale(LC_ALL, "");

    //Archive archive(argv[1]);
    //std::cout<<archive.debugRepr();

    FileSystem fs(argv[1]);
    std::cout<<fs.debugRepr();
    std::cout<<fs.archive()->debugRepr();

    return 0;
}
