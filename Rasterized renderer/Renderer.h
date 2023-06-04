#pragma once

#include <Eigen>
#include <windows.h>
#include "GameObject.h"
#include "Resource.h"
#include "Camera.h"
#include "Light.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>


enum class RenderState
{
	OFF = 0,
	ON = 1
};

struct RenderPSO
{
	RenderState WireframeState = RenderState::OFF;
	RenderState TriangleState = RenderState::ON;
	RenderState PointState = RenderState::OFF;
};

class Renderer
{
public:
	Renderer();
	Renderer(int width, int height, float nearPlan, float farPlan, float fov);
	~Renderer();

	void SetHDC(HDC ohdc, HDC oscreenHDC);
	void LoadObject(GameObject* object);		// ����Object
	void Update();
	void SetCamera(const float x, const float y, const float z);
	void SetRenderPso(RenderPSO pso);
	void SetRenderPso(RenderState WireframeState, RenderState TriangleState, RenderState PointState);

// Render Pipeline
// Space: obj->world->view->clip->cvv->ndc->screen
// Op: (vertex shader)mvp->proj->||cvv clip->/w ndc -> screen ||-> Triangle Traversal + (fragment) + per fragment(ztest)
private:
	void Init();			// ��ʼ��
	void Draw();
	void UpdateConstantBuffer();
	void UpdateObjectBuffer(GameObject*);
	void DrawRenderItems(); // �������items
	void DrawInstanceIndexd(const Mesh& mesh, const Material& mat); // Draw Call
	void DrawGizmos();
	void OutPut();
	void WinOutPut();
	void Clear();			// ��ջ����� 

	void ShadowReceive();	// ��Ӱ�ռ�
	
	bool CvvCull(const Eigen::Vector4f v, Camera camera);
	void ProjDivid(Eigen::Vector4f& screenPos);
	void Ndc2Screen(Eigen::Vector4f& screenPos, const Eigen::Matrix4f& screenMatrix);
	bool Zwrite_test(int x, int y, float depth);
	bool InsideTriangle(float x, float y, const std::vector<Eigen::Vector3f>& tri);

	void DrawPixel(int x, int y, const Color& color);
	void DrawLine(const vector2& v1, const vector2& v2, const vector2& v3);
	void DrawPoint(const vector2& v);
	void DrawPoint(const vector2& v1, const vector2& v2, const vector2& v3);
	void DrawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3, Shader* shader);
	void DrawPrimitive();

	void CpuCullAndUpdateObjBuffers();

private:
	void SetCamera();
	void SetBuffers();

// Render Members
private:
	int width, height;
	float aspect, fov;
	float nearPlane, farPlane;
	
	Buffer<vector3>* frame_buffer = nullptr;
	Buffer<float>* z_buffer = nullptr;

	Eigen::Vector4f m_view;
	Eigen::Vector4f m_proj;
	Eigen::Vector4f m_screen;
	Eigen::Vector4f m_viewProj;

// Windows
private:
	HDC hdc;
	HDC screenHDC;

// Object Members
private:
	Camera camera;
	Light direct_light;
	Cbuffer globalCbuffer;
	std::vector<GameObject*> objList;
	std::vector<GameObject*> renderList;

// RenderState
private:
	RenderPSO renderPso;

public:
	static const std::string output_base_path;
};