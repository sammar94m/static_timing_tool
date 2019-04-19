#ifndef FLIPFLOP_H_
#define FLIPFLOP_H_

#include "enums.h"
#include "Cell.h"

class FlipFlop: public Cell {
public:
	bool endpoint = false;
	FlipFlop(cellType type, string name, CellTemplate* ctemplate) :
			Cell(type, name, ctemplate) {
	}
	virtual ~FlipFlop() = default;
	pin getDatPin();
	pin getClkPin();
	bool isReady();
	void initDataReq();

};

#endif
