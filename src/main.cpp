#include <cstdlib>
#include <cstdio>
#include <string>
#include "HTMLEngine.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        // fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        // return 1;
    }

    HTMLEngine engine;
    engine.LoadURL("tests/test12.html" );// argv[1]);
    engine.Run();

    return 0;
}