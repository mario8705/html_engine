#pragma once

class Stylesheet;
class HTMLElement;
class TextElement;

class ADOMNode
{
    friend HTMLElement;
    friend TextElement; // XXX Not sure about this one

public:
    enum NodeType
    {
        TextNode,
        ElementNode
    };

    virtual ~ADOMNode();

    inline NodeType GetNodeType() const
    {
        return m_nodeType;
    }

    inline Stylesheet *GetStylesheet() const
    {
        return m_stylesheet;
    }

    inline HTMLElement *GetParent() const
    {
        return m_parent;
    }

    inline HTMLElement *AsElementNode()
    {
        if (m_nodeType != ElementNode)
        {
            return nullptr;
        }

        return reinterpret_cast<HTMLElement *>(this);
    }

    inline const HTMLElement *AsElementNode() const
    {
        if (m_nodeType != ElementNode)
        {
            return nullptr;
        }

        return reinterpret_cast<const HTMLElement *>(this);
    }

    inline TextElement *AsTextNode()
    {
        if (m_nodeType != TextNode)
        {
            return nullptr;
        }

        return reinterpret_cast<TextElement *>(this);
    }

    inline const TextElement *AsTextNode() const
    {
        if (m_nodeType != TextNode)
        {
            return nullptr;
        }

        return reinterpret_cast<const TextElement *>(this);
    }

protected:
    explicit ADOMNode(NodeType nodeType, HTMLElement *parent = nullptr);

    Stylesheet *m_stylesheet;
    HTMLElement *m_parent;

private:
    NodeType m_nodeType;
};
