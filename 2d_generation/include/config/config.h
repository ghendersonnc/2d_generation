#pragma once
#include <cstdint>
#include <string>

namespace Fw::Config
{
    namespace Window
    {
        constexpr uint16_t windowWidth = 1280;
        constexpr uint16_t windowHeight = 720;
        inline std::string windowTitle = "Helloworld";
    }

    namespace Chunk
    {
        constexpr uint16_t chunkSize = 32;
    }
}