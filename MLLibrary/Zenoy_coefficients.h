#pragma once
#include "zmath.h"
namespace Zenoy
{
	union DNACoefficient
	{
		double double_expr;
		int64_t numeric_expr;
	};
	struct CoefficientVector
	{
		static const unsigned char
			SCORE = 0,
			BULKINESS = 1,
			BURN = 2,
			AGGREGATE_HEIGHT = 3,
			HOLES = 4,
			//Length of the array. This should be the highest number + 1
			LENGTH = 5;
		DNACoefficient values[LENGTH];
		double calc_length();
		CoefficientVector& normalize();
		CoefficientVector& operator+=(const CoefficientVector& rhs);
		CoefficientVector& operator /=(const double& rhs);
		CoefficientVector& operator *=(const double& rhs);
		bool operator==(const CoefficientVector& rhs);
		bool operator !=(const CoefficientVector& rhs);
	};
}