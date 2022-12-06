#include <iostream>
#include "../include/AppWindow.h"
#include "../include/Examples.h"
#include "../include/Functions.h"
#include "../include/MImageCalculator.h"
#include "imgui.h"
#include "../include/Texture.h"

float MyAwesomeFunction(SDL_Point arg)
{
	const SDL_Point headPos{300, 300};
	const float headRadius = 72;
	const SDL_Point ear1Pos{headPos.x + 60, headPos.y - 60};
	const SDL_Point ear2Pos{headPos.x - 60, ear1Pos.y};
	const float earRadius = 48;
	const SDL_Point eye1Pos{headPos.x - 30, headPos.y - 20};
	const SDL_Point eye2Pos{headPos.x + 30, headPos.y - 20};
	const float eyeRadius = 16;
	const SDL_Point mouthPos {headPos.x, headPos.y + 20};
	const SDL_Point mouthPos2 {headPos.x, headPos.y + 40};
	const float mouthRadius = 24;

	using namespace Functions;

	float mouth1 = Circle(mouthPos, mouthRadius, arg);
	float mouth2 = -Circle(mouthPos2, mouthRadius, arg);

	float head = Circle(headPos, headRadius, arg);

	float ear1 = Circle(ear1Pos, earRadius, arg);
	float ear2 = Circle(ear2Pos, earRadius, arg);

	float eye1 = Circle(eye1Pos, eyeRadius, arg);
	float eye2 = Circle(eye2Pos, eyeRadius, arg);

	float mouth = RAnd(mouth1, -mouth2);

	float result = RAnd(RAnd(RAnd(ROr(ROr(head, ear1), ear2), -eye1), -eye2), -mouth);
	
	return result;
}

class Window: public AppWindow
{
public:
	Window(SDL_Point windowSize):
		AppWindow(windowSize),
		texture(GetRenderer(), windowSize),
		calculator(&MyAwesomeFunction)
	{
		ComputeFunction();
	}

	void ComputeFunction()
	{
		std::vector<std::vector<MImagePixelData>> data = calculator.GetSpaceData(texture.GetSize());
		for (int i = 0; i < data.size(); ++i)
		{
			for (int j = 0; j < data[i].size(); ++j)
			{
				MImagePixelData mimageData = data[i][j];
				uint8_t colorValue = (mimageData.nx + 1.f) * 127.f;
				if (mimageData.zone == FunctionZone::Positive)
					texture.SetPixel(SDL_Point{i, j}, SDL_Color{colorValue, 0, 0, 255});
				else if (mimageData.zone == FunctionZone::Negative)
					texture.SetPixel(SDL_Point{i, j}, SDL_Color{0, 0, colorValue, 255});
			}
		}
		texture.UpdateTexture();
	}
	
	void Render() override
	{
		texture.Render();
	}
	
	void RenderGui() override
	{
		ImGui::Begin("MyWindow");
		ImGui::Text("Mouse position");
		ImGui::Text("\t%d, %d", mousePosition.x, mousePosition.y);
		ImGui::End();
	}
	
	void ProcessEvent(const SDL_Event& e) override
	{
		if (e.type == SDL_MOUSEMOTION)
		{
			mousePosition.x = e.motion.x;
			mousePosition.y = e.motion.y;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				SDL_Point startPoint;
				startPoint.x = e.button.x;
				startPoint.y = e.button.y;
				GradientDescent(startPoint, 2);
			}
		}
	}

	void GradientDescent(SDL_Point startPoint, int step, SDL_Color pathColor = {0, 255, 0, 255})
	{
		// Спускаемся пока не выйдем за пределы пространства текстуры
		while(startPoint.x >= 0 && startPoint.x < texture.GetSize().x &&
			startPoint.y >= 0 && startPoint.y < texture.GetSize().y)
		{
			// Проверяем, не попали ли мы в точку, которая уже была рассчитана
			SDL_Color newColor = texture.GetColor({startPoint.x, startPoint.y});
			if (newColor.r == pathColor.r &&
				newColor.g == pathColor.g &&
				newColor.b == pathColor.b &&
				newColor.a == pathColor.a)
			{
				break;
			}
			
			// Окрашивание пути
			texture.SetPixel({startPoint.x, startPoint.y}, pathColor);
			
			// Рассчет ЛГХ в текущей точке
			MImagePixelData data = calculator.GetPixelData({startPoint.x, startPoint.y});
				// Движение по направлению убывания с шагом
			startPoint.x -= step * data.nx;
			startPoint.y += step * data.ny;
		}
		texture.UpdateTexture();
	}
	
	Texture texture;
	MImageCalculator calculator;
	SDL_Point mousePosition;
};




int main(int argc, char** argv)
{
	Window window({800, 600});
	
	window.Show();
	
	return 0;
}