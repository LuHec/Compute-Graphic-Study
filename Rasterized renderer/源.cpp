#include <iostream>
//#include <windows.h>
#include <Eigen>
#include "Renderer.h"
#include "IShader.h"

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
	rotation.y() = 45;
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
	rotation.y() = 45;
	rotation.z() = 15;

	auto& pos = gameobject->component.transform.position;
	pos.x() = 5;

	return gameobject;
}

int main()
{
	// 左手系，y轴向上，[-1, 1]，深度为负值
	Renderer renderer(800, 800, 0.5f, 1000.0f, 120.0f);
	renderer.SetRenderPso(RenderState::ON, RenderState::ON, RenderState::OFF);

	renderer.LoadObject(AddTraingle());
	renderer.LoadObject(AddCube());
	renderer.LoadObject(AddPyramid());
	renderer.SetCamera(.0f, 0.0f, -8.0f);
	renderer.Draw();
}

// TODO
// 修复深度,ndc[-1, 1]，输出[0, 1]
// 需要修改CULL部分,fov出现问题
