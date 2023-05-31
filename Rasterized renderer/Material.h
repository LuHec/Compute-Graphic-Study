#pragma once

#include <vector>
#include "Texture.h"
#include "IShader.h"

class Material
{
public:
	Material();
	void UpdateObjBuffer(const ObjBuffer& obuffer);
	void UpdateCBuffer(const Cbuffer& cbuffer);
	Shader* shader = nullptr;
	std::vector<Texture> textures;
};
