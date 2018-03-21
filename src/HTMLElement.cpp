#include "HTMLElement.h"
#include <algorithm>
#include <sstream>
#include "Stylesheet.h"
#include "TextElement.h"

using namespace std;

HTMLElement::HTMLElement(const string &tagName, HTMLElement *parent)
    : ADOMNode(ADOMNode::ElementNode, parent), m_tagName(tagName)
{
    if (m_parent)
    {
        m_parent->AppendChildren(this);
    }

    m_stylesheet->LoadUserAgentStyles();
}

HTMLElement::~HTMLElement()
{
    for (const ADOMNode *child : m_childrens)
    {
        delete child;
    }
}

string HTMLElement::GetTagName() const
{
    return m_tagName;
}

bool HTMLElement::HasAttribute(string name) const
{
    return m_attributes.find(name) != m_attributes.end();
}

string HTMLElement::GetAttribute(string name, string defaultValue) const
{
    if (!HasAttribute(name))
    {
        return defaultValue;
    }

    return m_attributes.at(name);
}

string HTMLElement::GetAttribute(string name) const
{
    return m_attributes.at(name);
}

void HTMLElement::SetAttribute(string name, string value)
{
    m_attributes[name] = value;
}

void HTMLElement::AppendChildren(ADOMNode *child)
{
    m_childrens.push_back(child);
    child->m_parent = this;
}

vector<ADOMNode *> HTMLElement::GetChildrens() const
{
    return m_childrens;
}

std::string HTMLElement::RawData() const
{
    std::stringstream ss;
    TextElement *element;

    for (auto it = m_childrens.begin(); it != m_childrens.end(); ++it)
    {
        if ((element = (*it)->AsTextNode()) != nullptr)
        {
            ss << element->GetData();
        }
    }

    return ss.str();
}

HTMLElement *HTMLElement::FindLastOf(const std::string &tagName) const
{
    HTMLElement *element, *lastOf = nullptr, *tmp;

    for (auto it = m_childrens.begin(); it != m_childrens.end(); ++it)
    {
        if ((element = (*it)->AsElementNode()) != nullptr)
        {
            if (element->GetTagName() == tagName)
            {
                lastOf = element;
            }

            tmp = element->FindLastOf(tagName);

            if (tmp)
            {
                lastOf = tmp;
            }
        }
    }

    return lastOf;
}

void HTMLElement::DumpTree(int ident) const
{
    printf("%s\n", m_tagName.c_str());
    const HTMLElement *element;
    const TextElement *textNode;

    for (const ADOMNode *child : m_childrens)
    {
        for (int i = 0; i < ident; i++)
        {
            printf("  ");
        }

        if ((element = child->AsElementNode()) != nullptr)
        {
            element->DumpTree(ident + 1);
        }
        else if ((textNode = child->AsTextNode()) != nullptr)
        {
            printf("Text: %s\n", textNode->GetData().c_str());
        }
    }
}