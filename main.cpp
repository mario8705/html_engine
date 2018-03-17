#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include "parser.h"
#include "dom.h"
#include "window.h"

struct Rectangle
{
    float left;
    float right;
    float top;
    float bottom;
};

static void RenderElement(cairo_t *cr, HTMLElement *element)
{
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 14.0);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, element->GetData().c_str(), &extents);

    cairo_move_to(cr, 0, extents.height);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_show_text(cr, element->GetData().c_str());

    for (HTMLElement *child : element->GetChildrens())
    {
        RenderElement(cr, child);
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

    RenderElement(cr, rootElement);

    window.Present();
    window.PumpEvents();
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