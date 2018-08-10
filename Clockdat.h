#ifndef CLOCKDAT_H_
#define CLOCKDAT_H_


#include "enums.h"


class clockdat {
	int RISE_AR;
	slope FALL_SLOPE;
	slope RISE_SLOPE;
	int low; //clk param
	int high; //clk param
public:
	clockdat(  int RISE_AR, slope FALL_SLOPE, slope RISE_SLOPE,int low, int high) :
			 FALL_SLOPE(FALL_SLOPE), RISE_AR(RISE_AR), RISE_SLOPE(RISE_SLOPE), low(low), high(high) {
	}
	clockdat();
};




#endif
