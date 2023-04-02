#include "Material.h"

Material::Material()
{
	shader = new PhongShader();
}

void Material::UpdateObjBuffer(ObjBuffer obuffer)
{
	shader->UpdateObjBuffer(obuffer);
}

void Material::UpdateCBuffer(Cbuffer cbuffer)
{
	shader->UpdateCbuffer(cbuffer);
}
