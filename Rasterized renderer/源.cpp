#include <iostream>
#include <Eigen>
#include "ObjLoader.h"
#include "Renderer.h"
#include "IShader.h"
#include <windows.h>
#pragma comment (lib, "MSIMG32.LIB")

GameObject* AddTraingle()
{
	GameObject* gameobject = new GameObject();
	auto& mesh = gameobject->mesh;

	mesh.vertices.emplace_back(Eigen::Vector3f(0, 3, 0));
	mesh.vertices.emplace_back(Eigen::Vector3f(-1, 1, 0));
	mesh.vertices.emplace_back(Eigen::Vector3f(1, 1, 0));

	auto& vertices = mesh.vertices;
	vertices[0].normal = { 0,0, 1 };
	vertices[1].normal = { 0,0, 1 };
	vertices[2].normal = { 0,0, 1 };

	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(2);

	auto& rotation = gameobject->component.transform.rotation;
	rotation.x() = 0;
	rotation.y() = 0;
	rotation.z() = 0;

	auto& pos = gameobject->component.transform.position;
	pos.x() = -3;

	return gameobject;
}

GameObject* AddCube()
{
	GameObject* gameobject = new GameObject();
	auto& mesh = gameobject->mesh;
	auto& v = mesh.vertices;
	auto& i = mesh.indices;
	v.resize(24);
	i.resize(36);

	
	float w2 = 0.5f * 2;
	float h2 = 0.5f * 2;
	float d2 = 0.5f * 2;

	// Fill in the front face vertex data.
	v[0] = vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	auto& rotation = gameobject->component.transform.rotation;
	rotation.x() = 0;
	rotation.y() = 0;
	rotation.z() = 0;

	return gameobject;
}


GameObject* AddPyramid()
{
	GameObject* gameobject = new GameObject();
	auto& mesh = gameobject->mesh;
	auto& v = mesh.vertices;
	auto& i = mesh.indices;
	v.resize(5);
	i.resize(18);

	float w = 1.5f;
	float h = 4.0f;

	v[0] = vertex(-w, 0, w, 0, 0, 0);
	v[1] = vertex(w, 0, w, 0, 0, 0);
	v[2] = vertex(w, 0, -w, 0, 0, 0);
	v[3] = vertex(-w, 0, -w, 0, 0, 0);
	v[4] = vertex(0, h, 0, 0, 0, 0);

	i[0] = 0; i[1] = 4; i[2] = 1;
	i[3] = 1; i[4] = 4; i[5] = 2;
	i[6] = 2; i[7] = 4; i[8] = 3;
	i[9] = 3; i[10] = 4; i[11] = 0;
	i[12] = 0; i[13] = 2; i[14] = 3;
	i[15] = 0; i[16] = 1; i[17] = 2;

	auto& rotation = gameobject->component.transform.rotation;
	rotation.x() = 0;
	rotation.y() = 0;
	rotation.z() = 0;

	auto& pos = gameobject->component.transform.position;
	pos.x() = 5;

	return gameobject;
}

HWND ghMainWnd = 0;
HDC hdc = 0;
HDC screenHDC = 0;
int windowHeight = 800;
int windowWidth = 800;
Renderer* device = new Renderer(windowHeight, windowWidth, 0.5f, 1000.0f, 120.0f);

// 初始化Win应用代码
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// 消息循环
int Update();

// 窗口过程，处理窗口接受的消息
LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// WinMain，类似于main的程序入口
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	if (!InitWindowsApp(hInstance, SW_NORMAL))
		return 0;
	return Update();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// 结构体，用来填写窗口属性
	WNDCLASS wc;

	//窗口样式
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 把窗口过程作为回调函数传给窗口
	wc.lpfnWndProc = WndProc;
	// 可以申请额外空间，这里不需要
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	// 当前应用实例的句柄，通过WinMain传入
	wc.hInstance = instanceHandle;
	// 设置图标句柄
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	// 设置鼠标句柄
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	// 设置画刷句柄，决定了背景颜色
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// 指定窗口菜单，暂时没有
	wc.lpszMenuName = 0;
	// 指定窗口类结构体名字，后续引用时直接使用该字符串
	wc.lpszClassName = L"BasicWndClass";

	// 注册到windows系统
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// 创建窗口
	ghMainWnd = CreateWindow(
		// 在上文已经注册的结构体名字
		L"BasicWndClass",
		// 窗体名称
		L"Toy Renderer",
		WS_OVERLAPPED,
		// 左上角在屏幕坐标系的位置
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// 宽高
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// 父窗口句柄
		0,
		// 菜单句柄
		0,
		// 窗口关联的应用程序句柄
		instanceHandle,
		// 指向用户自定义数据的制作，可用作WM_CREATE的lpParam参数
		0);

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// 创建bitmap
	hdc = GetDC(ghMainWnd);
	screenHDC = CreateCompatibleDC(hdc);

	BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER),windowWidth, windowHeight, 1, 32, BI_RGB, windowWidth * windowHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	HBITMAP bitmapHandler = CreateDIBSection(screenHDC, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
	if (bitmapHandler == NULL)
		return false;

	HBITMAP screenObject = (HBITMAP)SelectObject(screenHDC, bitmapHandler);

	// 设置HDC
	device->SetRenderPso(RenderState::OFF, RenderState::ON, RenderState::OFF);
	//device->LoadObject(AddTraingle());
	//device->LoadObject(AddCube());
	//device->LoadObject(AddPyramid());
	GameObject* model = new GameObject();
	device->LoadObject(ObjLoader::LoadObj(std::string("Model//test.obj"), model));
	auto& rotate = model->component.transform.rotation;
	rotate.y() = 180;
	device->SetCamera(.0f, 20.0f, -100.0f);
	device->SetHDC(hdc, screenHDC);


	// 显示窗口
	SetWindowPos(ghMainWnd, NULL, 50, 50, windowWidth, windowHeight, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}


int Update()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//MessageBox(0, L"TEST", L"HELLO", MB_OK);
			device->Update();
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			// 确认是否退出窗体
			if (MessageBox(0, L"Quit？", L"Quite", MB_OKCANCEL) == IDOK)
				DestroyWindow(ghMainWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		DestroyWindow(ghMainWnd);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}