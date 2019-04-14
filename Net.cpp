/*
 * Net.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */
#include "Net.h"
#include "Cell.h"

Net::Net(string _name, netType _type, bool _isClk) :
		name(_name), type(_type), isClk(_isClk) {
}

Net::Net(string name, bool isClk) :
		name(name), type(_netType::LOCAL), isClk(isClk) {
}

Net::~Net() {
}

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
	if (cell == NULL) {
		cout << "setting driver failed because null pointer" << endl;
	} else {
		//	cout<<"setting cell "<<cell->name <<" as a driver to Net "<<this->name<<endl;
	}

	driver.first = cell;
	driver.second = pin_t;
}

void Net::add_receiver(Cell* cell, pin pin_t) {
	receivers.push_front(new receiver(cell, pin_t));
}

void Net::CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv, pin rcvpin) {
	if (rcv == NULL) {
		cout << "CALC DREVREQ: error rcv = NULL" << endl;
	} else {
		//TODO: fix for fanout
		delay del = getRcvDelay(rcv, rcvpin);
		int tmp[2];
		for (int j = 0; j < 2; j++) {
			tmp[MAX] = driver.first->PinData[driver.second].tmp_req[MAX][j].val;
			tmp[MIN] = driver.first->PinData[driver.second].tmp_req[MIN][j].val;
			driver.first->PinData[driver.second].tmp_req[MAX][j].val = min(
					tmp[MAX], rcvreq[MAX][j].val - del);
			driver.first->PinData[driver.second].tmp_req[MIN][j].val = max(
					tmp[MIN], rcvreq[MIN][j].val - del);
		}
		driver.first->PinData[driver.second].CalcTmpMarg();
		driver.first->PinData[driver.second].updateWC();
	}

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
Tr state;
MAXMIN M;
bool CompPRcv(receiver* lhs, receiver* rhs) {
	return rhs->cell->PinData[lhs->inPin].GetWCTmpMarg(M)
			< rhs->cell->PinData[lhs->inPin].GetWCTmpMarg(M);
}
list<receiver*>::iterator Net::getCritReciever(MAXMIN MODE) {
	list<receiver*>::iterator j;
	list<receiver*> tmp;
	M = MODE;
	for (auto i : this->receivers) {
		if (i->cell->visittime > resettime) {
			tmp.push_back(i);
		}
	}
	tmp.sort(CompPRcv);
	j = find(this->receivers.begin(), this->receivers.end(), tmp.front());

	return j;
}

void Net::RecordBS(_PATH* pPA,path_vec::iterator PA, list<receiver*>::iterator ref,
		margin refm, PriorityQ<branchslack>& BS, MAXMIN MODE, Tr state) {
	margin refmarg = (*ref)->cell->PinData[(*ref)->inPin].tmp_marg[MODE][state];
	for (auto i = this->receivers.begin(); i != this->receivers.end(); i++) {
		if (ref != i && (*i)->cell->visittime > resettime) {
			margin tmp =
					 (*i)->cell->PinData[(*i)->inPin].tmp_marg[MODE][state] - refmarg;
			if (tmp > 0)
				continue;
			branchslack bs(pPA,PA, tmp, refm, i,state);
			BS.Add(bs);
		}
	}
}
