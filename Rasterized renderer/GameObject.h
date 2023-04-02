#pragma once

#include "Component.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject() = default;
	GameObject(Transform trans);
	GameObject(Transform trans, Material mat);
	GameObject(Transform trans, Mesh m);
	GameObject(Transform trans, Mesh m, Material mat);
	~GameObject();

	Component component;
	Material material;
	Mesh mesh;
};