#include <cassert>

namespace Common
{
	constexpr void ASSERT(const int& expression, const char* message)
	{
		assert(expression && message);
	}
}