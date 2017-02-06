#ifndef BLOCK_RCBLOCK_H
#define BLOCK_RCBLOCK_H
#include "block.h"

namespace Game
{
	struct RightClickBlock : public Block
	{
		RightClickBlock(uint8_t type): Block(type) {};

		virtual bool rightClick() = 0;
	};
}

#endif