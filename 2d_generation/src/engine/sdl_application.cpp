#include "sdl_application.h"

#include <vector>
#include <unordered_map>

#include <glad/glad.h>

#include <fw_utility/file_loaders.h>
#include <fw_graphics/shader.h>
#include "engine/world.h"
#include "config/config.h"


namespace Fw::Graphics
{
    SdlApplication::SdlApplication() {
        using namespace Config::Window;
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        _done = false;
        _window = SDL_CreateWindow("SDLAPP", windowWidth, windowHeight, SDL_WINDOW_OPENGL);
        if (!_window)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "COULD NOT CREATE WINDOW");
        }

        _context = SDL_GL_CreateContext(_window);
        SDL_GL_MakeCurrent(_window, _context);

        if (!_context)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "CONTEXT NOT SET");
        }

        if (_window == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
            failure = true;
        }
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            SDL_Log("OPENGL NOT LOADED");
            failure = true;
        }
        if (!failure)
        {
            loop();
        }
    }

    void SdlApplication::loop() {
        using namespace Config::Shader;
        std::unordered_map<Name, Shader> shaders;
        shaders.try_emplace(CHUNK, RESOURCE_PATH "shaders/chunk.vert", RESOURCE_PATH "shaders/chunk.frag");
        
        
        Engine::World world;

        while (!_done) {
            this->events();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            world.update();
            world.render(shaders);


            SDL_GL_SwapWindow(_window);
        }
    }

    SdlApplication::~SdlApplication() {
        SDL_GL_DestroyContext(_context);
        SDL_DestroyWindow(_window);
        SDL_Quit();

    }

    void SdlApplication::events() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE)) {
                _done = true;
            }
        }
    }
}
