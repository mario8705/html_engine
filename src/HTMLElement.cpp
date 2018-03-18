#include "HTMLElement.h"
#include <algorithm>
#include "Stylesheet.h"

using namespace std;

HTMLElement::HTMLElement(const string &tagName, HTMLElement *parent)
    : m_tagName(tagName)
{
    m_parent = parent;

    if (parent)
    {
        parent->AppendChildren(this);
    }

    m_stylesheet = new Stylesheet(this);
}

HTMLElement::~HTMLElement()
{
    delete m_stylesheet;

    for (HTMLElement *child : m_childrens)
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

HTMLElement *HTMLElement::GetParent() const
{
    return m_parent;
}

void HTMLElement::AppendChildren(HTMLElement *child)
{
    printf("Parent: %s\n", m_tagName.c_str());
    m_childrens.push_back(child);
    child->m_parent = this;
}

vector<HTMLElement *> HTMLElement::GetChildrens() const
{
    return m_childrens;
}

void HTMLElement::DumpTree(int ident) const
{
    printf("%s\n", m_tagName.c_str());

    for (const HTMLElement *child : m_childrens)
    {
        for (int i = 0; i < ident; i++)
        {
            printf("  ");
        }

        child->DumpTree(ident + 1);
    }
}