#include "Renderer.h"

void Renderer::Draw()
{
	Update();
	CpuCullAndUpdateObjBuffers();
	DrawRenderItems();
	OutPut();
}

Renderer::Renderer() : width(400), height(400), nearPlane(0.5f), farPlane(1000.0f), aspect(1) 
{
	Init();
}

Renderer::Renderer(int w, int h, float near, float far, float fov) : width(w), height(h), nearPlane(near), farPlane(far), fov(fov), aspect(w/h)
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
	UpdateConstantBuffer();
	std::cout << "view: \n" << globalCbuffer.ViewMatrix << std::endl;
	std::cout << "ProjectMatrix : \n"  << globalCbuffer.ProjectMatrix << std::endl;
	std::cout << "screen: \n" << globalCbuffer.ScreenMatrix << std::endl;
}

void Renderer::UpdateConstantBuffer()
{
	globalCbuffer =
	{
		camera.GetViewMatrix(),
		camera.GetProjMatrix(),
		camera.GetScreenMatrix(),
		camera.transform.position
	};
}

void Renderer::UpdateObjectBuffer(GameObject* gameobject)
{
	ObjBuffer obuffer = {gameobject->component.GetWorldMatrix()};

	gameobject->material.UpdateCBuffer(globalCbuffer);
	gameobject->material.UpdateObjBuffer(obuffer);
}

void Renderer::DrawRenderItems()
{
	for (auto& obj : renderList)
	{
		DrawIndexdInstance(obj->mesh, obj->material);
	}
}

// TODO
// 管线流程开画
void Renderer::DrawIndexdInstance(const Mesh& mesh, const Material& mat)
{
	int indexCount = mesh.indices.size();
	Shader* shader = mat.shader;
	std::vector<VertexOut*> vouts;
	vouts.reserve(mesh.vertices.size());
	std::vector<int> indices_out;
	indices_out.reserve(indexCount);

	auto& vertices = mesh.vertices;
	auto& indices = mesh.indices;

	// 调用shader顶点着色器，然后进行齐次除法、变换到屏幕空间
	for (int i = 0; i < indexCount; i += 3)
	{
		auto v1 = shader->VS(vertices[indices[i]]);
		auto v2 = shader->VS(vertices[indices[i + 1]]);
		auto v3 = shader->VS(vertices[indices[i + 2]]);
		
		if (CvvCull(v1->ScreenPos, camera)) continue;
		if (CvvCull(v2->ScreenPos, camera)) continue;
		if (CvvCull(v3->ScreenPos, camera)) continue;
				
		ProjDivid(v1->ScreenPos);
		Ndc2Screen(v1->ScreenPos, camera.GetScreenMatrix());

		ProjDivid(v2->ScreenPos);
		Ndc2Screen(v2->ScreenPos, camera.GetScreenMatrix());

		ProjDivid(v3->ScreenPos);
		Ndc2Screen(v3->ScreenPos, camera.GetScreenMatrix());

		vouts.emplace_back(v1);
		vouts.emplace_back(v2); 
		vouts.emplace_back(v3);
		indices_out.emplace_back(indices[i]); 
		indices_out.emplace_back(indices[i + 1]);
		indices_out.emplace_back(indices[i + 2]);
	}

	std::cout << "index count: " << indexCount << std::endl;

	// 光栅化
	// TODO
	// 三角形设置
	// 三角形遍历
	// 画
	for (int i = 0; i < indices_out.size(); i += 3)
	{
		std::cout << "indices_out.size() : " << indices_out.size() << std::endl;
		DrawLine
		(
			{ vouts[indices[i]]->ScreenPos.x(), vouts[indices[i]]->ScreenPos.y() },
			{ vouts[indices[i + 1]]->ScreenPos.x(), vouts[indices[i + 1]]->ScreenPos.y() },
			{ vouts[indices[i + 2]]->ScreenPos.x(), vouts[indices[i + 2]]->ScreenPos.y() }
		);

		DrawTriangle(vouts[indices[i]], vouts[indices[i+1]], vouts[indices[i + 2]], shader);
	}
}

void Renderer::OutPut()
{
	std::ofstream fout("result.ppm");

	fout << "P3\n"
		<< width << ' ' << height << "\n255\n";

	// 要注意！！物体的屏幕坐标为(x, y)，但是绘制是从左上角开始，且向右边写入的，所以要求以列为外循环
	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			auto color = (*frame_buffer)[i][j];
			fout << (int)color.x << ' ' << (int)color.y << ' ' << (int)color.z << '\n';
			//std::cout << "Drawing" << std::endl;
		}
	}

	std::cout << "end" << std::endl;

	std::ofstream zout("depth.ppm");

	zout << "P3\n"
		<< width << ' ' << height << "\n255\n";

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			auto z = (*z_buffer)[i][j];
			zout <<  int(z * 255) << ' ' << 0 << ' ' << 0 << '\n';
			//std::cout << "Drawing z : " << z << std::endl;
		}
	}

	std::cout << "end" << std::endl;
}


void Renderer::LoadObject(GameObject* object)
{
	objList.emplace_back(object);
}


