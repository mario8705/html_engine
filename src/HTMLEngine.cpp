#include "HTMLEngine.h"
#include <cstdio>
#include "MainWindow.h"
#include "HTMLElement.h"
#include "Stylesheet.h"
#include "TextElement.h"

#include <map>

#ifndef M_PI
#define M_PI 3.14159654359
#endif

struct LayoutInfo
{
    float x, y;
    float w, h;
};

std::map<ADOMNode *, LayoutInfo> m_layoutCache;

void cairo_rounded_rectangle(cairo_t * cr, double x, double y, double width, double height, double radius)
{
	double degrees = M_PI / 180.0;

	cairo_new_sub_path(cr);
	cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path(cr);
}

void chrome_draw_button(cairo_t * cr, int x, int y, int width, int height, char * title) {
	cairo_save(cr);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);

	cairo_rounded_rectangle(cr, 2 + x, 2 + y, width - 4, height - 4, 2.0);
	cairo_set_source_rgba(cr, 44.0/255.0, 71.0/255.0, 91.0/255.0, 29.0/255.0);
	cairo_set_line_width(cr, 4);
	cairo_stroke(cr);

	cairo_rounded_rectangle(cr, 2 + x, 2 + y, width - 4, height - 4, 2.0);
	cairo_set_source_rgba(cr, 158.0/255.0, 169.0/255.0, 177.0/255.0, 1.0);
	cairo_set_line_width(cr, 2);
	cairo_stroke(cr);

	{
		cairo_pattern_t * pat = cairo_pattern_create_linear(2 + x, 2 + y, 2 + x, 2 + y + height - 4);
		cairo_pattern_add_color_stop_rgba(pat, 0, 1, 1, 1, 1);
		cairo_pattern_add_color_stop_rgba(pat, 1, 241.0/255.0, 241.0/255.0, 244.0/255.0, 1);
		cairo_rounded_rectangle(cr, 2 + x, 2 + y, width - 4, height - 4, 2.0);
		cairo_set_source(cr, pat);
		cairo_fill(cr);
		cairo_pattern_destroy(pat);
	}

	{
		cairo_pattern_t * pat = cairo_pattern_create_linear(3 + x, 3 + y, 3 + x, 3 + y + height - 4);
		cairo_pattern_add_color_stop_rgba(pat, 0, 252.0/255.0, 252.0/255.0, 254.0/255.0, 1);
		cairo_pattern_add_color_stop_rgba(pat, 1, 223.0/255.0, 225.0/255.0, 230.0/255.0, 1);
		cairo_rounded_rectangle(cr, 3 + x, 3 + y, width - 5, height - 5, 2.0);
		cairo_set_source(cr, pat);
		cairo_fill(cr);
		cairo_pattern_destroy(pat);
	}

	{
		// draw_string(&fake_context, (width - str_width) / 2 + x, y + (height) / 2 + 4, rgb(49,49,49), title);
	}

    cairo_restore(cr);
}

void DrawBrowserChrome(cairo_t *cr)
{
    chrome_draw_button(cr, 100, 100, 200, 40, "Hello, World!");
}

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
        childLayoutInfo.h = element->GetStylesheet()->GetFontSize();

        yOffset += childLayoutInfo.h;
    }
}

void PaintBackground(cairo_t *cr, const Stylesheet *stylesheet, const LayoutInfo &layoutInfo)
{
    Stylesheet::BackgroundType bt = stylesheet->GetBackgroundType();

    if (bt == Stylesheet::BT_SolidColor)
    {
        color_cairo_set_source(cr, stylesheet->GetBackgroundColor());
        cairo_rectangle(cr, layoutInfo.x, layoutInfo.y, layoutInfo.w, layoutInfo.h);
        cairo_fill(cr);
    }
}

void RenderElement(cairo_t *cr, ADOMNode *element)
{
    const LayoutInfo &layoutInfo = m_layoutCache[element];
    // const Stylesheet *stylesheet = element->GetStylesheet();

    // PaintBackground(cr, stylesheet, layoutInfo);

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
    HTMLElement *root = m_document->GetRootElement();
    CalcLayout(root);
    DrawBrowserChrome(m_mainWindow->GetCairo());
    RenderElement(m_mainWindow->GetCairo(), root);

    m_mainWindow->Present();
    m_mainWindow->PumpEvents();
}