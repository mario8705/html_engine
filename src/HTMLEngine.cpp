#include "HTMLEngine.h"
#include <cstdio>
#include "MainWindow.h"
#include "HTMLElement.h"
#include "Stylesheet.h"
#include "TextElement.h"

#include <map>

struct LayoutInfo
{
    float x, y;
    float w, h;
};

std::map<ADOMNode *, LayoutInfo> m_layoutCache;

void CalcLayout(HTMLElement *element)
{
    LayoutInfo &layoutInfo = m_layoutCache[element];

    if (element->GetParent() == nullptr)
    {
        layoutInfo.x = 0;
        layoutInfo.y = 0;
        layoutInfo.w = 1280;
        layoutInfo.h = 720;
    }

    float yOffset = 0;

    for (ADOMNode *child : element->GetChildrens())
    {
        LayoutInfo &childLayoutInfo = m_layoutCache[child];

        childLayoutInfo.x = layoutInfo.x;
        childLayoutInfo.y = layoutInfo.y + yOffset;
        childLayoutInfo.w = layoutInfo.w;
        /// TODO Move getstylesheet into adomnode
        childLayoutInfo.h = element->GetStylesheet()->GetFontSize();

        yOffset += childLayoutInfo.h;
    }
}

void PaintBackground(cairo_t *cr, const Stylesheet *stylesheet, const LayoutInfo &layoutInfo)
{
    Stylesheet::BackgroundType bt = stylesheet->GetBackgroundType();

    // if (bt == Stylesheet::BT_SolidColor)
    {
        color_cairo_set_source(cr, 0x0); // stylesheet->GetBackgroundColor());
        cairo_rectangle(cr, layoutInfo.x, layoutInfo.y, layoutInfo.w, layoutInfo.h);
        cairo_fill(cr);
    }
}

void RenderElement(cairo_t *cr, ADOMNode *element)
{
    const LayoutInfo &layoutInfo = m_layoutCache[element];
    const Stylesheet *stylesheet = element->GetStylesheet();

    PaintBackground(cr, stylesheet, layoutInfo);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    // cairo_set_font_size(cr, stylesheet->GetFontSize());

    if (element->GetNodeType() == ADOMNode::TextNode)
    {
        const TextElement *textElement = reinterpret_cast<const TextElement *>(element);

        cairo_text_extents_t extents;
        cairo_text_extents(cr, textElement->GetData().c_str(), &extents);

        float xText = 0.0f;

        /*switch (stylesheet->GetTextAlignment())
        {
            case Stylesheet::TextAlignment::Center:
                xText = (layoutInfo.w - extents.width) / 2;            
                break;

            case Stylesheet::TextAlignment::Right:
                xText = layoutInfo.w - extents.width;
                break;
            
            default:
                break;
        }*/

        // color_cairo_set_source(cr, stylesheet->GetTextColor());
        cairo_move_to(cr, layoutInfo.x + xText, layoutInfo.y + extents.height);
        cairo_show_text(cr, textElement->GetData().c_str());
    }
    else if (element->GetNodeType() == ADOMNode::ElementNode)
    {
        const HTMLElement *elementNode = reinterpret_cast<const HTMLElement *>(element);
    
        for (ADOMNode *child : elementNode->GetChildrens())
        {
            RenderElement(cr, child);
        }
    }
}

HTMLEngine::HTMLEngine()
{
    m_mainWindow.reset(new MainWindow);
    m_document.reset(new DOMDocument);
}

void HTMLEngine::LoadURL(const std::string &filename)
{
    m_document->LoadDocument(filename);
    printf("Loading local file: %s\n", filename.c_str());

    HTMLElement *root = m_document->GetRootElement();
    
    root->GetStylesheet()->SetBackgroundType(Stylesheet::BT_SolidColor);
    root->GetStylesheet()->SetBackgroundColor(0xFFFFFFFF);
}

void HTMLEngine::Run()
{
    m_mainWindow->Lock();

    HTMLElement *root = m_document->GetRootElement();
    CalcLayout(root);
    RenderElement(m_mainWindow->GetCairo(), root);
    
    m_mainWindow->SetCaption("HTML Engine - " + m_document->GetTitle());

    m_mainWindow->Unlock();

    m_mainWindow->Present();
    m_mainWindow->PumpEvents();
}