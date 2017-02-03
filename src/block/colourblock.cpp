#include "colourblock.h"

namespace Block
{
	ColourBlock cb = ColourBlock();

	bool ColourBlock::rightClick()
	{
		if (type == 5)
		{
			type = 6;
		}
		else if (type == 6)
		{
			type = 7;
		}
		else
		{
			type = 5;
		}

		return true;
	}
}
