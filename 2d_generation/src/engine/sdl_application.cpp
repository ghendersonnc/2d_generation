#include "sdl_application.h"

#include <unordered_map>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <fw_utility/file_loaders.h>
#include <fw_graphics/shader.h>
#include "engine/world.h"
#include "config/config.h"


namespace Fw::Graphics
{
    SdlApplication::SdlApplication() {
        using namespace Config;
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Window::openGlMajorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Window::openGlMinorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        _done = false;
        _window = SDL_CreateWindow(Window::windowTitle.c_str(), Window::windowWidth, Window::windowHeight, SDL_WINDOW_OPENGL);
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

        constexpr float right = static_cast<float>(Config::Window::windowWidth) / 2.f;
        constexpr float left = right - Config::Window::windowWidth;
        constexpr float top = static_cast<float>(Config::Window::windowHeight) / 2.f;
        constexpr float bottom = top - Config::Window::windowHeight;
        this->renderer.addOrthographicCamera(left, right, bottom, top);
        Engine::World world(this->renderer);

        this->initImgui();

        while (!_done) {
            this->events();
            this->processInput();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            this->cameraGuiInfo();

            this->renderer.clear();
            world.update();
            world.render(shaders);
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(_window);
        }
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    SdlApplication::~SdlApplication() {
        SDL_GL_DestroyContext(_context);
        SDL_DestroyWindow(_window);
        SDL_Quit();

    }

    void SdlApplication::initImgui() const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui_ImplSDL3_InitForOpenGL(_window, _context);
        ImGui_ImplOpenGL3_Init();
    }

    void SdlApplication::events() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE)) {
                _done = true;
            }
        }
    }

    void SdlApplication::processInput() {
        const bool* keyStates = SDL_GetKeyboardState(nullptr);
        if (keyStates[SDL_SCANCODE_W])
        {
            this->renderer.cameras.at("main").position.second += 2.f;
        }
        if (keyStates[SDL_SCANCODE_S])
        {
            this->renderer.cameras.at("main").position.second -= 2.f;
        }
        if (keyStates[SDL_SCANCODE_D])
        {
            this->renderer.cameras.at("main").position.first += 2.f;
        }
        if (keyStates[SDL_SCANCODE_A])
        {
            this->renderer.cameras.at("main").position.first -= 2.f;
        }
    }

    void SdlApplication::cameraGuiInfo() {
        ImGui::Begin("Diagnostic");
        ImGui::Text("X: %f Y: %f", this->renderer.cameras.at("main").position.first, this->renderer.cameras.at("main").position.second);
        if (ImGui::Button("Reset")) {
            this->renderer.cameras.at("main").position.first = 0;
            this->renderer.cameras.at("main").position.second = 0;
        }
        ImGui::Separator();
        ImGui::End();
    }
}
