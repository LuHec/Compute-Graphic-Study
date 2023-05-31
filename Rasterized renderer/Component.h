#pragma once

#include "Transform.h"
#include <Eigen>

class Component
{
public:
	Component();
	Component(Transform trans);
	~Component();

	Eigen::Matrix4f GetWorldMatrix();

	Transform transform;
	

private:
	void UpdateMatrix();


	Eigen::Matrix4f WorldMatrix;
};