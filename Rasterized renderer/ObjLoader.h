#pragma once
#include <iostream>

class GameObject;

namespace ObjLoader
{
	GameObject* LoadObj(std::string file_path, GameObject* object);
}