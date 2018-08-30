#ifndef OUTPUTNET_H_
#define OUTPUTNET_H_

#include "enums.h"

#include "Net.h"

class outputNet: public Net {
	string REQ_TIME;
	load outload;
public:
	outputNet(string name, bool isClk, int low, int high, string REQ_TIME,
			load outload) :
			Net(name, OUTPUT, isClk, low, high), REQ_TIME(REQ_TIME), outload(
					outload) {
	};

	//virtual ~outputNet(){};
};









#endif
