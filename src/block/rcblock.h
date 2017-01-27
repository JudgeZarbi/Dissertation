#ifndef JZ_RCBLOCK_H
#define JZ_RCBLOCK_H
#include "block.h"

namespace Block
{
	struct RightClickBlock : public Block
	{
		virtual bool rightClick() = 0;
	};
}

#endif