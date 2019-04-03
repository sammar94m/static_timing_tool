/*
 * Net.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */
#include "Net.h"
#include "Cell.h"

delay Net::getRcvDelay(Cell* rcv, pin rcvpin) {
	for (auto RcvIT = this->receivers.begin(); RcvIT != this->receivers.end();
			RcvIT++) {
		if ((*RcvIT)->cell == rcv && (*RcvIT)->inPin == rcvpin) {
			return (*RcvIT)->netDely;
		}
	}
	return -1;
}
slope Net::getRcvSlope(slope drvSlope, Cell* rcv, pin rcvpin) {
	for (auto RcvIT = this->receivers.begin(); RcvIT != this->receivers.end();
			RcvIT++) {
		if ((*RcvIT)->cell == rcv && (*RcvIT)->inPin == rcvpin) {
			return sqrt(
					(*RcvIT)->slopeDeg * (*RcvIT)->slopeDeg
							+ drvSlope * drvSlope);
		}
	}
	return -1;
}
void Net::set_driver(Cell* cell, pin pin_t) {
	driver = pair<Cell*, pin>(cell, pin_t);
}

void Net::add_receiver(Cell* cell, pin pin_t) {
	receivers.push_front(new receiver(cell, pin_t));
}

void Net::CalcDrvReq() {
}

void Net::calcRcvData(receiver* pRcv, const PinDat& Data, pin inPin) {
	Cell* pCell = pRcv->cell;
	delay netDelay = getRcvDelay(pCell, inPin);
	PinDat RcvDat = Data;
	for (const auto i : { MAX, MIN }) {
		for (const auto j : { FALL, RISE }) {
			RcvDat.tmp_slope[i][j] = getRcvSlope(RcvDat.tmp_slope[i][j], pCell,
					inPin);
			RcvDat.tmp_vld[i][j].val += netDelay;
		}
	}
	RcvDat.updateWC(); //TODO: WRITE WC
	pCell->PinData[inPin] = RcvDat;
	pCell->ready_inputs++;

}
PinDat Net::getDrvData() {
	return this->driver.first->PinData[this->driver.second];
}
