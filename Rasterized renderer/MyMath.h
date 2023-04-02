#pragma once
#include "Resource.h"
namespace MyMath
{
	float clamp(float minV, float maxV, float value);

	void Bresenham(int x1, int y1, int x2, int y2, Buffer<vector3>* frame_buffer);
}

