/*
 * Receiver.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Sammar
 */

#include "Receiver.h"

void receiver::set_slopeDeg_netDely(string cellName, input_pin pin, delay n_dly ,slope s_deg
			) {
		if(cell && cell->name == cellName && inPin == pin ){
			slopeDeg = s_deg;
			netDely = n_dly;
		}
	}


//	void receiver::print() {
//		cout << "receiver:" << endl;
//		cout << "cell = " << cell->getName() << " ,inPin=" << inPin << " ,slopeDeg="
//				<< slopeDeg << " ,netDely=" << netDely << endl;
//	}
