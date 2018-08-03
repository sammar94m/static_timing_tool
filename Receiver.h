#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "enums.h"
#include "Cell.h"

class receiver {
public:
	Cell* cell;
	input_pin inPin;
	int slopeDeg;
	int netDely;

	receiver(Cell *cell, input_pin inPin, int slopeDeg = 0, int netDely = 0) :
			cell(cell), inPin(inPin), slopeDeg(slopeDeg), netDely(netDely) {}


};


#endif

