#include "../include/MImageCalculator.h"

MImageCalculator::MImageCalculator(const std::function<float(SDL_Point pixel)>& computeFunction):
	_computeFunction(computeFunction)
{
}

std::vector<std::vector<MImagePixelData>> MImageCalculator::GetSpaceData(SDL_Point spaceSize)
{
	// Matrix
	std::vector data(spaceSize.x, std::vector<MImagePixelData>(spaceSize.y));

	for (int x = 0; x < spaceSize.x; ++x)
	{
		for (int y = 0; y < spaceSize.y; ++y)
			data[x][y] = GetPixelData({x, y});
	}
	
	return data;
}

MImagePixelData MImageCalculator::GetPixelData(SDL_Point pixel){
    MImagePixelData pixelData;
    
    int x = pixel.x;
    int y = pixel.y;
    
    int x1 = x,	 y1 = y;
    int x2 = x,	 y2 = y + 1;
    int x3 = x + 1, y3 = y;
    float z1 = _computeFunction({x,y});
    float z2 = _computeFunction({x,y});
    float z3 = _computeFunction({x,y});
    
    // Считаем дискременанты
    float Ai = y1 * (z2 - z3) - y2 * (z1 - z3) + y3 * (z1 - z2);
    float Bi = -(x1 * (z2 - z3) - x2 * (z1 - z3) + x3 * (z1 - z2));
    float Ci = x1 * (y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2);
    float Di = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 * (y1 * z2 - y2 * z1);

    // Находим компоненты нормалей к площадке
    pixelData.nx =  Ai / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
    pixelData.ny = -Bi / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
    pixelData.nz = -Ci / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci);
    pixelData.nd =  Di / sqrtf(Ai * Ai + Bi * Bi + Ci * Ci + Di * Di);
    
    
    Uint8 Cx = (pixelData.nx + 1.f) * 127.f;
    int isPositive = (z1 >= 0) + (z2 >= 0) + (z3 >= 0);
    if (isPositive > 2) // больше половины значений положительные
        pixelData.zone = Positive;
    else
        pixelData.zone = Negative;

    return pixelData;
}