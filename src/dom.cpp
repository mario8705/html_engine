#include "dom.h"
#include <map>
#include <vector>
#include "parser.h"
#include "HTMLElement.h"
#include "TextElement.h"

using HTMLTokenIterator = std::vector<HTMLToken>::const_iterator;

void assemble_dom(HTMLElement *parent, HTMLTokenIterator start, HTMLTokenIterator end)
{
    HTMLElement *element = parent;
    std::vector<HTMLElement *> stack;

    /*element = new HTMLElement("p", parent);
    element->SetData("Hello, World!");
    element->SetAttribute("align", "center");

    element = new HTMLElement("div", parent);
    element = new HTMLElement("p", element);
    element->SetData("Hello, World inside a div");
*/

    for (; start != end; ++start)
    {
        const HTMLToken &tok = *start;

        switch (tok.type)
        {
            case HT_OpenTag:
                printf("Open tag: %s\n", tok.data);
                element = new HTMLElement(tok.data, element);

                stack.push_back(element);
                break;
            
            case HT_CloseTag:
                printf("Close tag: %s\n", tok.data);
                if (!stack.empty())
                {
                    stack.pop_back();

                    if (!stack.empty())
                    {
                        element = stack.back();
                    }
                    else
                    {
                        element = parent;
                    }
                }

                break;
            
            case HT_Attribute:
                printf("Attr: '%s' : '%s'\n", tok.data, tok.data2);
                element->SetAttribute(tok.data, tok.data2);
                break;
            
            case HT_Data:
                printf("Data: %s\n", tok.data);
                element->AppendChildren(new TextElement(tok.data));
                break;

            default:
                break;
        }
    }
}

HTMLElement *create_dom(const std::vector<HTMLToken> &tokens)
{
    HTMLElement *rootElement = new HTMLElement("html");

    assemble_dom(rootElement, tokens.begin(), tokens.end());

    return rootElement;
}