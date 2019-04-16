/*
 * FlipFlop.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Flipflop.h"

void FlipFlop::setSetup(MAXMIN AnlsType, Tr Tr, delay val) {
	setup[AnlsType] [Tr] = val;
}

int FlipFlop::getSetup(MAXMIN AnlsType, Tr Tr) {
	return setup[AnlsType][Tr];

}
void FlipFlop::setHold(MAXMIN AnlsType, Tr Tr, delay val) {
	setup[AnlsType] [Tr] = val;
}

int FlipFlop::getHold(MAXMIN AnlsType, Tr Tr) {
	return setup[AnlsType ][Tr];

}
clockdat* FlipFlop::getClkdat() {
	return NULL;
}

