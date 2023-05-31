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

MyMath::barycoord MyMath::barycentric(const vector2& A, const vector2& B, const vector2& C, const vector2& P)
{
	// from https://zhuanlan.zhihu.com/p/65495373
	Eigen::Vector3f v1(B.x - A.x, C.x - A.x, A.x - P.x);
	Eigen::Vector3f v2(B.y - A.y, C.y - A.y, A.y - P.y);
	auto u = v1.cross(v2);
	
	// aAB + bAC + cPA = 0 => P = (1 - a/c - b/c)A + (a/c)B + (b/c)C
	return {(1 - (u.x() + u.y()) / u.z()), u.x() / u.z(), u.y() / u.z()};
}
