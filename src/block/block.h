#ifndef _JZ_BASEBLOCK_H
#define _JZ_BASEBLOCK_H
#include <cstdint>

//The class from which all blocks are eventually derived from.
namespace Block
{
	struct Block
	{
		uint8_t type;

		Block(uint8_t type): type(type) {};

//		virtual bool rightClick();	
	};

}

#endif