void Renderer::SetCamera()
{
	camera = Camera({ {0, 0, -10} }, width, height, nearPlane, farPlane, fov);
}

void Renderer::SetBuffers()
{
	frame_buffer = new Buffer<vector3>(width, height, {0, 0, 0});
	z_buffer = new Buffer<float>(width, height, 1.0f);

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
	float w = v.w();

	std::cout << "cvvcull :" << "x = " << x << " y = " << y << " z = " << z << " w = " << w << std::endl;

	if (x >= -w && x <= w) 
		return true;
	if (y >= -w && y <= w)
		return true;
	if (z >= -w && z <= w)
		return true;
	return false;
}

void Renderer::ProjDivid(Eigen::Vector4f& screenPos)
{
	screenPos.x() /= screenPos.w();
	screenPos.y() /= screenPos.w();
	screenPos.z() /= screenPos.w();
	std::cout << "NDCPos.x = " << screenPos.x() << " NDCPos.y = " << screenPos.y() << " NDCPos.z = " << screenPos.z() << " NDCPos.w = " << screenPos.w() << std::endl;
}

void Renderer::Ndc2Screen(Eigen::Vector4f& screenPos, Eigen::Matrix4f screenMatrix)
{
	// 要把w归位1才能正常引用屏幕空间矩阵
	float w = screenPos.w();
	screenPos.w() = 1;
	std::cout << "NDC screenMatrix:\n" << screenMatrix << std::endl;
	screenPos = screenMatrix * screenPos;
	screenPos.w() = w;
	std::cout << "screenPos.x = " << screenPos.x() << " screenPos.y = " << screenPos.y() << " screenPos.z = " << screenPos.z() << " screenPos.w = " << screenPos.w() << std::endl;
}

bool Renderer::Zwrite_test(int x, int y, float depth)
{
	depth *= -1;
	// depth是负的，要转成正的
	std::cout << "depth : " << depth << std::endl;
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

void Renderer::SetPixel(int x, int y, Color color)
{
	(*frame_buffer)[x][y] = {color.x, color.y, color.z};
}

void Renderer::DrawLine(vector2 v1, vector2 v2, vector2 v3)
{
	MyMath::Bresenham(v1.x + 0.5, v1.y + 0.5, v2.x + 0.5, v2.y + 0.5, frame_buffer);
	//MyMath::Bresenham(v1.x, v1.y, v3.x, v3.y, frame_buffer);
	//MyMath::Bresenham(v2.x, v2.y, v1.x, v1.y, frame_buffer);
	MyMath::Bresenham(v2.x + 0.5, v2.y + 0.5, v3.x + 0.5, v3.y + 0.5, frame_buffer);
	MyMath::Bresenham(v3.x + 0.5, v3.y + 0.5, v1.x + 0.5, v1.y + 0.5, frame_buffer);
	//MyMath::Bresenham(v3.x, v3.y, v2.x, v2.y, frame_buffer);
}

void Renderer::DrawPoint(vector2 v1, vector2 v2, vector2 v3)
{
	std::cout << "v1 = " << (int)v1.x << ',' << v1.y << std::endl;
	std::cout << "v2 = " << (int)v2.x << ',' << v2.y << std::endl;
	std::cout << "v3 = " << (int)v3.x << ',' << v3.y << std::endl;
	(*frame_buffer)[(int)v1.x][(int)v1.y] = { 255, 255, 255 };
	(*frame_buffer)[(int)v2.x][(int)v2.y] = { 255, 255, 255 };
	(*frame_buffer)[(int)v3.x][(int)v3.y] = { 255, 255, 255 };
}

// TODO
// 三角形遍历
// 插值
// ztest
// 写入
void Renderer::DrawTriangle(VertexOut* v1, VertexOut* v2, VertexOut* v3, Shader* shader)
{
	//Eigen::Vector3f p1(v1->ScreenPos.x(), v1->ScreenPos.y(), v1->ScreenPos.z());
	//Eigen::Vector3f p2(v2->ScreenPos.x(), v2->ScreenPos.y(), v2->ScreenPos.z());
	//Eigen::Vector3f p3(v2->ScreenPos.x(), v3->ScreenPos.y(), v3->ScreenPos.z());

	auto p1 = v1->ScreenPos.head<3>();
	auto p2 = v2->ScreenPos.head<3>();
	auto p3 = v3->ScreenPos.head<3>();

	std::vector<Eigen::Vector3f> tri;
	tri.emplace_back(p1);
	tri.emplace_back(p2);
	tri.emplace_back(p3);
	
	// Boundingbox
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
				auto bary = MyMath::barycentric(A, B, C, P);
				VertexOut out_interpolated = *v1 * bary.alpha + *v2 * bary.beta + *v3 * bary.gamma;
				if (Zwrite_test(x, y, out_interpolated.ScreenPos.z()));
				{
					SetPixel(x, y, shader->PS(out_interpolated));
				}
			}
		}
	}

}

// TODO
// 视椎体剔除
void Renderer::CpuCullAndUpdateObjBuffers()
{
	for (auto& o : objList)
	{
		UpdateObjectBuffer(o);
		renderList.emplace_back(o);
	}
}
