#ifndef INPUTNET_H_
#define INPUTNET_H_

#include "enums.h"
#include "Net.h"

class inputNet: public Net {
	string AR_TIME;
	int SL_RISE;
	int SL_FALL;
public:
	inputNet(string name, bool isClk, int low, int high, int SL_RISE,
			int SL_FALL,string AR_TIME) :

			Net(name, INPUT, isClk, low, high), AR_TIME(AR_TIME), SL_RISE(
					SL_RISE), SL_FALL(SL_FALL) {
	}
	;
	//virtual ~inputNet(){};
};





#endif

