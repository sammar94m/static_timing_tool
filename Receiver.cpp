/*
 * Receiver.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Sammar
 */

#include "Receiver.h"
#include "Cell.h"

void receiver::set_slopeDeg_netDely(string cellName, input_pin pin, delay n_dly,
		slope s_deg) {
	if (cell && cell->name == cellName && inPin == pin) {
		slopeDeg = s_deg;
		netDely = n_dly;
	}
}
