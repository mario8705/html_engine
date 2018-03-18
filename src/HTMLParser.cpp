#include "HTMLParser.h"
#include "CharacterBuffer.h"

HTMLParser::HTMLParser()
{
}

void HTMLParser::Parse(CharacterBuffer &buf)
{
    buf.TrimLeft();

    while (!buf.IsEmpty())
    {
        if (*buf == '<')
        {
            ++buf;

            buf.TrimLeft();

            if (*buf == '/')
            {
                printf("Close tag\n");
                ++buf;
                buf.TrimLeft();
            }

            std::string ident = buf.GetRegex("^[a-z][a-z0-9]+");
            printf("Ident spotted: %s\n", ident.c_str());

            buf.TrimLeft();

            while (buf)
            {
                if (*buf == '>')
                {
                    break;
                }

                ++buf;
            }
        }

        ++buf;
    }

    exit(0);
}
