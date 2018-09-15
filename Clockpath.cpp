/*
 * Clockpath.cpp
 *
 *  Created on: 9 Aug 2018
 *      Author: Saeed
 */
#include "enums.h"
#include "Cell.h"
#include "Net.h"
#include "Flipflop.h"
#include "Clockdat.h"
extern queue<Net*> InputClkTable;

void clockPathDelayCalcAux(pin input, clockdat& inclock, Cell* currCell,
		MAXMIN MODE) {
	if (currCell->type == FlIPFlOP || currCell->type == OUTCELL) {
		return;
	}
	for (auto pinIT = currCell->outMap.begin(); pinIT != currCell->outMap.end();
			++pinIT) {
		delay Celldelay[2]; // 0 - fall 1 - rise
		slope drvslope[2]; // 0 - fall 1 - rise
		Net* currNet = pinIT->second;
		currNet->isClk = true;
		output_pin currCelloutPin = pinIT->first;
		load outload = currCell->getCout(currCelloutPin);// calculate load on the output pin
		drvslope[_FALL_] = currCell->getSlope(input, currCelloutPin, MODE, RF,
				inclock.RISE_SLOPE, outload);
		drvslope[_RISE_] = currCell->getSlope(input, currCelloutPin, MODE, FR,
				inclock.FALL_SLOPE, outload);
		Celldelay[_FALL_] = currCell->getDelay(input, currCelloutPin, MODE, RF,
				inclock.RISE_SLOPE, outload);
		Celldelay[_RISE_] = currCell->getDelay(input, currCelloutPin, MODE, RF,
				inclock.RISE_SLOPE, outload);
		for (auto rcvCellIt = currNet->receivers.begin();
				rcvCellIt != currNet->receivers.end(); ++rcvCellIt) {
			input_pin recieverpin = (*(rcvCellIt))->inPin;
			slope Netoutslope[2];
			delay netdelay;
			Netoutslope[_FALL_] = currNet->getRcvSlope(drvslope[_FALL_],
					(*(rcvCellIt))->cell, recieverpin);
			Netoutslope[_RISE_] = currNet->getRcvSlope(drvslope[_RISE_],
					(*(rcvCellIt))->cell, recieverpin);
			netdelay = currNet->getRcvDelay((*(rcvCellIt))->cell, recieverpin); // calculate Net delay
			clockdat nextstageclock(
					inclock.RISE_AR + Celldelay[_FALL_] + inclock.low
							+ Celldelay[_RISE_], Netoutslope[_FALL_],
					Netoutslope[_RISE_], inclock.high + Celldelay[_RISE_],
					inclock.low + Celldelay[_FALL_]);
			currNet->ClkArtime[*(rcvCellIt)] = nextstageclock;
			// Move On
			clockPathDelayCalcAux(recieverpin, nextstageclock,
					(*(rcvCellIt))->cell, MODE);
		}
	}
	return;
}

void clockPathDelayCalc() {
	Cell* curr;
	while (!InputClkTable.empty()) {
		Net* inNetp;
		inNetp = InputDataTable.front();
		InputDataTable.pop();
		for (auto RcvIt = inNetp->receivers.begin();
				RcvIt != inNetp->receivers.end(); ++RcvIt) {
			curr = (*RcvIt)->cell;
			clockPathDelayCalcAux("", ((inputNet*) curr)->clk, curr, MAX);
			clockPathDelayCalcAux("", ((inputNet*) curr)->clk, curr, MIN);
		}
	}
