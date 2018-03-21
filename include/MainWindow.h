#pragma once
#include <string>
#include <SDL.h>
#include <cairo/cairo.h>

class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    void SetCaption(const std::string &captionText);

    void Lock();
    void Unlock();

    void PumpEvents();

    void Present();

    cairo_t *GetCairo() const;

private:
    SDL_Surface *m_surf;
    cairo_t *m_cairo;
    cairo_surface_t *m_cairoSurf;
};