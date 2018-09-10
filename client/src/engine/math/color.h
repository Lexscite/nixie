#ifndef NIXIE_COLOR_H_
#define NIXIE_COLOR_H_

#pragma once

#include "math.h"


namespace Nixie
{
	class Color
	{
	public:
		Color();
		Color(float value);
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);
		Color(const Vector3<float>& v);
		Color(const Vector3<float>& v, float a);

		inline operator float*() const { return new float[4]{ r, g, b, a }; };

	private:
		float ClampValue(float value);

	public:
		float r, g, b, a;
	};
}

#endif