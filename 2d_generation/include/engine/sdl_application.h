#pragma once
#include <SDL3/SDL.h>
namespace Fw::Graphics
{
    class SdlApplication
    {
    public:
        bool failure = false;
        SdlApplication();

        void loop();

        ~SdlApplication();
    private:
        SDL_Window* _window;
        bool _done;
        SDL_GLContext _context;

        void events();
    };
}

