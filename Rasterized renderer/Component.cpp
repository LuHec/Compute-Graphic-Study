#include "Component.h"
#include <iostream>

Component::Component()
{
	WorldMatrix = Eigen::Matrix4f::Identity();
}

Component::Component(Transform trans) : transform(trans)
{
	UpdateMatrix();
}

Component::~Component()
{
}

Eigen::Matrix4f Component::GetWorldMatrix()
{
	UpdateMatrix();
	return WorldMatrix;
}

void Component::UpdateMatrix()
{
	// 构造矩阵
	// 注意这个傻屌Eigen库double 和 float不能互相运算，后面AngleAxisf不能用AngleAxisd
	Eigen::Vector3f Eurler = transform.rotation;

	// 构造顺序为z，y，x
	Eigen::Matrix3f rotation_3;
	rotation_3 =
		Eigen::AngleAxisf(Eurler[2], Eigen::Vector3f::UnitZ()) *
		Eigen::AngleAxisf(Eurler[1], Eigen::Vector3f::UnitY()) *
		Eigen::AngleAxisf(Eurler[0], Eigen::Vector3f::UnitX());
	std::cout << 
		"rotation matrix :\n" <<
		rotation_3 << std::endl;

	Eigen::Matrix4f rotation;
	rotation <<
		rotation_3(0, 0), rotation_3(0, 1), rotation_3(0, 2), 0,
		rotation_3(1, 0), rotation_3(1, 1), rotation_3(1, 2), 0,
		rotation_3(2, 0), rotation_3(2, 1), rotation_3(2, 2), 0,
		0, 0, 0, 1;

	Eigen::Matrix4f position;
	position <<
		1, 0, 0, transform.position.x(),
		0, 1, 0, transform.position.y(),
		0, 0, 1, transform.position.z(),
		0, 0, 0, 1;

	Eigen::Matrix4f scale;
	scale <<
		transform.scale.x(), 0, 0, 0,
		0, transform.scale.y(), 0, 0,
		0, 0, transform.scale.z(), 0,
		0, 0, 0, 1;

	WorldMatrix = position * scale * rotation;
}
