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
		MessageBox(0, L"����ʧ�ܣ�", L"����ʧ�ܣ�", MB_OK);
		return object;
	}
	std::string line;
	std::string space;

	std::vector<vector3> vnbuffer;
	std::vector<vector2> vtbuffer;

	// ���ȴ���������
	while (getline(fin, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 'n')	// ���㷨��
			{
				std::istringstream s(line);
				float x, y, z;
				s >> space >> x >> y >> z;
				vnbuffer.emplace_back(x, y, z);
			}
			else if (line[1] == 't') // ����uv
			{
				std::istringstream s(line);
				float x, y;
				s >> space >> x >> y;
				vtbuffer.emplace_back(x, y);
			}
			else // ����λ�ã�ֱ�Ӵ��뵽vertices��
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

	// Ȼ��ͨ��f����������һ���������������ԣ�v��index���Ǵ���indices������
	while (getline(fin, line))
	{
		if (line[0] == 'f')
		{
			int n = line.size();
			int j = 2;

			while (j < n)
			{
				// ��������/UV����/��������
				int index[3]{ 0,1,0 };
				for (int i = 0; i < 3; i++)
				{
					while (j < n && line[j] != '/' && line[j] != ' ')
					{
						index[i] = index[i] * 10 + (line[j] - '0');
						j++;
					}

					// ����б��,ÿ����������ո�
					j++;
				}

				// �洢������obj������1��ʼ
				indices.emplace_back(index[0] - 1);
				// �ö���������λ��Ȼ����䶥������
				/*vertices[index[0] - 1].uv = { vtbuffer[index[1] - 1].x, vtbuffer[index[1] - 1].y };*/
				vertices[index[0] - 1].uv = { 0, 0 };
				vertices[index[0] - 1].normal = { vnbuffer[index[2] - 1].x, vnbuffer[index[2] - 1].y, vnbuffer[index[2] - 1].z };
			}
		}
	}

	fin.close();
	return object;
}
