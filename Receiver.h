#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "enums.h"
#include "cellfwd.h"

class receiver {
public:
	Cell* cell;
	input_pin inPin;
	slope slopeDeg;
	delay netDely;

	receiver(Cell *cell, input_pin inPin, int slopeDeg = 0, int netDely = 0) :
			cell(cell), inPin(inPin), slopeDeg(slopeDeg), netDely(netDely) {
	};

	void set_slopeDeg_netDely(string cellName, input_pin pin, delay n_dly,
			slope s_deg);


};

#endif

