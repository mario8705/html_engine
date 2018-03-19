#include "DOMNode.h"
#include "Stylesheet.h"

ADOMNode::ADOMNode(ADOMNode::NodeType nodeType, HTMLElement *parent)
    : m_nodeType(nodeType), m_parent(parent)
{
    m_stylesheet = new Stylesheet(this);
}

ADOMNode::~ADOMNode()
{
    delete m_stylesheet;
}
