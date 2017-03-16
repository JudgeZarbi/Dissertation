#ifndef BLOCK_RCBLOCK_H
#define BLOCK_RCBLOCK_H
#include "block.h"

namespace Game
{
	struct RightClickBlock : public virtual Block
	{
		RightClickBlock(uint8_t type): Block(type) {};

		virtual bool rightClick() = 0;
	};
}

#endif