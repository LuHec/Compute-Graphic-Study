#pragma once

// TODO
// 1.VS ���뵥������
// 2.PS	���뵥������
// 3.Shadow
#include <vector>
#include "Resource.h"
#include "Mesh.h"
#include "Color.h"

struct VertexOut
{
	Eigen::Vector4f ScreenPos;
	Eigen::Vector3f worldPos;
	Eigen::Vector3f normal;
	Eigen::Vector2f uv;
};

class Shader
{
public:
	Shader() = default;
	virtual VertexOut* VS(vertex v) { return new VertexOut(); }
	virtual Color PS(VertexOut in) { return Color(0); }
	virtual void UpdateCbuffer(const Cbuffer cbuffer) {}
	virtual void UpdateObjBuffer(const ObjBuffer objbuffer) {}
protected:
	ObjBuffer obuffer;
	Cbuffer cbuffer;
};


class PhongShader : public Shader
{
public:
	PhongShader() = default;
	~PhongShader() = default;

	virtual VertexOut* VS(vertex v) override;
	virtual Color PS(VertexOut in) override;
	virtual void UpdateCbuffer(const Cbuffer cbuffer) override
	{
		this->cbuffer.CameraPos = cbuffer.CameraPos;
		this->cbuffer.ProjectMatrix = cbuffer.ProjectMatrix;
		this->cbuffer.ScreenMatrix = cbuffer.ScreenMatrix;
		this->cbuffer.ViewMatrix = cbuffer.ViewMatrix;
		this->cbuffer.light = cbuffer.light;
	}
	virtual void UpdateObjBuffer(const ObjBuffer objbuffer) override
	{
		obuffer.WorldMatrix = objbuffer.WorldMatrix;
		obuffer.InvTransWorldMatrix = obuffer.WorldMatrix.inverse().transpose();
	}

private:
	struct PhongVertexOut : VertexOut
	{
		
	};
};

//void PhongShader::UpdateCbuffer(const Cbuffer cbuffer)
//{
//	this->cbuffer.CameraPos = cbuffer.CameraPos;
//	this->cbuffer.ProjectMatrix = cbuffer.ProjectMatrix;
//	this->cbuffer.ScreenMatrix = cbuffer.ScreenMatrix;
//	this->cbuffer.ViewMatrix = cbuffer.ViewMatrix;
//	this->cbuffer.light = cbuffer.light;
//}
//
//void PhongShader::UpdateObjBuffer(const ObjBuffer objbuffer)
//{
//	obuffer.WorldMatrix = objbuffer.WorldMatrix;
//	obuffer.InvTransWorldMatrix = obuffer.WorldMatrix.inverse().transpose();
//}