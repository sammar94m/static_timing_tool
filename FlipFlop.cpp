/*
 * FlipFlop.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Flipflop.h"

void FlipFlop::setSetup(MAXMIN AnlsType, InOutTr Tr, int val) {
	setup[2 * AnlsType + Tr] = val;
}

int FlipFlop::getSetup(MAXMIN AnlsType, InOutTr Tr) {
	return setup[2 * AnlsType + Tr];

}
void FlipFlop::setHold(MAXMIN AnlsType, InOutTr Tr, int val) {
	setup[2 * AnlsType + Tr] = val;
}

int FlipFlop::getHold(MAXMIN AnlsType, InOutTr Tr) {
	return setup[2 * AnlsType + Tr];

}
clockdat* FlipFlop::getClkdat() {
	for (auto rcvIT = inMap["CLK"]->receivers.begin();
			rcvIT != inMap["CLK"]->receivers.end(); ++rcvIT) {
		if ((rcvIT.operator *())->cell == this) {
			return &(inMap["CLK"]->ClkArtime[(rcvIT.operator *())]);
		}
	}
	return NULL;
}

