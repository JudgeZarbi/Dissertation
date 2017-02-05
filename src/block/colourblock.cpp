#include "colourblock.h"

namespace Block
{
	ColourBlock cb = ColourBlock();

	bool ColourBlock::rightClick()
	{
		std::cout << "Before: " << type << std::endl;
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
		std::cout << "After: " << type << std::endl;
		std::cout << "RC!" << std::endl;

		return true;
	}
}
