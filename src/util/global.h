#ifndef UTIL_GLOBAL_H
#define UTIL_GLOBAL_H

// A file to store values that are needed in various places.

namespace Game
{
	const int blocktypes = 9;
	const int CHUNKS_RANGE = 10;
	const int CHUNKS_X = 2*CHUNKS_RANGE + 1;
	const int CHUNKS_Y = 1;
	const int CHUNKS_Z = 2*CHUNKS_RANGE + 1;
	const int FRAME_RANGE = 5;
	//temp
	const int DIFF = 2 * CHUNKS_RANGE;
	const int MAX_TASKS = (CHUNKS_RANGE + 1) * (CHUNKS_RANGE + 10);
}

#endif