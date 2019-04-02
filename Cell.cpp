/*
 * Cell.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Cell.h"

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

	return Template->getDelay(in, out, AnlsType, Tr, inslope, outload);
}
slope Cell::getSlope(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
		slope inslope, load outload) {
	return Template->getSlope(in, out, AnlsType, Tr, inslope, outload);
}
bool Cell::PossiblTr(input_pin in, output_pin out, InOutTr Tr) {
	int Poss = this->Template->delayTable[make_pair(in, out)].GetTableVal(MAX,
			Tr, 0, 0);
	return Poss == -1 ? false : true;
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
	delay del[2][2] = { INT_MAX, INT_MAX, INT_MIN, INT_MIN };
	slope slp[2][2] = { INT_MAX, INT_MAX, INT_MIN, INT_MIN };
	PinDat& Dat = PinData[outPin];
	slope tmp_slp;
	delay tmp_del;
	/*
	 * find the max and min delay/slope from all the outputs to inputs along with
	 */
	for (auto& pair : inMap) {
		auto in_Pin = pair.first;
		auto inNet = pair.second;
		for (const auto i : { MIN, MAX }) {
			for (const auto j : { FALL, RISE }) { //output transition
				for (const auto q : { FALL, RISE }) { // input transition
					InOutTr ioTr = GetInOut(q, j);
					tmp_slp = getSlope(in_Pin, outPin, i, ioTr,
							PinData[in_Pin].tmp_slope[i][q], getCout(outPin));
					tmp_del = getDelay(in_Pin, outPin, i, ioTr, tmp_slp,
							getCout(outPin));
					tmp_del += PinData[in_Pin].tmp_vld[i][q].val;
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
	ready_inputs=0;
}

void Cell::print() {
	static int cnt = 0;
	if (cnt == 0) {
		cout << "bla bla " << endl;
		cnt = 1;
	}
}

