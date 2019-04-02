#ifndef OUTPUTNET_H_
#define OUTPUTNET_H_

#include "enums.h"

#include "Net.h"

class outputNet: public Net {
	string REQ_TIME;
	load outload;
public:
	outputNet(string name, bool isClk, string REQ_TIME, load outload) :
			Net(name, OUTPUT, isClk), REQ_TIME(REQ_TIME), outload(outload) {
	}
	;

	//virtual ~outputNet(){};
	void calcRcvData(receiver* pRcv, const PinDat& Data);
};

#endif
