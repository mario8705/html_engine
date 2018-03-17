#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include "parser.h"

class HTMLElement
{
public:
    HTMLElement(const std::string &tagName, HTMLElement *parent = nullptr)
        : m_tagName(tagName), m_parent(parent)
    {
    }

    std::string GetTagName() const
    {
        return m_tagName;
    }

    void AddChildren(HTMLElement *element)
    {
        m_childrens.push_back(element);
    }

    bool IsRootElement() const
    {
        return m_parent == nullptr;
    }

    std::string GetData() const
    {
        return m_data;
    }

    void SetData(std::string data)
    {
        m_data = data;
    }

    void PrintTree(int depth = 1)
    {
        if (depth == 1) printf("%s\n", m_tagName.c_str());

        for (HTMLElement *child : m_childrens)
        {
            int ident = depth;
            while (ident--)
            {
                printf("  ");
            }

            printf("%s (%s)\n", child->GetTagName().c_str(), child->GetData().c_str());
            child->PrintTree(depth + 1);            
        }
    }

private:
    HTMLElement *m_parent;
    std::vector<HTMLElement *> m_childrens;
    std::string m_tagName;
    std::string m_data;
};

class HTMLDocument
{
public:
    HTMLDocument()
    {
    }
    
    ~HTMLDocument()
    {
    }
};

using HTMLTokenIterator = std::vector<HTMLToken>::const_iterator;

static void assemble_dom(HTMLElement *parent, HTMLTokenIterator start, HTMLTokenIterator end)
{
    HTMLElement *element = nullptr;

    while (start != end)
    {
        const HTMLToken &tok = *start;

        if (tok.type == HT_OpenTag)
        {
            element = new HTMLElement(tok.data);
            printf("Open tag: %s\n", tok.data);    
        }
        else if (tok.type == HT_CloseTag)
        {
            parent->AddChildren(element);
            element = nullptr;
            printf("Close tag: %s\n", tok.data);
        }
        else if (tok.type == HT_Data)
        {
            element->SetData(tok.data);
            printf("Data: %s\n", tok.data);
        }

        ++start;
    }
}

static void create_dom(const std::vector<HTMLToken> &tokens)
{
    HTMLElement rootElement("html");

    assemble_dom(&rootElement, tokens.begin(), tokens.end());

    rootElement.PrintTree();
}

static void parse_and_run(const char *path)
{
    size_t sz;
    char *buf = load_all(path, &sz);
    buf_it it;
    std::vector<HTMLToken> tokens;

    if (!buf)
    {
        return; // TODO
    }

    it.start = buf;
    it.end = buf + sz;

    parse(&it, tokens);

    free(buf);

    // Ok, we have all the tokens so lets create a dom now

    create_dom(tokens);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    parse_and_run(argv[1]);

    return 0;
}