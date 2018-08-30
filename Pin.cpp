/*
 * Pin.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Sammar
 */
#include "Net.h"
#include "Pin.h"

	void Pin::updateWC(margin MAR, slope SLOP, Transitions Tr /* Fall or Rise */) {
		if (Tr == FALL) {
			WCMarF = WCMarF < MAR ? WCMarF : MAR;
			WCSlopeF = WCSlopeF > SLOP ? WCSlopeF : SLOP;
			validF = true;
		} else {
			WCMarR = WCMarR < MAR ? WCMarR : MAR;
			WCSlopeR = WCSlopeR > SLOP ? WCSlopeR : SLOP;
			validR = true;
		}
	}



