cmake_minimum_required (VERSION 3.20)
project(sdl_learning)

set(CMAKE_CXX_STANDARD 17)

# 3rd party
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
add_subdirectory("vendor/glad")

# own
add_subdirectory(fw_graphics)
add_subdirectory(fw_utility)

add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/2d_generation/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/2d_generation/include/engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/2d_generation/include/game"
)

set(ENGINE_SOURCES
    "2d_generation/src/engine/sdl_application.cpp"
    "2d_generation/src/engine/chunk.cpp"
    "2d_generation/src/engine/world.cpp"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "2d_generation/src/main.cpp"
    "2d_generation/src/meshes/chunk_mesh.cpp"
    ${ENGINE_SOURCES}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/2d_generation/resources/")

# hot reload for Visual Studio
if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/ZI>)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/INCREMENTAL>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/ZI>)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    SDL3::SDL3
    glm::glm
    fw_graphics
    fw_utility
)
