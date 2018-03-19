#include "TextElement.h"
#include "HTMLElement.h"

using namespace std;

TextElement::TextElement(const string &data, HTMLElement *parent)
    : ADOMNode(ADOMNode::TextNode), m_data(data)
{
    if (m_parent)
    {
        m_parent->AppendChildren(this);
    }
}
