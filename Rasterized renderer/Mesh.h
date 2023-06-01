#pragma once

#include <vector>
#include <string>
#include <Eigen>

struct vertex
{
	vertex() = default;
	vertex(Eigen::Vector3f p, Eigen::Vector3f n = Eigen::Vector3f(0, 1, 0), Eigen::Vector2f u = Eigen::Vector2f(0, 0)) : pos(p), normal(n), uv(u)
	{

	}

	vertex(
		float px, float py, float pz,
		float nx, float ny, float nz) :
		pos(Eigen::Vector3f(px, py, pz)),
		normal(Eigen::Vector3f(nx, ny, nz)) {}

	vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		pos(Eigen::Vector3f(px, py, pz)),
		normal(Eigen::Vector3f(nx, ny, nz)),
		uv(Eigen::Vector2f(u, v)) {}

	Eigen::Vector3f pos;
	Eigen::Vector3f normal;
	Eigen::Vector2f uv;
};


// ¶¥µã¡¢¶¥µãË÷Òý
struct Mesh
{
	std::vector<vertex> vertices;
	std::vector<int> indices;
};