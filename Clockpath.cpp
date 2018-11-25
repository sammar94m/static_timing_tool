/*
 * Clockpath.cpp
 *
 *  Created on: 9 Aug 2018
 *      Author: Saeed
 */

#include "Clockpath.h"
void calcClkNetDelay(Net* currNet, receiver* rcvrp, slope drvslope[],
		input_pin recieverpin, clockdat& nextstageclock, clockdat& inclock,
		delay Celldelay[], bool inverse) {
	slope Netoutslope[2];
	delay netdelay;
	Netoutslope[_FALL_] = currNet->getRcvSlope(drvslope[_FALL_],
			((rcvrp))->cell, recieverpin);
	Netoutslope[_RISE_] = currNet->getRcvSlope(drvslope[_RISE_],
			((rcvrp))->cell, recieverpin); //TODO: FIX 0 slope
	netdelay = currNet->getRcvDelay((rcvrp)->cell, recieverpin); // calculate Net delay
	if (inverse) {
		clockdat tmpclock(
				inclock.RISE_AR + inclock.high + Celldelay[_RISE_] + netdelay,
				Netoutslope[_FALL_], Netoutslope[_RISE_],
				inclock.high + Celldelay[_RISE_] - Celldelay[_FALL_],
				inclock.low + Celldelay[_FALL_] - Celldelay[_RISE_]);
		nextstageclock = tmpclock;
	} else {
		clockdat tmpclock(
				inclock.RISE_AR + Celldelay[_RISE_] + netdelay,
				Netoutslope[_FALL_], Netoutslope[_RISE_],
				inclock.low - Celldelay[_RISE_] + Celldelay[_FALL_],
				inclock.high - Celldelay[_FALL_] + Celldelay[_RISE_]);
		nextstageclock = tmpclock;
	}

}
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
		load outload = currCell->getCout(currCelloutPin); // calculate load on the output pin
		drvslope[_FALL_] = currCell->getSlope(input, currCelloutPin, MODE, RF,
				inclock.RISE_SLOPE, outload);
		drvslope[_RISE_] = currCell->getSlope(input, currCelloutPin, MODE, FR,
				inclock.FALL_SLOPE, outload);
		Celldelay[_FALL_] = currCell->getDelay(input, currCelloutPin, MODE, RF,
				inclock.RISE_SLOPE, outload);
		Celldelay[_RISE_] = currCell->getDelay(input, currCelloutPin, MODE, FR,
				inclock.RISE_SLOPE, outload);
		for (auto rcvCellIt = currNet->receivers.begin();
				rcvCellIt != currNet->receivers.end(); ++rcvCellIt) {
			clockdat nextstageclock;
			calcClkNetDelay(currNet, *rcvCellIt, drvslope,
					(*(rcvCellIt))->inPin, nextstageclock, inclock, Celldelay,true);
			input_pin recieverpin = (*(rcvCellIt))->inPin;
			currNet->ClkArtime[*(rcvCellIt)] = nextstageclock;
			// Move On
			clockPathDelayCalcAux(recieverpin, nextstageclock,
					(*(rcvCellIt))->cell, MODE);
		}
	}
	return;
}

void clockPathDelayCalc() {
	while (!InputClkTable.empty()) {
		Net* inNetp;
		inNetp = InputClkTable.front();
		InputClkTable.pop();
		for (auto RcvIt = inNetp->receivers.begin();
				RcvIt != inNetp->receivers.end(); ++RcvIt) {
			clockdat nextstageclock;
			clockdat inclock = ((inputNet*) inNetp)->clk;
			slope drvslope[2];
			drvslope[_FALL_] = ((inputNet*) inNetp)->clk.FALL_SLOPE;
			drvslope[_RISE_] = ((inputNet*) inNetp)->clk.RISE_SLOPE;
			delay delaytmp[2] = { 1 };
			calcClkNetDelay(inNetp, *RcvIt, drvslope, (*(RcvIt))->inPin,
					nextstageclock, inclock, delaytmp, false);
			inNetp->ClkArtime[*RcvIt] = nextstageclock;
			clockPathDelayCalcAux((*RcvIt)->inPin, inNetp->ClkArtime[*RcvIt],
					(*RcvIt)->cell, MAX);
			clockPathDelayCalcAux((*RcvIt)->inPin, inNetp->ClkArtime[*RcvIt],
					(*RcvIt)->cell, MIN);
		}
	}
}

