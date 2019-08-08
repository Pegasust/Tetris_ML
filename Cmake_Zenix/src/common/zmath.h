#pragma once
#include <math.h>


namespace Common
{
namespace ZMath
{
	int round_nearest(double x);
	int round_nearest(float f);

	inline double get_magnitude(double const & x, double const & y)
	{
		return sqrt(x * x + y * y);
	}
	template<typename T>
	struct vector2
	{
		T x, y;
		vector2 operator+(const vector2& rhs)
		{
			vector2 re;
			re.x = this->x + rhs.x;
			re.y = this->y + rhs.y;
			return re;
		}
		vector2 operator-(const vector2& rhs)
		{
			vector2 re;
			re.x = this->x - rhs.x;
			re.y = this->y - rhs.y;
			return re;
		}
		void operator+=(const vector2& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
		}
		void operator-=(const vector2& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
		}
		vector2 operator+()
		{
			return { this->x, this->y };
		}
		vector2 operator-()
		{
			return { -this->x, -this->y };
		}
		vector2 operator*(const int& i)
		{
			return { this->x * i, this->y * i };
		}
		vector2 operator*(const double& d)
		{
			return { this->x * d, this->y * d };
		}
		double magnitude() const
		{
			double d_x = (double)x;
			double d_y = (double)y;
			return get_magnitude(d_x, d_y);
		}
	};
}
}