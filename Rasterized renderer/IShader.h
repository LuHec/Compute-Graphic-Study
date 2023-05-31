#pragma once

// TODO
// 1.VS 输入单个顶点
// 2.PS	输入单个像素
// 3.Shadow
#include <vector>
#include "Resource.h"
#include "Mesh.h"
#include "Color.h"
#include "MyMath.h"

struct VertexOut
{
	Eigen::Vector4f ScreenPos;
	Eigen::Vector3f worldPos;
	Eigen::Vector3f normal;
	Eigen::Vector2f uv;

	VertexOut operator*(const float weight) const
	{
		return VertexOut
		{
			this->ScreenPos * weight,
			this->worldPos * weight,
			this->normal * weight,
			this->uv * weight
		};
	}

	VertexOut operator+(const VertexOut& other) const
	{
		return VertexOut
		{
			this->ScreenPos + other.ScreenPos,
			this->worldPos + other.worldPos,
			this->normal + other.normal,
			this->uv + other.uv
		};
	}
};

class Shader
{
public:
	Shader() = default;
	virtual VertexOut VS(vertex v) { return VertexOut(); }
	virtual Color PS(VertexOut in) { return {}; }
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

	virtual VertexOut VS(vertex v) override;
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
