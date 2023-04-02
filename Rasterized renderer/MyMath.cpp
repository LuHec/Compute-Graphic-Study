#include "MyMath.h"

float MyMath::clamp(float minV, float maxV, float value)
{
	float res = value;
	if (value < minV)
	{
		res = minV;
	}
	else if (value > maxV)
	{
		res = maxV;
	}

	return res;
}

void MyMath::Bresenham(int x0, int y0, int x1, int y1, Buffer<vector3>* frame_buffer)
{
	//int dx = x1 - x0;
	//int dy = y1 - y0;
	//float k = abs(dy / dx);		// Ð±ÂÊ
	//float e = -0.5;
	//int xi = x0, yi = y0;
	//int sx = dx > 0 ? 1 : -1;
	//int sy = dy > 0 ? 1 : -1;
	//while ((*frame_buffer)[xi][yi] = { 255, 255, 255 }, xi != x1)
	//{	
	//	e = e + k;
	//	if (e > 0)
	//	{
	//		yi += sy;
	//		e -= 1;
	//	}

	//	xi += sx;
	//}

	// from https://blog.csdn.net/qq_41883085/article/details/102706471

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int erro = (dx > dy ? dx : -dy) / 2;
	
	while ((*frame_buffer)[x0][y0] = { 255, 255, 255 }, x0 != x1 || y0 != y1)
	{
		int e2 = erro;
		if (e2 > -dx) { erro -= dy; x0 += sx; }
		if (e2 < dy) { erro += dx; y0 += sy; }
	}
}
