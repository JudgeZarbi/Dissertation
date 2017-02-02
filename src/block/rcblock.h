#ifndef JZ_RCBLOCK_H
#define JZ_RCBLOCK_H
#include "block.h"

namespace Block
{
	struct RightClickBlock : public Block
	{
		RightClickBlock(uint8_t type): Block(type) {};

		virtual bool rightClick() = 0;
	};
}

#endif