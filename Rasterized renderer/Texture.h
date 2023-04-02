#pragma once
#include "Color.h"
#include "Resource.h"
#include "MyMath.h"

class Texture
{
public:
	Color Sample(float u, float v) const
	{
		/*int int_u = MyMath::clamp(0, 1, u);
		int int_v = MyMath::clamp(0, 1, v);
		return (*texCol)[int_u][int_v];*/
		return (*texCol)[0][0];
	}


	std::string Name;

private:
	Buffer<Color>* texCol = nullptr;
};

