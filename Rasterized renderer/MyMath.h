#pragma once
#include "Resource.h"
namespace MyMath
{
	constexpr float PI = 3.1415926;
	constexpr float div180 = 0.005555555555;

	float clamp(float minV, float maxV, float value);

	void Bresenham(int x1, int y1, int x2, int y2, Buffer<vector3>* frame_buffer);

	struct barycoord
	{
		float alpha, beta, gamma;
	};

	barycoord barycentric(const vector2& A, const vector2& B, const vector2& C, const vector2& P);
}

