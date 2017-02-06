#ifndef BLOCK_BLOCK_H
#define BLOCK_BLOCK_H
#include <cstdint>
#include <iostream>

//The class from which all blocks are eventually derived from.
namespace Game
{
	struct Block
	{
		uint8_t type;

		Block(): type(0) {};
		Block(uint8_t type): type(type) {};

		virtual bool rightClick() { return false; };	
	};

}

#endif