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
virtual ~FlipFlop(){};


	void setSetup(MAXMIN AnlsType, Transitions Tr, int val) {
	setup[2 * AnlsType + Tr] = val;
	}

	int getSetup(MAXMIN AnlsType, Transitions Tr) {
		return setup[2 * AnlsType + Tr];

	}
	void setHold(MAXMIN AnlsType, Transitions Tr, int val) {
	setup[2 * AnlsType + Tr] = val;
	}

	int getHold(MAXMIN AnlsType, Transitions Tr) {
		return setup[2 * AnlsType + Tr];

	}
	int getClkAR(){
		for(auto rcvIT=inMap["CLK"]->receivers.begin();rcvIT<inMap["CLK"]->receivers.end();++rcvIT){
			if((rcvIT.operator *())->cell==this){
				return inMap["CLK"]->ClkArtime[(rcvIT.operator *())].RISE_AR;
			}
		}
		return -1;
	}


};










#endif
