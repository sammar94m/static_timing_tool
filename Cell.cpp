/*
 * Cell.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Cell.h"


//virtual void saveclkdat(clockdat cd){};

load Cell::getCin(pin input) {
	return Template->temp_pinLoadMap[input];
}
const map<output_pin, Net*>& Cell::getOutMap() {
	return outMap;
}
load Cell::getCout(output_pin out){
	load tmp=0;
	Net* outNet=this->outMap[out];
	for(auto RcvIT=outNet->receivers.begin();RcvIT!=outNet->receivers.end();RcvIT++){
		tmp+=((*RcvIT)->cell)->getCin((*RcvIT)->inPin);
	}
	return tmp;
}

string Cell::getName() {
	return name;
}
delay Cell::getDelay(input_pin in, output_pin out, MAXMIN AnlsType, Transitions Tr,
		slope inslope, load outload) {

	return Template->getDelay(in, out, AnlsType, Tr, inslope, outload);
}
slope Cell::getSlope(input_pin in, output_pin out, MAXMIN AnlsType, Transitions Tr,
		slope inslope, load outload) {
	return Template->getSlope(in, out, AnlsType, Tr, inslope, outload);
}
bool Cell::PossiblTr(input_pin in, output_pin out, Transitions Tr) {
	int Poss = this->Template->delayTable[make_pair(in, out)].GetTableVal(MAX,
			Tr, 0, 0);
	return Poss == -1 ? false : true;
}

void Cell::updateWCdat(pin PIN, margin Margin, MAXMIN mode,
		Transitions Tr/*FALL OR RISE*/) {
	if (WCmarg[mode].find(PIN) == WCmarg[mode].end()) {
		WCmarg[mode][PIN] = Margin;
		WCtr[mode] = Tr;
	} else {
		WCmarg[mode][PIN] =
				WCmarg[mode][PIN] > Margin ? Margin : WCmarg[mode][PIN];
		WCtr[mode] = WCmarg[mode][PIN] > Margin ? Tr : WCtr[mode];
	}
}
margin Cell::getWCdat(input_pin input, MAXMIN MODE) {
	return WCmarg[MODE][input];
}
static void Cell::print() {
	static int cnt = 0;
	if (cnt == 0) {
		cout << "bla bla " << endl;
		cnt = 1;
	}
}

