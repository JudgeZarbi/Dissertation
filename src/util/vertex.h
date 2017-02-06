#ifndef UTIL_VERTEX_H
#define UTIL_VERTEX_H

#include <iostream>

namespace Util
{
	struct vertex
	{
		int8_t x, y, z, w;
		vertex(): x(0), y(0), z(0), w(0) {};
		vertex(int8_t x, int8_t y, int8_t z, int8_t w): x(x), y(y), z(z), w(w) {};

		friend std::ostream& operator<<(std::ostream& out, const vertex& rhs)
		{
			out << "[" << std::to_string(rhs.x) << ", " << std::to_string(rhs.y) << ", " << std::to_string(rhs.z) << ", " << std::to_string(rhs.w) << "]" << std::endl;
			return out;
		};

	};	
}


#endif