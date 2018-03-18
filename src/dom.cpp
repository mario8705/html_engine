#include "dom.h"
#include <map>
#include "parser.h"
#include "HTMLElement.h"
#include <vector>

using HTMLTokenIterator = std::vector<HTMLToken>::const_iterator;

void assemble_dom(HTMLElement *parent, HTMLTokenIterator start, HTMLTokenIterator end)
{
    HTMLElement *element = nullptr;
    std::vector<HTMLElement *> stack;

    while (start != end)
    {
        const HTMLToken &tok = *start;

        if (tok.type == HT_OpenTag)
        {
            printf("Open tag: %s, stack=%d\n", tok.data, stack.size());
            element = new HTMLElement(tok.data, stack.empty() ? parent : (*stack.end()));

            stack.push_back(element);
        }
        else if (tok.type == HT_CloseTag)
        {
            printf("Close tag: %s\n", tok.data);

            if (!stack.empty())
            {
                stack.pop_back();
                element = *(stack.end());
            }
            else
            {
                element = nullptr;
            }
        }
        else if (tok.type == HT_Data)
        {
            printf("Data: %s\n", tok.data);
            element->SetData(tok.data);
        }
        else if (tok.type == HT_Attribute)
        {
            printf("Attribute: %s=%s\n", tok.data, tok.data2);

            element->SetAttribute(tok.data, tok.data2);
        }

        ++start;
    }
}

HTMLElement *create_dom(const std::vector<HTMLToken> &tokens)
{
    HTMLElement *rootElement = new HTMLElement("html");

    assemble_dom(rootElement, tokens.begin(), tokens.end());

    return rootElement;
}