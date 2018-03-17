#pragma once
#include <SDL.h>
#include <cairo/cairo.h>

class Window
{
public:
    Window()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            // TODO error handling
            exit(1);
        }
    
        SDL_WM_SetCaption("HTML Engine", nullptr);

        m_surf = SDL_SetVideoMode(1280, 720, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
        m_cairoSurf = cairo_image_surface_create_for_data((unsigned char *)m_surf->pixels, CAIRO_FORMAT_RGB24, m_surf->w, m_surf->h, m_surf->pitch);
        m_cairo = cairo_create(m_cairoSurf);
    }

    ~Window()
    {
        cairo_destroy(m_cairo);
        cairo_surface_destroy(m_cairoSurf);

        SDL_Quit();
    }

    void PumpEvents()
    {
        SDL_Event event;
    
        while (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return;
            }
        }
    }

    void Present()
    {
        SDL_Flip(m_surf);
    }

    cairo_t *get_cairo() const
    {
        return m_cairo;
    }

private:
    SDL_Surface *m_surf;
    cairo_t *m_cairo;
    cairo_surface_t *m_cairoSurf;
};