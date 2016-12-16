#include "mouse.h"

namespace Controls
{
	void mouseMotion(SDL_MouseMotionEvent* ev, glm::vec3* angle) {
		static const float mousespeed = 0.001;

		angle->x -= ev->xrel * mousespeed;
		angle->y -= ev->yrel * mousespeed;

		if(angle->x < -M_PI)
			angle->x += M_PI * 2;
		if(angle->x > M_PI)
			angle->x -= M_PI * 2;
		if(angle->y < -M_PI / 2)
			angle->y = -M_PI / 2;
		if(angle->y > M_PI / 2)
			angle->y = M_PI / 2;

	}
}

