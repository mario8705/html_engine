#include "MainWindow.h"
#include <stdexcept>
#include <SDL.h>
#include <cairo/cairo.h>

MainWindow::MainWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_WM_SetCaption("HTML Engine", nullptr);

    m_surf = SDL_SetVideoMode(1280, 720, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!m_surf)
    {
        throw std::runtime_error(SDL_GetError());
    }

    m_cairoSurf = cairo_image_surface_create_for_data((unsigned char *)m_surf->pixels, CAIRO_FORMAT_RGB24, m_surf->w, m_surf->h, m_surf->pitch);
    
    m_cairo = cairo_create(m_cairoSurf);
}

MainWindow::~MainWindow()
{
    cairo_surface_destroy(m_cairoSurf);
    cairo_destroy(m_cairo);
    SDL_Quit();
}

void MainWindow::SetCaption(const std::string &captionText)
{
    SDL_WM_SetCaption(captionText.c_str(), nullptr);    
}

void MainWindow::PumpEvents()
{
    SDL_Event event;

    do
    {
        SDL_WaitEvent(&event);
    } while (event.type != SDL_QUIT);
}

void MainWindow::Present()
{
    SDL_Flip(m_surf);
}

cairo_t *MainWindow::GetCairo() const
{
    return m_cairo;
}
