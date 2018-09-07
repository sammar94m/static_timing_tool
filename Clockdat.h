#ifndef CLOCKDAT_H_
#define CLOCKDAT_H_

#include "enums.h"

class clockdat {
public:
	int RISE_AR;
	slope FALL_SLOPE;
	slope RISE_SLOPE;
	int low; //clk param
	int high; //clk param

	clockdat(){
		 RISE_AR =0;
		 FALL_SLOPE=0;
		 RISE_SLOPE=0;
		 low=0; //clk param
		 high=0; //clk param
	}

	clockdat(int RISE_AR, slope FALL_SLOPE, slope RISE_SLOPE, int low, int high) :
			RISE_AR(RISE_AR % (high + low)), FALL_SLOPE(FALL_SLOPE), RISE_SLOPE(
					RISE_SLOPE), low(low), high(high) {
	}
};

#endif
