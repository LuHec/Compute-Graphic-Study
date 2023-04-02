#pragma once

#include <Eigen>
#include "GameObject.h"
#include "Resource.h"
#include "Camera.h"
#include "Light.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>

class Renderer
{
public:
	Renderer();
	Renderer(int width, int height, float nearPlan, float farPlan, float fov);
	~Renderer();

	void LoadObject(GameObject* object);		// 传入Object
	void Draw();

// Render Pipeline
// Space: obj->world->view->clip->cvv->ndc->screen
// Op: (vertex shader)mvp->proj->||cvv clip->/w ndc -> screen ||-> Triangle Traversal + (fragment) + per fragment(ztest)
private:
	void Init();			// 初始化
	void Clear();			// 清空缓冲区 
	void Update();
	void UpdateConstantBuffer();
	void UpdateObjectBuffer(GameObject*);
	void DrawRenderItems(); // 逐个绘制items
	void DrawIndexdInstance(const Mesh& mesh, const Material& mat); // Draw Call
	void OutPut();

	void VertexShader();	// 顶点着色器
	void ShadowReceive();	// 阴影收集
	void Rasterlize();		// 光栅化
	void FragmentShader();	// 片元着色器
	
	bool CvvCull(const Eigen::Vector4f v, Camera camera);
	void ProjDivid(Eigen::Vector4f& screenPos);
	void Ndc2Screen(Eigen::Vector4f& screenPos, Eigen::Matrix4f screenMatrix);
	void Zwrite_test();

	void SetPixel();
	void DrawLine(vector2 v1, vector2 v2, vector2 v3);
	void DrawPoint(vector2 v1, vector2 v2, vector2 v3);
	void DrawTriangle();
	void DrawPrimitive();

	void CpuCullAndUpdateObjBuffers();

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


// Object Members
private:
	Camera camera;
	Light direct_light;
	Cbuffer globalCbuffer;
	std::vector<GameObject*> objList;
	std::vector<GameObject*> renderList;

private:
	void SetCamera();
	void SetBuffers();
};