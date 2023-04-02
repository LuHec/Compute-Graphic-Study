#pragma once

#include <Eigen>
#include "Transform.h"

constexpr float PI = 3.1415926;

class Camera
{
public:
	Camera() = default;
	Camera(Transform trans, int wid, int hei, float near, float far, float fov);

	void UpdateMatrix();	
	float GetNearPlane();
	float GetFarPlane();
	void SetWidthHeight(int wid, int hei);
	void SetFov(float fov);
	void SetNearFar(float near, float far);
	void SetDirection(Eigen::Vector3f dir);

	Eigen::Matrix4f GetViewMatrix();
	Eigen::Matrix4f GetProjMatrix();
	Eigen::Matrix4f GetScreenMatrix();
	Eigen::Matrix4f GetViewProjMatrix();

	~Camera() = default;

	Transform transform;
	

private:
	void SetView();
	void SetProj();
	void SetScreen();


	float aspect, eye_fov;
	float zNear, zFar;
	int width, height;

	Eigen::Vector3f camera_dir;

	Eigen::Matrix4f m_view;
	Eigen::Matrix4f m_proj;
	Eigen::Matrix4f m_screen;
	Eigen::Matrix4f m_viewProj;

};