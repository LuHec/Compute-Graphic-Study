#include "Renderer.h"
// 函数部分

void Renderer::Draw()
{
	DrawRenderItems();
	WinOutPut();
	//OutPut();
}

void Renderer::SetCamera(const float x, const float y, const float z)
{
	camera.transform.position.x() = x;
	camera.transform.position.y() = y;
	camera.transform.position.z() = z;
}

void Renderer::SetRenderPso(RenderPSO pso)
{
	renderPso = pso;
}

void Renderer::SetRenderPso(RenderState WireframeState, RenderState TriangleState, RenderState PointState)
{
	renderPso = { WireframeState, TriangleState, PointState };
}

Renderer::Renderer() : width(400), height(400), nearPlane(0.5f), farPlane(1000.0f), aspect(1) 
{
	Init();
}

Renderer::Renderer(int w, int h, float znear, float zfar, float fov) : width(w), height(h), nearPlane(znear), farPlane(zfar), fov(fov), aspect(w/h)
{ 
	Init();
}

Renderer::~Renderer()
{
	delete(frame_buffer);
	delete(z_buffer);
}

void Renderer::Init()
{
	SetCamera();
	SetBuffers();
}

void Renderer::Update()
{
	Clear();

	UpdateConstantBuffer();
	std::cout << "view: \n" << globalCbuffer.ViewMatrix << std::endl;
	std::cout << "ProjectMatrix : \n" << globalCbuffer.ProjectMatrix << std::endl;
	std::cout << "screen: \n" << globalCbuffer.ScreenMatrix << std::endl;
	CpuCullAndUpdateObjBuffers();
	Draw();
}

void Renderer::UpdateConstantBuffer()
{
	camera.UpdateMatrix();
	globalCbuffer.ViewMatrix = camera.GetViewMatrix();
	globalCbuffer.ProjectMatrix = camera.GetProjMatrix();
	globalCbuffer.ScreenMatrix = camera.GetScreenMatrix();
	globalCbuffer.CameraPos = camera.transform.position;
}

void Renderer::UpdateObjectBuffer(GameObject* gameobject)
{
	ObjBuffer obuffer = {gameobject->component.GetWorldMatrix()};

	gameobject->material.UpdateCBuffer(globalCbuffer);
	gameobject->material.UpdateObjBuffer(obuffer);
}

void Renderer::DrawRenderItems()
{
	for (const GameObject* obj : objList)
	{
		DrawInstanceIndexd(obj->mesh, obj->material);
	}
}

// TODO
// 管线流程开画
void Renderer::DrawInstanceIndexd(const Mesh& mesh, const Material& mat)
{
	// 准备顶点和索引
	int indexCount = mesh.indices.size();
	int vecticesCount = mesh.vertices.size();
	Shader* shader = mat.shader;
	std::vector<VertexOut> vouts;
	std::vector<VertexOut> projVouts;
	//std::vector<int> indices_out;

	const std::vector<vertex>& vertices = mesh.vertices;
	const std::vector<int>& indices = mesh.indices;

	for (auto& v : vertices)
	{
		std::cout << "v.worldPos =(" << v.pos << ")" << std::endl;
	}

	// 调用shader顶点着色器,然后变换到ndc
	// 需要存储进行齐次除法变换的和没有进行过的
	// 后续使用没进行齐次除法的进行cvv剔除，通过后再使用进行过齐次除法并变换到屏幕空间的坐标画三角形
	for (const auto& v : vertices)
	{
		auto vs_v = shader->VS(v);
		vouts.emplace_back(vs_v);

		ProjDivid(vs_v.ScreenPos);
		Ndc2Screen(vs_v.ScreenPos, camera.GetScreenMatrix());
		projVouts.emplace_back(vs_v);
	}

	for (auto& v : vouts)
	{
		std::cout << "v.worldPos =(" << v.worldPos.x() << ", " << v.worldPos.y() << ", " << v.worldPos.z() << ")" << std::endl;
		std::cout << "v.ScreenPos =(" << v.ScreenPos.x() << "," << v.ScreenPos.y() << ")" << std::endl;
		std::cout << "------------------ padding ------------------" << std::endl;
	}

	std::cout << "index count: " << indexCount << std::endl;
	std::cout << "indices count: " << indices.size() << std::endl;

	// 光栅化，需要先进行视椎体剔除
	// 
	// Tips: 这里暂时剔除整个面
	int cnt = 0;
	for (int i = 0; i < indices.size(); i += 3)
	{		
		auto& v1 = vouts[indices[i]];
		auto& v2 = vouts[indices[i + 1]];
		auto& v3 = vouts[indices[i + 2]];

		// 此时的ScreenPos还是ProjPos
		if (CvvCull(v1.ScreenPos, camera) || CvvCull(v2.ScreenPos, camera) || CvvCull(v3.ScreenPos, camera)) continue;

		cnt++;
		
		auto& projV1 = projVouts[indices[i]];
		auto& projV2 = projVouts[indices[i + 1]];
		auto& projV3 = projVouts[indices[i + 2]];

		if (RenderState::ON == renderPso.TriangleState)
		{
			DrawTriangle(projV1, projV2, projV3, shader);
		}

		if (RenderState::ON == renderPso.PointState)
		{
			DrawPoint
			(
				{ vouts[indices[i]].ScreenPos.x(), vouts[indices[i]].ScreenPos.y() },
				{ vouts[indices[i + 1]].ScreenPos.x(), vouts[indices[i + 1]].ScreenPos.y() },
				{ vouts[indices[i + 2]].ScreenPos.x(), vouts[indices[i + 2]].ScreenPos.y() }
			);
		}
		
		if (RenderState::ON == renderPso.WireframeState)
		{
			DrawLine
			(
				{ projV1.ScreenPos.x(), projV1.ScreenPos.y() },
				{ projV2.ScreenPos.x(), projV2.ScreenPos.y() },
				{ projV3.ScreenPos.x(), projV3.ScreenPos.y() }
			);
		}
		
	}
	std::cout << "rendered faces : " << cnt << std::endl;
}

