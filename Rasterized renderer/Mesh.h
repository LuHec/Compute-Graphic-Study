#pragma once

#include <vector>
#include <string>
#include <Eigen>

struct vertex
{
	vertex(Eigen::Vector3f p, Eigen::Vector3f n = Eigen::Vector3f(0, 1, 0), Eigen::Vector2f u = Eigen::Vector2f(0, 0)) : pos(p), normal(n), uv(u)
	{

	}
	Eigen::Vector3f pos;
	Eigen::Vector3f normal;
	Eigen::Vector2f uv;
};


// ¶¥µã¡¢¶¥µãË÷Òý
struct Mesh
{
	std::vector<vertex> vertexs;
	std::vector<int> indices;
};