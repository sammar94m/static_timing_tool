#ifndef FLIPFLOP_H_
#define FLIPFLOP_H_

#include "enums.h"
#include "Cell.h"

class FlipFlop: public Cell {
	bool visited;
public:
	FlipFlop(cellType type, string name, string logic_string) {
		this->type = type;
		this->name = name;
		this->visited = false;
	}
virtual ~FlipFlop(){};


//	void setSetup(MAXMIN AnlsType, Transitions Tr, int val) {
//		setupdata[2 * AnlsType + Tr] = val;
//
//	}
//
//	int getSetup(MAXMIN AnlsType, Transitions Tr) {
//		return setupdata[2 * AnlsType + Tr];
//
//	}


};










#endif
