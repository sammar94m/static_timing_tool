#ifndef FLIPFLOP_H_
#define FLIPFLOP_H_

#include "enums.h"
#include "Cell.h"

class FlipFlop: public Cell {
public:
	bool visited;
	delay setup[4];
	delay hold[4];

	FlipFlop(cellType type, string name, string logic_string) {
		this->type = type;
		this->name = name;
		this->visited = false;
	}
	virtual ~FlipFlop() {
	}
	;

	void setSetup(MAXMIN AnlsType, InOutTr Tr, int val);

	int getSetup(MAXMIN AnlsType, InOutTr Tr);
	void setHold(MAXMIN AnlsType, InOutTr Tr, int val);

	int getHold(MAXMIN AnlsType, InOutTr Tr);
	clockdat* getClkdat();

};

#endif
