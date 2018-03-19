#pragma once
#include "DOMNode.h"
#include <string>

class HTMLElement;

class TextElement : public ADOMNode
{
public:
    explicit TextElement(const std::string &data, HTMLElement *parent = nullptr);

    inline std::string GetData() const
    {
        return m_data;
    }

private:
    std::string m_data;
};
