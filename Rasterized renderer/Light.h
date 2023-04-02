#pragma once

#include "Transform.h"

class Light
{
public:
	static enum class LightType {Direction = 0, Point,};

	Light(LightType type = LightType::Direction, Transform trans = Transform());
	~Light() = default;

private:
	LightType m_type;
	Transform transform;
};
