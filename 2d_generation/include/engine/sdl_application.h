#pragma once
#include <SDL3/SDL.h>

#include <fw_graphics/renderer2d.h>


namespace Fw::Graphics
{
    class SdlApplication
    {
    public:
        Renderer2D renderer;
        bool failure = false;
        SdlApplication();

        void run();

        ~SdlApplication();
    private:
        SDL_Window* _window;
        bool _done;
        SDL_GLContext _context;

        void initImgui() const;
        void events();
        void processInput();
        void cameraInfo();
        void mouseInfo();
    };
}

