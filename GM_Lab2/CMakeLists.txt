cmake_minimum_required(VERSION 3.0.0)
project(Lab2 VERSION 0.1.0)

set(IMGUI_DIR "C:/Users/fenryux/lib/imgui")
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

find_package(SDL2 REQUIRED)

add_executable(Lab2
                src/main.cpp
                src/AppWindow.cpp
                src/Texture.cpp
                ${PROJECT_SOURCE_DIR}/include/AppWindow.h
                ${PROJECT_SOURCE_DIR}/include/Examples.h
                ${PROJECT_SOURCE_DIR}/include/Texture.h
                ${PROJECT_SOURCE_DIR}/include/Functions.h
                ${IMGUI_DIR}/imconfig.h
                ${IMGUI_DIR}/backends/imgui_impl_sdl.h
                ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer.h
                ${IMGUI_DIR}/imgui_internal.h
                ${IMGUI_DIR}/imgui.h
                ${IMGUI_DIR}/imstb_rectpack.h
                ${IMGUI_DIR}/imstb_textedit.h
                ${IMGUI_DIR}/imstb_truetype.h
                ${IMGUI_DIR}/imgui_demo.cpp
                ${IMGUI_DIR}/imgui_draw.cpp
                ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
                ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer.cpp
                ${IMGUI_DIR}/imgui_tables.cpp
                ${IMGUI_DIR}/imgui_widgets.cpp
                ${IMGUI_DIR}/imgui.cpp)

target_include_directories(Lab2
    PUBLIC ${SDL2_INCLUDE_DIRS} ${IMGUI_SOURCES})

target_link_libraries(Lab2 PUBLIC ${SDL2_LIBRARIES})
target_link_options(Lab2 PRIVATE -mwindows)

