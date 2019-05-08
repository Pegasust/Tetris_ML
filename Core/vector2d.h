#pragma once
#include <math.h>
namespace TMath
{
	//struct vector2d
	//{
	//	double x, y;
	//	vector2d operator+(const vector2d& rhs)
	//	{
	//		vector2d re;
	//		re.x = this->x + rhs.x;
	//		re.y = this->y + rhs.y;
	//		return re;
	//	}
	//};
	//struct vector2f
	//{
	//	float x, y;
	//	vector2f operator+(const vector2f& rhs)
	//	{
	//		vector2f re;
	//		re.x = this->x + rhs.x;
	//		re.y = this->y + rhs.y;
	//		return re;
	//	}
	//};
	double get_magnitude(double& x, double& y)
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

