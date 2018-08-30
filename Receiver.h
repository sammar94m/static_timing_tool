#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "enums.h"

class Cell;

class receiver {
public:
	Cell* cell;
	input_pin inPin;
	int slopeDeg;
	int netDely;

	receiver(Cell *cell, input_pin inPin, int slopeDeg = 0, int netDely = 0) :
			cell(cell), inPin(inPin), slopeDeg(slopeDeg), netDely(netDely) {
	}

//	void print() {
//		cout << "receiver:" << endl;
//		cout << "cell = " << cell->getName() << " ,inPin=" << inPin << " ,slopeDeg="
//				<< slopeDeg << " ,netDely=" << netDely << endl;
//	}

};

#endif

