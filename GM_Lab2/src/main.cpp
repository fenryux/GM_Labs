#include <iostream>
#include "../include/AppWindow.h"
#include "../include/Examples.h"
#include "../include/Functions.h"
#include "imgui.h"
#include "../include/Texture.h"


class Window: public AppWindow
{
public:
	Window(SDL_Point windowSize):
		AppWindow(windowSize),
		texture(GetRenderer(), windowSize)
	{
	}
	
	void Render() override
	{
		texture.Render();
	}
	
	void RenderGui() override
	{
		ImGui::Begin("MyWindow");
		if (ImGui::Button("Example 1"))
			Example1(texture);
		if (ImGui::Button("Example 2"))
			Example2(texture);
		if (ImGui::Button("Example 3"))
			Example3(texture);
		if (ImGui::Button("Example 4"))
			Example4(texture);
		if (ImGui::Button("Example Mouse"))
			Example_Mouse(texture);
		ImGui::End();
	}
	
	void ProcessEvent(const SDL_Event& e) override
	{
		
	}
	
	Texture texture;
};


int main(int argc, char** argv)
{
	Window window({800, 600});
	
	window.Show();
	
	return 0;
}