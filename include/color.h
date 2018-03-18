#pragma once
#include <string>
#include <unordered_map>
#include <cairo/cairo.h>

// Format: 0xRRGGBBAA
typedef unsigned int color_t;

static std::unordered_map<std::string, color_t> g_colorTranslationMap =
{
    { "red", 0xFF0000FF },
    { "black", 0x000000FF },
    { "white", 0xFFFFFFFF },
    { "fuschia", 0xFF00FFFF }
};

// Utility function
static void color_cairo_set_source(cairo_t *cr, color_t color)
{
    const double r = ((color & 0xFF000000) << 0) / 255.0;
    const double g = ((color & 0x00FF0000) << 8) / 255.0;
    const double b = ((color & 0x0000FF00) << 16) / 255.0;

    cairo_set_source_rgb(cr, r, g, b);
}