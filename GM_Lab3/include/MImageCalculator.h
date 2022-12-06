#pragma once

#include <SDL.h>
#include <math.h>
#include <vector>
#include <functional>

enum MImageType
{
	Cx, Cy, Cz, Cd
};

enum FunctionZone
{
   Negative, Positive
};

struct MImagePixelData
{
	double nx, ny, nz, nd;
	FunctionZone zone;
};

class MImageCalculator
{
public:
	MImageCalculator(const std::function<float(SDL_Point pixel)>& computeFunction);
	std::vector<std::vector<MImagePixelData>> GetSpaceData(SDL_Point spaceSize);
	MImagePixelData GetPixelData(SDL_Point point);
	
private:
	std::function<float(SDL_Point)> _computeFunction;
};
