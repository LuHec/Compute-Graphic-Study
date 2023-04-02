#pragma once
#include <Eigen>

struct Transform
{
	Eigen::Vector3f position{ 0, 0, 0 };
	Eigen::Vector3f rotation{0, 0, 0};
	Eigen::Vector3f scale {1,1,1};
};