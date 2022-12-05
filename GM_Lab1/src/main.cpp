#include <iostream>
#include <vector>

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius){
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
    return status;
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL init falure\n";
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    SDL_Window* window = SDL_CreateWindow("Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cout << "Couldn't create window\n";
        return -1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Couldn't create renderer\n";
        return -1;
    }

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    bool programWorks = true;
    int fps = 60;
    Uint64 trueFrameTime = 1000.f / fps;

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;

    int windowWidth = SDL_GetWindowSurface(window)->w;
    int windowHeight = SDL_GetWindowSurface(window)->h;
    SDL_Color color[windowHeight];

    for(int i = 0; i < windowHeight; i++){
        color[i].r = (i + int(float(windowHeight / 256))) > 255 ? 255 : i + int(float(windowHeight / 256));
        color[i].g = (i + int(float(windowHeight / 256))) > 255 ? 255 : i + int(float(windowHeight / 256));
        color[i].b = 0;
        color[i].a = 0;
    }

    while (programWorks)
    {
        Uint64 frameTime = SDL_GetTicks();
        // HandleInputs
        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                ImGui_ImplSDL2_ProcessEvent(&e);
                if (e.type == SDL_QUIT)
                    programWorks = false;
            }
        }

        // UpdateImage
        {
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            // Render Gui
            {
                ImGui::Begin("Window", nullptr, window_flags);
                // ImGui::ShowDemoWindow();
                if(ImGui::Button("Linear Gradient")){
                    for(int i = 0; i < windowHeight; i++){
                        SDL_SetRenderDrawColor(renderer, color[i].r, color[i].g , color[i].b, color[i].a);
                        SDL_RenderDrawLine(renderer, 0, i, windowWidth, i);
                    }
                }
                if(ImGui::Button("Circular Gradient")){
                    for(int i = windowHeight; i > 0; i--){
                        SDL_SetRenderDrawColor(renderer, color[i].r, color[i].g , color[i].b, color[i].a);
                        SDL_RenderFillCircle(renderer, windowWidth / 2, windowHeight / 2, i);
                    }
                }

                ImGui::End();
            }

            ImGui::Render();

            // SDL_RenderClear(renderer);

            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);
        }
        
        frameTime = SDL_GetTicks() - frameTime;
        if (frameTime < trueFrameTime)
            SDL_Delay(trueFrameTime - frameTime);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}