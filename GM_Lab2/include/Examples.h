﻿#pragma once
#include "Functions.h"
#include "Texture.h"
#include "math.h"

void Example1(Texture& texture)
{
	// Задаем позицию нашей окружности на текстуре
	const SDL_FPoint circlePos = {(float)texture.GetSize().x/2, (float)texture.GetSize().y/2};
	// Радиус окружности
	float circleRadius = std::max(texture.GetSize().x, texture.GetSize().y)/4.f;

	// Проходимся по массиву пикселей в текстуре
	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			// Считаем значение функции окружности
			float value = Functions::Circle(circlePos, circleRadius, SDL_FPoint{(float)x, (float)y});
			// Внутреннюю зону красим в красный цвет, наружнюю в синий
			if (value > 0.f)
				texture.SetPixel(SDL_Point{x, y}, SDL_Color{255, 100, 100, 255});
			else
				texture.SetPixel(SDL_Point{x, y}, SDL_Color{100, 100, 255, 255});
		}
	}
	texture.UpdateTexture();
}

void Example2(Texture& texture)
{
	const SDL_Point circlePos = {texture.GetSize().x/2, texture.GetSize().y/2};
	float circleRadius = std::max(texture.GetSize().x, texture.GetSize().y)/4.f;

	float minVal = 0, maxVal = 0;
	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			float z = Functions::Circle(circlePos, circleRadius, SDL_Point{x, y});
			if (z > maxVal)
				maxVal = z;
			if (z < minVal)
				minVal = z;
		}
	}
	
	// Проходимся по массиву пикселей в текстуре
	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			// Считаем значение функции окружности
			float z = Functions::Circle(circlePos, circleRadius, SDL_Point{x, y});
			float max = z / maxVal;
			float min = z / minVal;
			// Внутреннюю зону красим в красный цвет, наружнюю в синий
			if (z > 0.f)
				texture.SetPixel(SDL_Point{x, y}, SDL_Color{(Uint8)(max * 255), (Uint8)(max * 50), (Uint8)(max * 50), 255});
			else
				texture.SetPixel(SDL_Point{x, y}, SDL_Color{(Uint8)(min * 50), (Uint8)(min * 50), (Uint8)(min * 255), 255});
		}
	}
	texture.UpdateTexture();
}

void Example3(Texture& texture)
{
	// Задаем позицию нашей окружности на текстуре
	const SDL_Point circlePos = {texture.GetSize().x/2, texture.GetSize().y/2};
	
	// Радиус окружности
	float circleRadius = std::max(texture.GetSize().x, texture.GetSize().y)/4.f;

	float minVal = 0, maxVal = 0;
	// Проходимся по массиву пикселей в текстуре
	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			// Расчитываем площадку из трех значений
			int x1 = x,	 y1 = y;
			int x2 = x,	 y2 = y + 1;
			int x3 = x + 1, y3 = y;
			float z1 = Functions::Circle(circlePos, circleRadius, SDL_Point{x1, y1});
			float z2 = Functions::Circle(circlePos, circleRadius, SDL_Point{x2, y2});
			float z3 = Functions::Circle(circlePos, circleRadius, SDL_Point{x3, y3});
			
			// Считаем дискременанты
			float Ai = y1 * (z2 - z3) - y2 * (z1 - z3) + y3 * (z1 - z2);
			float Bi = -(x1 * (z2 - z3) - x2 * (z1 - z3) + x3 * (z1 - z2));
			float Ci = x1 * (y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2);
			float Di = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 * (y1 * z2 - y2 * z1);

			// Находим компоненты нормалей к площадке
			float Nx =  Ai / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Ny = -Bi / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nz = -Ci / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nt =  Di / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci + Di * Di);
			
			// Оисуем значения нормали по оси Х
			Uint8 Cx = (Nx + 1.f) * 127.f;
			int isPositive = (z1 >= 0) + (z2 >= 0) + (z3 >= 0);
			if (isPositive > 2) // больше половины значений положительные
				texture.SetPixel({x, y}, SDL_Color{Cx, 0, 0, 255});
			else
				texture.SetPixel({x, y}, SDL_Color{0, 0, Cx, 255});
		}
	}
	texture.UpdateTexture();
}

