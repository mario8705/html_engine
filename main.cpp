#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include "parser.h"
#include "dom.h"
#include "window.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct Rectangle
{
    float left;
    float top;
    float right;
    float bottom;
};

void CalcElementSize(HTMLElement *element, Rectangle *size)
{
    auto childs = element->GetChildrens();
    std::string data = element->GetData();

    // Assuming display=block here
    for (HTMLElement *childElement : childs)
    {
        Rectangle childSize = { 0, 0, 0, 0 };
        CalcElementSize(childElement, &childSize);

        size->right = MAX(size->right, childSize.right);
        size->bottom += size->bottom;
    }

    if (!data.empty())
    {
        // Assuming 14px base font
        size->bottom += 14;
    }

    element->SetClientWidth(size->right - size->left);
    element->SetClientHeight(size->bottom - size->top);
}

void CalcLayout(HTMLElement *element, const Rectangle &viewport)
{
    Rectangle size = { 0, 0, 0, 0 };
    CalcElementSize(element, &size);
}

static void RenderElement(cairo_t *cr, HTMLElement *element, int baseHeight)
{
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 14.0);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, element->GetData().c_str(), &extents);

    cairo_move_to(cr, 0, extents.height + baseHeight);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_show_text(cr, element->GetData().c_str());

    for (HTMLElement *child : element->GetChildrens())
    {
        RenderElement(cr, child, baseHeight);
        baseHeight += child->GetClientHeight();
    }
}

static void parse_and_run(const char *path)
{
    size_t sz;
    char *buf = load_all(path, &sz);
    buf_it it;
    std::vector<HTMLToken> tokens;

    if (!buf)
    {
        return; // TODO
    }

    it.start = buf;
    it.end = buf + sz;

    parse(&it, tokens);

    free(buf);

    // Ok, we have all the tokens so lets create a dom now

    HTMLElement *rootElement = create_dom(tokens);

    Window window;

    cairo_t *cr = window.get_cairo();

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_rectangle(cr, 0, 0, 1280, 720);
    cairo_fill(cr);

    CalcLayout(rootElement, { 0, 0, 1280, 720 });
    RenderElement(cr, rootElement, 0);

    window.Present();
    window.PumpEvents();

    delete rootElement;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    parse_and_run(argv[1]);

    return 0;
}