#include "Zenoy_coefficients.h"

double Zenoy::CoefficientVector::calc_length()
{
	double r = 0;
	for (unsigned char i = 0; i < LENGTH; i++)
	{
		r += (values[i].double_expr * values[i].double_expr);
	}
	r = sqrt(r);
	return r;
}

Zenoy::CoefficientVector& Zenoy::CoefficientVector::normalize()
{
	double length = calc_length();
	for (unsigned char i = 0; i < LENGTH; i++)
	{
		values[i].double_expr /= length;
	}
	return *this;
}

Zenoy::CoefficientVector& Zenoy::CoefficientVector::operator+=(const CoefficientVector& rhs)
{
	for (unsigned char i = 0; i < LENGTH; i++)
	{
		values[i].double_expr += rhs.values[i].double_expr;
	}
	return *this;
}

Zenoy::CoefficientVector& Zenoy::CoefficientVector::operator/=(const double& rhs)
{
	for (unsigned char i = 0; i < LENGTH; i++)
	{
		values[i].double_expr /= rhs;
	}
	return *this;
}

Zenoy::CoefficientVector& Zenoy::CoefficientVector::operator*=(const double& rhs)
{
	for (unsigned char i = 0; i < LENGTH; i++)
	{
		values[i].double_expr *= rhs;
	}
	return *this;
}

bool Zenoy::CoefficientVector::operator==(const CoefficientVector& rhs)
{
	for (unsigned char i = 0; i < LENGTH; i++)
	{
		if (values[i].double_expr != rhs.values[i].double_expr)
		{
			return false;
		}
	}
	return true;
}

bool Zenoy::CoefficientVector::operator!=(const CoefficientVector& rhs)
{
	return (*this == rhs);
}
