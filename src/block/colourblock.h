#ifndef JZ_COLOURBLOCK_H
#define JZ_COLOURBLOCK_H
#include "rcblock.h"
#include <iostream>

namespace Block
{
	struct ColourBlock : public RightClickBlock
	{
		ColourBlock(): RightClickBlock(5) {};

		bool rightClick();
	};
}

#endif