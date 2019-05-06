#pragma once
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
	};
}

