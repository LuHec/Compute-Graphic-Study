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
	vouts.reserve(mesh.vertexs.size());
	std::vector<int> indices_out;
	indices_out.reserve(indexCount);

	auto& vertices = mesh.vertexs;
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

	// 光栅化
	// TODO
	// 三角形设置
	// 三角形遍历
	// 画
	for (int i = 0; i < indices_out.size(); i += 3)
	{
		vector2 v1{};
		DrawLine({ vouts[indices[i]]->ScreenPos.x(), vouts[indices[i]]->ScreenPos.y() },
			{ vouts[indices[i + 1]]->ScreenPos.x(), vouts[indices[i + 1]]->ScreenPos.y() },
			{ vouts[indices[i + 2]]->ScreenPos.x(), vouts[indices[i + 2]]->ScreenPos.y() });
	}
}

void Renderer::OutPut()
{
	std::ofstream fout("result.ppm");

	fout << "P3\n"
		<< width << ' ' << height << "\n255\n";

	// 要注意！！物体的屏幕坐标为(x, y)，但是绘制是从左上角开始的，所以要求以列为外循环
	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			auto color = (*frame_buffer)[i][j];
			fout << color.x << ' ' << color.y << ' ' << color.z << '\n';
			//std::cout << "Drawing" << std::endl;
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
	z_buffer = new Buffer<float>(width, height, 1.1f);

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
	if (depth > (*z_buffer)[x][y])
	{
		(*z_buffer)[x][y] = depth;
		return true;
	}

	return false;
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
void Renderer::DrawTriangle(VertexOut* v1, VertexOut* v2, VertexOut* v3)
{

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
