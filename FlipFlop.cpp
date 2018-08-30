/*
 * FlipFlop.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Flipflop.h"

void FlipFlop::setSetup(MAXMIN AnlsType, Transitions Tr, int val) {
	setup[2 * AnlsType + Tr] = val;
}

int FlipFlop::getSetup(MAXMIN AnlsType, Transitions Tr) {
	return setup[2 * AnlsType + Tr];

}
void FlipFlop::setHold(MAXMIN AnlsType, Transitions Tr, int val) {
	setup[2 * AnlsType + Tr] = val;
}

int FlipFlop::getHold(MAXMIN AnlsType, Transitions Tr) {
	return setup[2 * AnlsType + Tr];

}
int FlipFlop::getClkAR() {
	for (auto rcvIT = inMap["CLK"]->receivers.begin();
			rcvIT != inMap["CLK"]->receivers.end(); ++rcvIT) {
		if ((rcvIT.operator *())->cell == this) {
			return inMap["CLK"]->ClkArtime[(rcvIT.operator *())].RISE_AR;
		}
	}
	return -1;
}