void Renderer::OutPut()
{
	std::string cbufferOut = output_base_path + "result.ppm";
	std::string zbufferOut = output_base_path + "depth.ppm";
	std::ofstream fout(cbufferOut);
	std::ofstream zout(zbufferOut);

	fout << "P3\n"
		<< width << ' ' << height << "\n255\n";

	// 要注意！！物体的屏幕坐标为(x, y)，但是绘制是从左上角开始，且向右边写入的，所以要求以列为外循环
	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			auto color = (*frame_buffer)[i][j];
			fout << (int)color.x << ' ' << (int)color.y << ' ' << (int)color.z << '\n';
		}
	}

	std::cout << "end" << std::endl;

	zout << "P3\n"
		<< width << ' ' << height << "\n255\n";

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			int z = (*z_buffer)[i][j] * 255;
			zout <<  z << ' ' << z << ' ' << z << '\n';
		}
	}

	std::cout << "end" << std::endl;
}

void Renderer::WinOutPut()
{
	// 双重缓冲
	BitBlt(hdc, 0, 0, width, height, screenHDC, 0, 0, SRCCOPY);
}

void Renderer::Clear()
{
	BitBlt(screenHDC, 0, 0, width, height, NULL, NULL, NULL, WHITENESS);

	for (int i = 0; i < z_buffer->height; i++)
	{
		for (int j = 0; j < z_buffer->width; j++)
		{
			(*z_buffer)[i][j] = 1;
		}
	}
}


void Renderer::SetHDC(HDC ohdc, HDC oscreenHDC)
{
	hdc = ohdc;
	screenHDC = oscreenHDC;
}

void Renderer::LoadObject(GameObject* object)
{
	objList.emplace_back(object);
}


void Renderer::SetCamera()
{
	camera = Camera({ {0, 0, -10} }, width, height, nearPlane, farPlane, fov);
	camera.SetDirection(Eigen::Vector3f(0, 0, 1));
}

void Renderer::SetBuffers()
{
	frame_buffer = new Buffer<vector3>(width, height, {0, 0, 0});
	z_buffer = new Buffer<float>(width, height, .0f);

	Eigen::Matrix4f ViewMatrix;
	Eigen::Matrix4f ProjectMatrix;
	Eigen::Matrix4f ScreenMatrix;
	Eigen::Vector3f CameraPos;

	ViewMatrix = camera.GetViewMatrix();

	globalCbuffer =
	{
		camera.GetViewMatrix(),
		camera.GetProjMatrix(),
		camera.GetScreenMatrix(),
		camera.transform.position
	};
}

bool Renderer::CvvCull(const Eigen::Vector4f v, Camera camera)
{	
	float x = v.x();
	float y = v.y();
	float z = v.z();
	float w = abs(v.w());

	std::cout << "cvv cull :" << "x = " << x << " y = " << y << " z = " << z << " w = " << w << std::endl;

	if ((x < -w || x > w) || (y < -w || y > w) || (z < -w || z > w))
		return true;
	else
		return false;
}

void Renderer::ProjDivid(Eigen::Vector4f& screenPos)
{
	screenPos.x() /= screenPos.w();
	screenPos.y() /= screenPos.w();
	screenPos.z() /= screenPos.w();
	//std::cout << "NDCPos.x = " << screenPos.x() << " NDCPos.y = " << screenPos.y() << " NDCPos.z = " << screenPos.z() << " NDCPos.w = " << screenPos.w() << std::endl;
}

void Renderer::Ndc2Screen(Eigen::Vector4f& screenPos, const Eigen::Matrix4f& screenMatrix)
{
	// 要把w归位1才能正常引用屏幕空间矩阵
	float w = screenPos.w();
	screenPos.w() = 1;
	//std::cout << "NDC screenMatrix:\n" << screenMatrix << std::endl;
	screenPos = screenMatrix * screenPos;
	screenPos.w() = w;
	//std::cout << "screenPos.x = " << screenPos.x() << " screenPos.y = " << screenPos.y() << " screenPos.z = " << screenPos.z() << " screenPos.w = " << screenPos.w() << std::endl;
}

