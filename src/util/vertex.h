#ifndef _JZ_VERTEX_H
#define _JZ_VERTEX_H

namespace Util
{
	struct vertex
	{
		int8_t x, y, z, w;
		vertex(): x(0), y(0), z(0), w(0) {};
		vertex(int8_t x, int8_t y, int8_t z, int8_t w): x(x), y(y), z(z), w(w) {};
	};	
}


#endif