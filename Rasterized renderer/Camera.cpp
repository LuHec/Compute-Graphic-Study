#include "Camera.h"
#include "MyMath.h"

Camera::Camera(Transform trans, int wid, int hei, float near, float far, float fov) : transform(trans), width(wid), height(hei), zNear(near), zFar(far), eye_fov(fov)
{
	aspect = width / height;
	camera_dir = Eigen::Vector3f(0, 0.2, -1);
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	SetView();
	SetProj();
	SetScreen();
	m_viewProj = m_proj * m_view;
}

float Camera::GetNearPlane()
{
	return zNear;
}

float Camera::GetFarPlane()
{
	return zFar;
}

void Camera::SetWidthHeight(int wid, int hei) { width = wid; height = hei; aspect = width / height; }

void Camera::SetFov(float fov) { eye_fov = fov; }

void Camera::SetNearFar(float near, float far) { zNear = near; zFar = far; }

void Camera::SetDirection(Eigen::Vector3f dir) { camera_dir = dir.normalized(); }

Eigen::Matrix4f Camera::GetViewMatrix() 
{
	return m_view;
}

Eigen::Matrix4f Camera::GetProjMatrix()
{
	return m_proj;
}

Eigen::Matrix4f Camera::GetScreenMatrix()
{
	return m_screen;
}

Eigen::Matrix4f Camera::GetViewProjMatrix()
{
	return m_viewProj;
}

void Camera::SetView()
{
	Eigen::Vector3f up = { 0, 1, 0 };
	Eigen::Vector3f camera_right = up.cross(camera_dir).normalized();
	Eigen::Vector3f camera_up = camera_dir.cross(camera_right).normalized();
	Eigen::Matrix4f LookUp_T;
	
	// LookUp是把单位矩阵变到新坐标，而我们是要把相机坐标变到单位矩阵，故取逆矩阵。
	// 由于是正交基，所以逆矩阵就是转置矩阵
	LookUp_T << 
		camera_right.x(), camera_right.y(), camera_right.z(), 0,
		camera_up.x()	, camera_up.y()	  , camera_up.z()	, 0,
		camera_dir.x()  , camera_dir.y()  , camera_dir.z()  , 0,
		0               , 0		          , 0			    , 1;

	Eigen::Matrix4f Trans;
	Trans <<
		1, 0, 0, -transform.position.x(),
		0, 1, 0, -transform.position.y(),
		0, 0, 1, -transform.position.z(),
		0, 0, 0, 1;

	m_view = LookUp_T * Trans;
}

void Camera::SetProj()
{
	float fov_2_radians = eye_fov / 2 * MyMath::PI / 180;

	float n = zNear;
	float f = zFar;

	float t = tan(fov_2_radians) * n;
	float b = -t;
	float r = t * aspect;
	float l = -r;

	// 左手坐标系投影矩阵
	m_proj <<
		2 * n / (r - l), 0, 0, 0,
		0, 2 * n / (t - b), 0, 0,
		0, 0, (f + n) / (f - n), -(2 * n * f) / (f - n),
		0, 0, -1, 0;

}

void Camera::SetScreen()
{
	// 暂定，网上看到的矩阵有z轴的变化，我暂时没看出来原因
	m_screen <<
		width / 2  , 0			, 0	, transform.position.x() + width / 2,
		0		   , height / 2 , 0	, transform.position.y() + height / 2,
		0		   , 0			, 1	, 0         ,	
		0		   , 0			, 0	, 1         ;
}
