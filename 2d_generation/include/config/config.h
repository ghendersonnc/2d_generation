#pragma once
#include <cstdint>
#include <string>

namespace Fw::Config
{
    namespace Window
    {
        constexpr uint16_t windowWidth = 1280;
        constexpr uint16_t windowHeight = 720;
        constexpr int openGlMajorVersion = 3;
        constexpr int openGlMinorVersion = 3;
        inline std::string windowTitle = "factory";
    }

    namespace Chunk
    {
        constexpr uint16_t chunkSize = 32;
    }
    namespace Shader {
        enum Name : int16_t
        {
            CHUNK = 0,
            SPRITE
        };
    }
}