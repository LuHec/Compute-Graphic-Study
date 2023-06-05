#pragma once

#include <unordered_map>
#include <Eigen>
#include "Light.h"


class Resource
{
public:
	Resource(unsigned char* res);
	~Resource();


private:
	unsigned char* resource;
};

struct vector2
{
	vector2() = default;
	vector2(float _x, float _y) : x(_x), y(_y) {}
	float x, y;
};

struct vector3
{
	vector3() = default;
	vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	float x, y, z;
};

struct vector4
{
	vector4() = default;
	vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	float x, y, z, w;
};

struct ObjBuffer
{
	Eigen::Matrix4f WorldMatrix = Eigen::Matrix4f::Identity();
	Eigen::Matrix4f InvTransWorldMatrix = Eigen::Matrix4f::Identity();
};

struct Cbuffer
{
	Eigen::Matrix4f ViewMatrix = Eigen::Matrix4f::Identity();
	Eigen::Matrix4f ProjectMatrix = Eigen::Matrix4f::Identity();
	Eigen::Matrix4f ScreenMatrix = Eigen::Matrix4f::Identity();
	Eigen::Vector3f CameraPos = Eigen::Vector3f::Identity();
	Light light;
};

template 
<class BufferType>
class Buffer
{
public:
	Buffer(int width, int height, BufferType color);
	BufferType* operator[](int i);
	~Buffer();

	int width, height;

private:
	BufferType** buffer = nullptr;
};

template <class BufferType>
Buffer<BufferType>::Buffer(int w, int h, BufferType color) : width(w), height(h)
{
	buffer = new BufferType * [width];
	for (int i = 0; i < width; ++i)
	{
		buffer[i] = new BufferType[height];
		for (int j = 0; j < height; ++j)
		{
			buffer[i][j] = color;
		}
	}
}

template<class BufferType>
BufferType* Buffer<BufferType>::operator[](int i)
{
	return buffer[i];
}

template<class BufferType>
Buffer<BufferType>::~Buffer()
{
	for (int i = 0; i < width; ++i)
	{
		delete[] buffer[i];
	}

	delete[] buffer;

	buffer = nullptr;
}


