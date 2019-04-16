#ifndef FLIPFLOP_H_
#define FLIPFLOP_H_

#include "enums.h"
#include "Cell.h"

class FlipFlop: public Cell {
public:
	delay setup[2][2];
	delay hold[2][2];
	FlipFlop(cellType type, string name, string logic_string) {
		this->type = type;
		this->name = name;
		this->visited = false;
	}
	virtual ~FlipFlop()=default;

	void setSetup(MAXMIN AnlsType, Tr Tr, delay val);
	int getSetup(MAXMIN AnlsType, Tr Tr);
	void setHold(MAXMIN AnlsType, Tr Tr, delay val);
	int getHold(MAXMIN AnlsType, Tr Tr);
	clockdat* getClkdat();

};

#endif
