#include "Material.h"

Material::Material()
{
	shader = new PhongShader();
}

void Material::UpdateObjBuffer(const ObjBuffer& obuffer)
{
	shader->UpdateObjBuffer(obuffer);
}

void Material::UpdateCBuffer(const Cbuffer& cbuffer)
{
	shader->UpdateCbuffer(cbuffer);
}
