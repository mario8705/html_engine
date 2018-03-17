#pragma once
#include <map>
#include "parser.h"

class HTMLElement
{
public:
    HTMLElement(const std::string &tagName, HTMLElement *parent = nullptr)
        : m_tagName(tagName), m_parent(parent)
    {
    }

    ~HTMLElement()
    {
        for (HTMLElement *child : m_childrens)
        {
            delete child;
        }
    }

    std::string GetTagName() const
    {
        return m_tagName;
    }

    void AddChildren(HTMLElement *element)
    {
        m_childrens.push_back(element);
    }

    std::vector<HTMLElement *> GetChildrens() const
    {
        return m_childrens;
    }

    std::vector<HTMLElement *> &GetChildrens()
    {
        return m_childrens;
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

    HTMLElement *GetParent() const
    {
        return m_parent;
    }

    float GetClientWidth() const 
    {
        return m_clientWidth;
    }

    void SetClientWidth(float clientWidth)
    {
        m_clientWidth = clientWidth;
    }

    float GetClientHeight() const
    {
        return m_clientHeight;
    }

    void SetClientHeight(float clientHeight)
    {
        m_clientHeight = clientHeight;
    }

    void SetAttribute(std::string name, std::string value)
    {
        m_attributes[name] = value;
    }

    std::string GetAttribute(std::string name) const
    {
        return m_attributes.at(name);
    }

    std::string GetAttribute(std::string name, std::string defaultValue) const
    {
        if (!HasAttribute(name))
        {
            return defaultValue;
        }

        return m_attributes.at(name);
    }

    bool HasAttribute(std::string name) const
    {
        return m_attributes.find(name) != m_attributes.end();
    }

private:
    HTMLElement *m_parent;
    std::map<std::string, std::string> m_attributes;
    std::vector<HTMLElement *> m_childrens;
    std::string m_tagName;
    std::string m_data;
    float m_clientWidth, m_clientHeight;
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
            element = new HTMLElement(tok.data, parent);
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
            printf("Data: %s\n", tok.data);
            element->SetData(tok.data);
        }
        else if (tok.type == HT_Attribute)
        {
            element->SetAttribute(tok.data, tok.data2);
            printf("Attribute: %s=%s\n", tok.data, tok.data2);
        }

        ++start;
    }
}

static HTMLElement *create_dom(const std::vector<HTMLToken> &tokens)
{
    HTMLElement *rootElement = new HTMLElement("html");

    assemble_dom(rootElement, tokens.begin(), tokens.end());

    return rootElement;
}