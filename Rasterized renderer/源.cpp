#include <iostream>
#include <Eigen>
#include "Renderer.h"
#include "IShader.h"

GameObject* AddTraingle()
{
	GameObject* gameobject = new GameObject();
	auto& mesh = gameobject->mesh;

	mesh.vertexs.emplace_back(Eigen::Vector3f(0, 1, 0));
	mesh.vertexs.emplace_back(Eigen::Vector3f(-1, 0, 0));
	mesh.vertexs.emplace_back(Eigen::Vector3f(1, 0, 0));

	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(2);

	return gameobject;
}

int main()
{
	Renderer renderer(400, 400, 0.5f, 1000.0f, 60.0f);

	auto obj = AddTraingle();

	renderer.LoadObject(obj);
	renderer.Draw();
}