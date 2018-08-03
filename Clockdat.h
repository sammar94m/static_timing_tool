#ifndef CLOCKDAT_H_
#define CLOCKDAT_H_


#include "enums.h"


class clockdat {
	int FALL_AR;
	slope FALL_SLOPE;
	int RISE_AR;
	slope RISE_SLOPE;
public:
	clockdat(int FALL_AR, slope FALL_SLOPE, int RISE_AR, slope RISE_SLOPE) :
			FALL_AR(FALL_AR), FALL_SLOPE(FALL_SLOPE), RISE_AR(RISE_AR), RISE_SLOPE(
					RISE_SLOPE) {
	}
	clockdat();
};




#endif
