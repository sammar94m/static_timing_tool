/*
 * Cell.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Cell.h"
#include "Net.h"

Cell::Cell() {
	Template = NULL;
	type = UNKNOWN;
	outNet = NULL;
	visittime = time(NULL);
	ready_inputs = 0;
	visited = false;
}
Cell::Cell(cellType _type, string _name, CellTemplate* _Template) :
		type(_type), name(_name), Template(_Template), outNet(NULL), visittime(
				time(NULL)), ready_inputs(0), visited(false) {
}
Cell::~Cell() {
	delete Template;
}
int Cell::getnumofinputs() {
	return inMap.size();
}
bool Cell::isReady() {
	return ready_inputs == getnumofinputs();
}

load Cell::getCin(pin input) {
	return Template->temp_pinLoadMap[input];
}

load Cell::getCout(output_pin out) {
	load Cout = 0;
	if (this->pinLoadMap.find(out) != this->pinLoadMap.end()) { // result is cached
		return this->pinLoadMap[out];
	} else {
		Net* rcvr = this->outNet;
		for (auto RcvIT = rcvr->receivers.begin();
				RcvIT != rcvr->receivers.end(); ++RcvIT) {
			input_pin A = (*(RcvIT))->inPin;
			Cell* tmp = (*(RcvIT))->cell; // TODO:add output pin load
			Cout += tmp->getCin(A);
		}
		Cout += (this->Template->temp_pinLoadMap)[out];
		return Cout;
	}

}

string Cell::getName() {
	return name;
}
delay Cell::getDelay(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
		slope inslope, load outload) {
	//TODO: check if its possible transition
	delay res = 1 + (AnlsType == MAX); //Template->getDelay(in, out, AnlsType, Tr, inslope, outload);
	DelCacheKey key(std::pair(std::pair(AnlsType, Tr), std::pair(in, out)));
	DelCache[key] = res;
	return res;
}
slope Cell::getSlope(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
		slope inslope, load outload) {
//	return Template->getSlope(in, out, AnlsType, Tr, inslope, outload);
	return 1;
}
bool Cell::PossiblTr(input_pin in, output_pin out, InOutTr Tr) {
//	int Poss = this->Template->delayTable[make_pair(in, out)].GetTableVal(MAX,
//			Tr, 0, 0);
	return ((Tr == RR) || (Tr == FF)) ? false : true;
}
InOutTr GetInOut(Tr in, Tr out) {
	if (in == RISE) {
		if (out == RISE) {
			return RR;
		} else {
			return RF;
		}
	} else {
		if (out == RISE) {
			return FR;
		} else {
			return FF;
		}
	}
}
void Cell::CalcOutputData() {
	PinDat& Dat = PinData[outPin];
	slope tmp_slp;
	delay tmp_del;
	/*
	 * find the max and min delay/slope from all the outputs to inputs along with
	 */
	for (auto& pair : inMap) {
		auto in_Pin = pair.first;
		for (const auto i : { MIN, MAX }) {
			for (const auto j : { FALL, RISE }) { //output transition
				for (const auto q : { FALL, RISE }) { // input transition
					InOutTr ioTr = GetInOut(q, j);
					if (!PossiblTr(in_Pin, outPin, ioTr)) {
						continue;
					}
					tmp_slp = getSlope(in_Pin, outPin, i, ioTr,
							PinData[in_Pin].tmp_slope[i][q], getCout(outPin));
					tmp_del = getDelay(in_Pin, outPin, i, ioTr, tmp_slp,
							getCout(outPin));
					tmp_del += PinData[in_Pin].tmp_vld[i][q].val;
					std::cout << name << ": Pin: " << in_Pin << ": tmp del="
							<< tmp_del << " mode,TR=" << i << " " << q << j
							<< endl;
					if (i == MAX) {
						if (tmp_del > Dat.tmp_vld[i][j].val) {
							Dat.tmp_vld[i][j].val = tmp_del;
							Dat.tmp_TR[i][j] = q;
						}
						Dat.tmp_slope[i][j] = max(tmp_slp, Dat.tmp_slope[i][j]);
					} else {
						if (tmp_del < Dat.tmp_vld[i][j].val) {
							Dat.tmp_vld[i][j].val = tmp_del;
							Dat.tmp_TR[i][j] = q;
						}
						Dat.tmp_slope[i][j] = min(tmp_slp, Dat.tmp_slope[i][j]);
					}
				}
			}
		}
	}
	Dat.updateWC();
	ready_inputs = 0;
	visited = true;
}
void Cell::CalcInputReq() {
	const PinDat& Dat = PinData[outPin];
	for (auto& pair : PinData) {
		auto& in_Pin = pair.first;
		auto& inDat = pair.second;
		if (in_Pin == outPin)
			continue;
		CalReq(in_Pin, outPin, inDat.tmp_req,  inDat.tmp_TR,Dat.tmp_req);
		inDat.CalcTmpMarg();
		inDat.updateWC();
	}
}
void Cell::CalReq(pin in, pin out, required (&inreq)[2][2],
		Tr (&inTr)[2][2],const required (&outreq)[2][2] ) {
	//FALL = 0 Rise = 1
	DelCacheKey key;
	delay tmp[2][4];/*Mode/TR*/
	//fetch delay
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			key = DelCacheKey(std::make_pair((MAXMIN) i, (InOutTr) j),
					std::make_pair(in, out));
			tmp[i][j] = DelCache[key]; //TODO: fix missing tr
		}
	}
	int tmp_req[2][2] = { INT_MIN, INT_MIN, INT_MAX, INT_MAX };
	for (const auto j : { FALL, RISE }) { //output transition
		for (const auto q : { FALL, RISE }) { // input transition
			InOutTr ioTr = GetInOut(q, j);
			if (!PossiblTr(in, outPin, ioTr)) {
				continue;
			}
			delay maxd = outreq[MAX][q].val - tmp[MAX][ioTr];
			delay mind = outreq[MIN][q].val - tmp[MIN][ioTr];
			if (maxd < tmp_req[MAX][j]) {
				tmp_req[MAX][j] = maxd;
				inTr[MAX][j] = q;
			}
			if (mind > tmp_req[MAX][j]) {
				tmp_req[MIN][j] = mind;
				inTr[MIN][j] = q;
			}
		}
	}

	for (const auto i : { MIN, MAX }) {
		for (const auto j : { FALL, RISE }) {
			inreq[i][j].val = tmp_req[i][j];
			inreq[i][j].tag = outreq[i][j].tag;
		}
	}
}
void Cell::print() {
	static int cnt = 0;
	if (cnt == 0) {
		cout << "bla bla " << endl;
		cnt = 1;
	}
}

void Cell::resetReq() {
	for (auto m : PinData) {
		m.second.resetReq();
	}
}

pin Cell::getWCpin(MAXMIN M) {
	int tmp = INT_MAX;
	pin p;
	for (auto i : PinData) {
		auto& pinTime = i.second;
		if (pinTime.GetWCTmpMarg(M) < tmp) {
			tmp = pinTime.GetWCTmpMarg(M);
			p = i.first;
		}

	}
	return p;
}
