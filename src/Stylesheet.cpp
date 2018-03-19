#include "Stylesheet.h"
#include "HTMLElement.h"

Stylesheet::Stylesheet(ADOMNode *parent)
    : m_parent(parent)
{
    m_backgroundType = Stylesheet::BT_Transparent;
    m_fontSize = 14.0f;
}

// Loads user agent styles
void Stylesheet::LoadUserAgentStyles()
{
    HTMLElement *element;

    if ((element = m_parent->AsElementNode()) != nullptr)
    {
        if (element->GetTagName() == "h1")
        {
            m_fontSize = 28.0f;            
        }
    }
}

ADOMNode *Stylesheet::GetParent() const
{
    return m_parent;
}

color_t Stylesheet::GetTextColor() const
{
    // if (m_parent->HasAttribute("fgcolor"))
    {
        // return g_colorTranslationMap[m_parent->GetAttribute("fgcolor")];
    }

    return 0x000000FF;
}

static Stylesheet::TextAlignment parse_text_alignment(const std::string &str)
{
    if (str == "left")
    {
    }
    else if (str == "center")
    {
        return Stylesheet::TextAlignment::Center;        
    }
    else if (str == "right")
    {
        return Stylesheet::TextAlignment::Right;        
    }

    // TODO throw an error if unknown?

    return Stylesheet::TextAlignment::Left;
}

Stylesheet::TextAlignment Stylesheet::GetTextAlignment() const
{
    // Check in the attached element for inline style and attribute align
    // TODO is text-align recursive?

    // if (m_parent->HasAttribute("align"))
    {
        // return parse_text_alignment(m_parent->GetAttribute("align"));
    }

    return TextAlignment::Left;
}

float Stylesheet::GetFontSize() const
{
    return m_fontSize;
}