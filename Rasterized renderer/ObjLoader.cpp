#include "ObjLoader.h"
#include <sstream>
#include "Resource.h"
#include <vector>
#include <string>
#include <fstream>
#include "GameObject.h"
#include <windows.h>

GameObject* ObjLoader::LoadObj(std::string file_path, GameObject* object)
{
	Mesh& mesh = object->mesh;
	std::vector<vertex>& vertices = mesh.vertices;
	std::vector<int>& indices = mesh.indices;

	std::fstream fin(file_path, std::ios::in);
	if (!fin.is_open())
	{
		MessageBox(0, L"导入失败！", L"导入失败！", MB_OK);
		return object;
	}
	std::string line;
	std::string space;

	std::vector<vector3> vnbuffer;
	std::vector<vector2> vtbuffer;

	// 首先存入数组中
	while (getline(fin, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 'n')	// 顶点法线
			{
				std::istringstream s(line);
				float x, y, z;
				s >> space >> x >> y >> z;
				vnbuffer.emplace_back(x, y, z);
			}
			else if (line[1] == 't') // 顶点uv
			{
				std::istringstream s(line);
				float x, y;
				s >> space >> x >> y;
				vtbuffer.emplace_back(x, y);
			}
			else // 顶点位置，直接存入到vertices里
			{
				std::istringstream s(line);
				float x, y, z;
				s >> space >> x >> y >> z;
				vertices.emplace_back(x, y, z);
			}
		}
	}

	fin.clear();
	fin.seekg(std::ios::beg);

	// 然后通过f进行索引，一行有三个顶点属性，v的index就是存入indices的数据
	while (getline(fin, line))
	{
		if (line[0] == 'f')
		{
			int n = line.size();
			int j = 2;

			while (j < n)
			{
				// 顶点索引/UV索引/法线索引
				int index[3]{ 0,1,0 };
				for (int i = 0; i < 3; i++)
				{
					while (j < n && line[j] != '/' && line[j] != ' ')
					{
						index[i] = index[i] * 10 + (line[j] - '0');
						j++;
					}

					// 跳过斜线,每组最后跳过空格
					j++;
				}

				// 存储索引，obj索引从1开始
				indices.emplace_back(index[0] - 1);
				// 用顶点索引定位，然后填充顶点属性
				/*vertices[index[0] - 1].uv = { vtbuffer[index[1] - 1].x, vtbuffer[index[1] - 1].y };*/
				vertices[index[0] - 1].uv = { 0, 0 };
				vertices[index[0] - 1].normal = { vnbuffer[index[2] - 1].x, vnbuffer[index[2] - 1].y, vnbuffer[index[2] - 1].z };
			}
		}
	}

	fin.close();
	return object;
}