void Example4(Texture& texture)
{
	auto ComplexFigure = [&texture](SDL_Point point) -> float
	{
		const SDL_Point circlePos1{texture.GetSize().x/3, texture.GetSize().y/3};
		const float circleRadius1 = std::max(texture.GetSize().x, texture.GetSize().y)/4;
		const SDL_Point circlePos2{texture.GetSize().x/2, texture.GetSize().y/3*2};
		const float circleRadius2 = std::max(texture.GetSize().x, texture.GetSize().y)/4;
		
		float c1 = Functions::Circle(circlePos1, circleRadius1, point);
		float c2 = Functions::Circle(circlePos2, circleRadius2, point);
		float res = Functions::ROr(c1, c2);
		
		return res;
	};

	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			int x1 = x,	 y1 = y;
			int x2 = x,	 y2 = y + 1;
			int x3 = x + 1, y3 = y;
			float z1 = ComplexFigure({x1, y1});
			float z2 = ComplexFigure({x2, y2});
			float z3 = ComplexFigure({x3, y3});
				
			float Ai = y1 * (z2 - z3) - y2 * (z1 - z3) + y3 * (z1 - z2);
			float Bi = -(x1 * (z2 - z3) - x2 * (z1 - z3) + x3 * (z1 - z2));
			float Ci = x1 * (y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2);
			float Di = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 * (y1 * z2 - y2 * z1);
				
			float Nx =  Ai / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Ny = -Bi / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nz = -Ci / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nt =  Di / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci + Di * Di);
				
			uint8_t Cx = (Nx + 1.f) * 127.f;
			int zoneFlag = (z1 >= 0) + (z2 >= 0) + (z3 >= 0);
			if (zoneFlag > 2)
				texture.SetPixel({x, y}, SDL_Color{Cx, 0, 0, 255});
			else
				texture.SetPixel({x, y}, SDL_Color{0, 0, Cx, 255});
		}
	}
	texture.UpdateTexture();
}

void Example_Mouse(Texture& texture){
	auto ComplexFigure = [&texture](SDL_Point point) -> float {
		const SDL_Point headPos{texture.GetSize().x/2, texture.GetSize().y/2};
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

		float mouth1 = Circle(mouthPos, mouthRadius, point);
		float mouth2 = -Circle(mouthPos2, mouthRadius, point);

		float head = Circle(headPos, headRadius, point);

		float ear1 = Circle(ear1Pos, earRadius, point);
		float ear2 = Circle(ear2Pos, earRadius, point);

		float eye1 = Circle(eye1Pos, eyeRadius, point);
		float eye2 = Circle(eye2Pos, eyeRadius, point);

		float mouth = RAnd(mouth1, -mouth2);

		float result = RAnd(RAnd(RAnd(ROr(ROr(head, ear1), ear2), -eye1), -eye2), -mouth);
		
		return result;
	};

	for (int y = 0; y < texture.GetSize().y; ++y)
	{
		for (int x = 0; x < texture.GetSize().x; ++x)
		{
			int x1 = x,	 y1 = y;
			int x2 = x,	 y2 = y + 1;
			int x3 = x + 1, y3 = y;
			float z1 = ComplexFigure({x1, y1});
			float z2 = ComplexFigure({x2, y2});
			float z3 = ComplexFigure({x3, y3});
				
			float Ai = y1 * (z2 - z3) - y2 * (z1 - z3) + y3 * (z1 - z2);
			float Bi = -(x1 * (z2 - z3) - x2 * (z1 - z3) + x3 * (z1 - z2));
			float Ci = x1 * (y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2);
			float Di = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 * (y1 * z2 - y2 * z1);
				
			float Nx =  Ai / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Ny = -Bi / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nz = -Ci / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
			float Nt =  Di / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci + Di * Di);
				
			uint8_t Cx = (Nx + 1.f) * 127.f;
			int zoneFlag = (z1 >= 0) + (z2 >= 0) + (z3 >= 0);
			if (zoneFlag > 2)
				texture.SetPixel({x, y}, SDL_Color{Cx, 0, 0, 255});
			else
				texture.SetPixel({x, y}, SDL_Color{0, 0, Cx, 255});
		}
	}
	texture.UpdateTexture();
}