bool Renderer::Zwrite_test(int x, int y, float depth)
{
	// depth是正常的
	//std::cout << "depth : " << depth << std::endl;
	if (depth < (*z_buffer)[x][y])
	{
		(*z_buffer)[x][y] = depth;
		return true;
	}

	return false;
}

bool Renderer::InsideTriangle(float x, float y, const std::vector<Eigen::Vector3f>& t)
{
	// 与边连线后和三条边的叉积结果正负一致
	//auto a = _v[0]
	Eigen::Vector3f o{ x, y, 0 };

	std::vector<Eigen::Vector3f> edgeso{ o - t[0], o - t[1], o - t[2] };
	std::vector<Eigen::Vector3f> edges{ t[0] - t[1], t[1] - t[2], t[2] - t[0] };

	if (edgeso[0].cross(edges[0]).z() >= 0)
	{
		return edgeso[1].cross(edges[1]).z() >= 0 && edgeso[2].cross(edges[2]).z() >= 0;
	}
	else
	{
		return edgeso[1].cross(edges[1]).z() < 0 && edgeso[2].cross(edges[2]).z() < 0;
	}
}

void Renderer::DrawPixel(int x, int y, const Color& color)
{
	//(*frame_buffer)[x][y] = {color.x, color.y, color.z};
	SetPixel(screenHDC, x, y, RGB(color.x, color.y, color.z));
}

void Renderer::DrawLine(const vector2& v1, const vector2& v2, const vector2& v3)
{
	MyMath::Bresenham(v1.x + 0.5, v1.y + 0.5, v2.x + 0.5, v2.y + 0.5, frame_buffer);
	//MyMath::Bresenham(v1.x, v1.y, v3.x, v3.y, frame_buffer);
	//MyMath::Bresenham(v2.x, v2.y, v1.x, v1.y, frame_buffer);
	MyMath::Bresenham(v2.x + 0.5, v2.y + 0.5, v3.x + 0.5, v3.y + 0.5, frame_buffer);
	MyMath::Bresenham(v3.x + 0.5, v3.y + 0.5, v1.x + 0.5, v1.y + 0.5, frame_buffer);
	//MyMath::Bresenham(v3.x, v3.y, v2.x, v2.y, frame_buffer);
}

void Renderer::DrawPoint(const vector2& v)
{
	(*frame_buffer)[(int)v.x][(int)v.y] = { 255, 255, 255 };
}

void Renderer::DrawPoint(const vector2& v1, const vector2& v2, const vector2& v3)
{
	std::cout << "v1 = （" << (int)v1.x << ',' << v1.y << ")" << std::endl;
	std::cout << "v2 = （" << (int)v2.x << ',' << v2.y << ")" << std::endl;
	std::cout << "v3 = （" << (int)v3.x << ',' << v3.y << ")" << std::endl;

	std::cout << "---------- padding ----------" << std::endl;

	DrawPixel((int)v1.x, (int)v1.y, { 255, 255, 255 });
	DrawPixel((int)v2.x, (int)v2.y, { 255, 255, 255 });
	DrawPixel((int)v3.x, (int)v3.y, { 255, 255, 255 });
}

// TODO
// 三角形遍历
// 插值
// z test
// 写入
void Renderer::DrawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3, Shader* shader)
{
	Eigen::Vector3f p1 = v1.ScreenPos.head<3>();
	Eigen::Vector3f p2 = v2.ScreenPos.head<3>();
	Eigen::Vector3f p3 = v3.ScreenPos.head<3>();

	std::vector<Eigen::Vector3f> tri;
	tri.emplace_back(p1);
	tri.emplace_back(p2);
	tri.emplace_back(p3);
	
	// Bounding box
	float t = std::ceil(fmax(fmax(p1.y(), p2.y()), p3.y()));
	float b = std::floor(fmin(fmin(p1.y(), p2.y()), p3.y()));
	float l = std::floor(fmin(fmin(p1.x(), p2.x()), p3.x()));
	float r = std::ceil(fmax(fmax(p1.x(), p2.x()), p3.x()));

	for (float x = l; x <= r; ++x)
	{
		for (float y = b; y <= t; ++y)
		{
			if (InsideTriangle(x, y, tri))
			{
				vector2 A{ p1.x(), p1.y() };
				vector2 B{ p2.x(), p2.y() };
				vector2 C{ p3.x(), p3.y() };
				vector2 P{ x, y };

				// 重心插值
				auto bary = MyMath::barycentric(A, B, C, P);
				VertexOut out_interpolated = v1 * bary.alpha + v2 * bary.beta + v3 * bary.gamma;

				// 深度测试通过写入像素
				if (Zwrite_test(x, y, out_interpolated.ScreenPos.z()));
				{
					DrawPixel(x, y, shader->PS(out_interpolated));
				}
			}
		}
	}

}

// TODO
// 视椎体剔除
void Renderer::CpuCullAndUpdateObjBuffers()
{
	for (GameObject* o : objList)
	{
		UpdateObjectBuffer(o);
	}
}



const std::string Renderer::output_base_path = "output/";