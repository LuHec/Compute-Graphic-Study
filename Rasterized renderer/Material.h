#pragma once

#include <vector>
#include "Texture.h"
#include "IShader.h"

class Material
{
public:
	Material();
	void UpdateObjBuffer(ObjBuffer obuffer);
	void UpdateCBuffer(Cbuffer cbuffer);
	Shader* shader = nullptr;
	std::vector<Texture> textures;
};
